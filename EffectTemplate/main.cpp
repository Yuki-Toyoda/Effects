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
const char kWindowTitle[] = "煙エフェクト";

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
	float time;
	float easeTime;
	unsigned int currentAlpha;

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

/******** エフェクト更新処理 **********/
void SmokeEffectUpdate(Effect& smokeEffect, Object& object, bool& next, int& effectQuantity) {
	if (smokeEffect.init == true) {

		smokeEffect.nextFrame = 3.0f;

		//位置等を初期化
		smokeEffect.position = { My::RandomF(object.position.x - object.radius.x, object.position.x + object.radius.x, 1), My::RandomF(object.position.y - object.radius.y, object.position.y + object.radius.y, 1) };
		smokeEffect.startPosition = { smokeEffect.position.x, smokeEffect.position.y };
		smokeEffect.endPosition = { smokeEffect.startPosition.x, smokeEffect.position.y + My::RandomF(200.0f, 300.0f, 1) };

		smokeEffect.size = { My::RandomF(10.0f, 15.0f, 0), smokeEffect.size.x };
		smokeEffect.startSize = { smokeEffect.size.x, smokeEffect.size.x };
		smokeEffect.endSize = { My::RandomF(25.0f, 30.0f, 0), smokeEffect.endSize.x };

		smokeEffect.strength = My::RandomF(60.0f, 100.0f, 0);
		smokeEffect.startStrength = smokeEffect.strength;
		smokeEffect.amplitude = 0.5f;

		smokeEffect.currentAlpha = 0x01;

		smokeEffect.time = 0.0f;

		smokeEffect.levitation = false;

		next = false;

		//エフェクト表示
		smokeEffect.isEnd = false;

		//effectQuantity += 1;

		//初期化フラグfalse
		smokeEffect.init = false;

	}

	if (smokeEffect.currentAlpha == 0x00) {

		//エフェクト消去
		smokeEffect.isEnd = true;

		//経過フレーム初期化
		smokeEffect.elapseFrame = 0.0f;

	}

	if (smokeEffect.elapseFrame == smokeEffect.nextFrame) {
		next = true;
	}

	if (smokeEffect.elapseFrame == 90) {
		effectQuantity = 0;
	}

	if (smokeEffect.isEnd == false) {

		if (smokeEffect.time < 0.8f && smokeEffect.levitation == false) {
			//粒子エフェクトのイージング処理
			smokeEffect.time += 0.01f;
			smokeEffect.easeTime = 1.0f - powf(1.0f - smokeEffect.time, 3.0f);
			//粒子エフェクトのサイズ変更
			smokeEffect.size.x = (1.0 - smokeEffect.easeTime) * smokeEffect.startSize.x + smokeEffect.easeTime * smokeEffect.endSize.x;
			smokeEffect.size.y = (1.0 - smokeEffect.easeTime) * smokeEffect.startSize.y + smokeEffect.easeTime * smokeEffect.endSize.y;

			smokeEffect.currentAlpha = (1.0 - smokeEffect.easeTime) * 0x01 + smokeEffect.easeTime * 0xFF;

		}
		else if (smokeEffect.time < 1.0f && smokeEffect.levitation == true) {
			//粒子エフェクトのイージング処理
			smokeEffect.time += 0.02f;
			smokeEffect.easeTime = 1.0f - powf(1.0f - smokeEffect.time, 3.0f);

			smokeEffect.currentAlpha = (1.0 - smokeEffect.easeTime) * 0xFF + smokeEffect.easeTime * 0x00;
			smokeEffect.position.y = (1.0 - smokeEffect.easeTime) * smokeEffect.startPosition.y + smokeEffect.easeTime * smokeEffect.endPosition.y;

		}


		if(smokeEffect.time >= 0.8f && smokeEffect.levitation == false) {
			smokeEffect.levitation = true;
			smokeEffect.time = 0.0f;
		}

		//経過フレーム加算
		smokeEffect.elapseFrame += 1.0f;

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

	/******** エフェクト関係 **********/

	int effectQuantity = 0;
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
			0xFF,
			false,
			false,
			true
		};
	}

	Object object{
		{640.0f, 360.0f},
		{20.0f, 20.0f}
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

		for (int i = 0; i < maxEffects; i++) {
			SmokeEffectUpdate(effect[i], object, next, effectQuantity);
		}

		Novice::ScreenPrintf(0, 10, "ve : %d", effect[0].levitation);
		Novice::ScreenPrintf(0, 30, "ve : %d", effect[1].levitation);
		Novice::ScreenPrintf(0, 50, "ve : %d", effect[2].levitation);
		Novice::ScreenPrintf(0, 70, "ve : %d", effect[3].levitation);
		Novice::ScreenPrintf(0, 90, "ve : %d", effect[4].levitation);

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/

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
			RED
		);

		/******** エフェクト描画 **********/
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
					0xFFFFFFFF00 + effect[i].currentAlpha
				);
			}
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
