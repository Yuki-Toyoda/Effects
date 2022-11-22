#include <Novice.h>

#define _USE_MATH_DEFINES
#include <math.h>

#include <MyFunc.h>
#include <Vector2D.h>


/*================================
	コピペ用↓
=================================*/

/*********************************
	大見出しコピペ
*********************************/

/******** 小見出しコピペ用 **********/

/*================================
	コピペ用↑
=================================*/

/******** ウィンドウ名の指定 **********/
const char kWindowTitle[] = "ブーストエフェクト";

/******** ウィンドウサイズの指定 **********/
const int kWinodowWidth = 1280; //x
const int kWindowHeight = 720; //y

/*********************************
	定数の宣言ここまで
*********************************/

/*********************************
	構造体宣言ここから
*********************************/

/******** エフェクト **********/
//position ... x, y座標
//size ... 矩形のサイズ
//velocity ... 動く速度
//acceleration ... 加速度
//theta ... 回転角
//elapseFrame ... 存在時間
//time ... 中心点に向かう時間
//easeTime ... イージング用時間
//currentAlpha ... 透明度
//init ... 初期化
//isEnd ... エフェクトが終了しているか
struct Effect {
	Vector2D position;
	Vector2D startPosition;
	Vector2D endPosition;
	Vector2D size;
	Vector2D startSize;
	Vector2D endSize;
	Vector2D velocity;
	float startStrength;
	float strength;
	float amplitude;
	float acceleration;
	float theta;
	float nextFrame;
	float elapseFrame;
	float alphaTime;
	float easeAlphaTime;
	float moveTime;
	float easeMoveTime;
	unsigned int color;

	bool levitation;
	bool init;
	bool isEnd;
};

struct Object {
	Vector2D position;
	Vector2D radius;
	Vector2D startRadius;
	float velocity;
	float acceleration;
};

//表示可能エフェクト数
const int maxEffects = 50;

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

int IntEaseIn(float t, long int b, long int c) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (1.0f - easeT) * b + easeT * c;
}

unsigned int ColorEasing(float t, unsigned int startColor, unsigned int endColor) {

	unsigned int red = IntEaseIn(t, (((startColor & 0xFF000000) >> 24) & 0xFF), (((endColor & 0xFF000000) >> 24) & 0xFF));
	unsigned int green = IntEaseIn(t, (((startColor & 0x00FF0000) >> 16) & 0xFF), (((endColor & 0x00FF0000) >> 16) & 0xFF));
	unsigned int blue = IntEaseIn(t, (((startColor & 0x0000FF00) >> 8) & 0xFF), (((endColor & 0x0000FF00) >> 8) & 0xFF));
	unsigned int alpha = IntEaseIn(t, (((startColor & 0x000000FF)) & 0xFF), (((endColor & 0x000000FF)) & 0xFF));

	unsigned int color = IntEaseIn(t, (startColor & 0xFF), (endColor & 0xFF));
	return (red << 24) + (green << 16) + (blue << 8) + alpha;

}

/******** エフェクト更新処理 **********/
void BoosterEffectUpdate(Effect& boosterEffect, Object& object, bool& next) {
	if (boosterEffect.init == true) {
		
		boosterEffect.nextFrame = 1;

		//位置等を初期化
		boosterEffect.position = { My::RandomF(object.position.x - object.radius.x, object.position.x + object.radius.x, 1), My::RandomF(object.position.y, object.position.y + object.radius.y * 2, 1) };
		boosterEffect.startPosition = { boosterEffect.position.x, boosterEffect.position.y };
		boosterEffect.endPosition = { My::RandomF(boosterEffect.startPosition.x - 30.0f, boosterEffect.startPosition.x + 30.0f, 1), boosterEffect.position.y + My::RandomF(200.0f, 250.0f, 1) };

		boosterEffect.size = { 3.0f, 3.0f };
		boosterEffect.startSize = { boosterEffect.size.x, boosterEffect.size.y };
		boosterEffect.endSize = { My::RandomF(15.0f, 20.0f, 1), boosterEffect.endSize.x};

		boosterEffect.strength = My::RandomF(60.0f, 100.0f, 0);
		boosterEffect.startStrength = boosterEffect.strength;
		boosterEffect.amplitude = 0.5f;

		boosterEffect.alphaTime = 0.0f;
		boosterEffect.moveTime = 0.0f;

		boosterEffect.levitation = false;

		next = false;

		//エフェクト表示
		boosterEffect.isEnd = false;

		//effectQuantity += 1;

		//初期化フラグfalse
		boosterEffect.init = false;

	}

	if (boosterEffect.moveTime >= 1.0f) {

		//エフェクト消去
		boosterEffect.isEnd = true;

		//経過フレーム初期化
		boosterEffect.elapseFrame = 0.0f;

	}

	if (boosterEffect.elapseFrame == boosterEffect.nextFrame) {

		next = true;

	}

	if (boosterEffect.isEnd == false) {

		if (boosterEffect.alphaTime < 1.0f && boosterEffect.levitation == false) {
			//粒子エフェクトのイージング処理
			boosterEffect.alphaTime += 0.03f;
			boosterEffect.easeAlphaTime = 1.0f - powf(1.0f - boosterEffect.alphaTime, 3.0f);

			//粒子エフェクトのサイズ変更
			boosterEffect.size.x = (1.0 - boosterEffect.easeAlphaTime) * boosterEffect.startSize.x + boosterEffect.easeAlphaTime * boosterEffect.endSize.x;
			boosterEffect.size.y = (1.0 - boosterEffect.easeAlphaTime) * boosterEffect.startSize.y + boosterEffect.easeAlphaTime * boosterEffect.endSize.y;

			
		}
		else if (boosterEffect.alphaTime < 1.0f && boosterEffect.levitation == true) {
			//粒子エフェクトのイージング処理
			boosterEffect.alphaTime += 0.01f;
			boosterEffect.easeAlphaTime = 1.0f - powf(1.0f - boosterEffect.alphaTime, 3.0f);

			boosterEffect.easeAlphaTime = boosterEffect.alphaTime * boosterEffect.alphaTime;

		}

		if (boosterEffect.moveTime < 1.0f) {

			boosterEffect.moveTime += 0.01f;
			boosterEffect.easeMoveTime = 1.0f - powf(1.0f - boosterEffect.moveTime, 3.0f);

			boosterEffect.position.x = (1.0 - boosterEffect.easeMoveTime) * boosterEffect.startPosition.x + boosterEffect.easeMoveTime * boosterEffect.endPosition.x;
			boosterEffect.position.y = (1.0 - boosterEffect.easeMoveTime) * boosterEffect.startPosition.y + boosterEffect.easeMoveTime * boosterEffect.endPosition.y;

			boosterEffect.color = ColorEasing(boosterEffect.moveTime, 0xFFFF7FFF, 0xFF000000);

		}
		else {
			boosterEffect.moveTime = 1.0f;
		}

		//エフェクトを徐々に消滅させる
		if (boosterEffect.alphaTime >= 0.3f && boosterEffect.levitation == false) {
			boosterEffect.levitation = true;
			boosterEffect.alphaTime = 0.0f;
		}

		//経過フレーム加算
		boosterEffect.elapseFrame += 1.0f;

	}

}

/*********************************
	関数宣言ここまで
*********************************/

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWinodowWidth, kWindowHeight);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	/*********************************
		変数宣言ここから
	*********************************/

	//矩形用テクスチャ読み込み
	int sampleTexture = Novice::LoadTexture("white1x1.png");
	int circleTexture = Novice::LoadTexture("./circle.png");

	int trigger = false;

	/******** エフェクト関係 **********/

	bool next = false;

	int mousePosX = 0;
	int mousePosY = 0;

	//エフェクト
	Effect effect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		effect[i] = {
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{1.0f, 1.0f},
			{1.0f, 1.0f},
			{1.0f, 1.0f},
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0xFF,
			false,
			false,
			true
		};
	}

	Object object{
		{640.0f, 360.0f},
		{10.0f, 10.0f}
	};

	/*********************************
		変数宣言ここまで
	*********************************/

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		/*********************************
			更新処理ここから
		*********************************/

		Novice::GetMousePosition(&mousePosX, &mousePosY);

		object.position = { (float)mousePosX, (float)mousePosY };

		if (Novice::IsTriggerMouse(0)) {
			next = true;
		}


		if (next == true) {
			for (int i = 0; i < maxEffects; i++) {
				if (next == true && effect[i].isEnd == true) {
					effect[i].init = true;
					next = false;
				}
			}
		}

		if (object.radius.x > 0) {
			object.radius.x -= object.velocity;
			object.radius.y -= object.velocity;
			object.velocity += object.acceleration;
		}
		else {
			object.radius.x = 0;
		}

		for (int i = 0; i < maxEffects; i++) {
			BoosterEffectUpdate(effect[i], object, next);
		}

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/

		//Novice::SetBlendMode(kBlendModeNormal);

		Novice::DrawBox(
			0, 0,
			1280, 720,
			0.0f,
			BLACK,
			kFillModeSolid
		);

		Novice::DrawQuad(
			object.position.x - object.radius.x / 2,
			object.position.y - object.radius.y / 2,

			object.position.x + object.radius.x / 2,
			object.position.y - object.radius.y / 2,

			object.position.x - object.radius.x / 2,
			object.position.y + object.radius.y / 2,

			object.position.x + object.radius.x / 2,
			object.position.y + object.radius.y / 2,

			0, 0,
			1, 1,

			sampleTexture,
			WHITE
		);

		/******** エフェクト描画 **********/

		Novice::SetBlendMode(kBlendModeAdd);

		for (int i = 0; i < maxEffects; i++) {
			if (!effect[i].isEnd) {
				Novice::DrawQuad(
					effect[i].position.x - effect[i].size.x,
					effect[i].position.y + effect[i].size.y,

					effect[i].position.x + effect[i].size.x,
					effect[i].position.y + effect[i].size.y,

					effect[i].position.x - effect[i].size.x,
					effect[i].position.y - effect[i].size.y,

					effect[i].position.x + effect[i].size.x,
					effect[i].position.y - effect[i].size.y,

					0, 0,
					32, 32,

					circleTexture,
					effect[i].color
				);
			}
		}

		Novice::SetBlendMode(kBlendModeNormal);

		for (int i = 0; i <= 25; i++) {
			Novice::ScreenPrintf(0, 20 * i, "color[%d] : %x", i ,effect[i].color);
		}

		/*********************************
			描画処理ここまで
		*********************************/

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
