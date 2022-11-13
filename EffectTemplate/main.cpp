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
void EffectUpdate(Effect& effect, Object& object, bool& next, int& effectQuantity) {
	if (effect.init == true) {

		effect.nextFrame = 1.0f;

		//位置等を初期化
		effect.position = { My::RandomF(object.position.x - object.radius.x / 2, object.position.x + object.radius.x / 2, 1), My::RandomF(object.position.y - object.radius.y / 2, object.position.y + object.radius.y / 2, 1) };
		effect.startPosition = { effect.position.x, effect.position.y };
		effect.endPosition = { effect.startPosition.x, effect.position.y - My::RandomF(200.0f, 300.0f, 1) };

		effect.size = { My::RandomF(1.0f, 2.0f, 0), effect.size.x };
		effect.startSize = { effect.size.x, effect.size.x };

		effect.strength = My::RandomF(60.0f, 100.0f, 0);
		effect.startStrength = effect.strength;
		effect.amplitude = 0.5f;

		effect.time = 0.0f;

		next = false;

		//エフェクト表示
		effect.isEnd = false;

		//effectQuantity += 1;

		//初期化フラグfalse
		effect.init = false;

	}

	if (effect.elapseFrame >= 50.0f) {

		//エフェクト消去
		effect.isEnd = true;

		//経過フレーム初期化
		effect.elapseFrame = 0.0f;

	}

	if (effect.elapseFrame == effect.nextFrame) {
		next = true;
	}

	if (effect.elapseFrame == 90) {
		effectQuantity = 0;
	}

	if (effect.isEnd == false) {

		//粒子エフェクトのイージング処理
		effect.time += 0.01f;
		effect.easeTime = 1.0f - powf(1.0f - effect.time, 3.0f);

		//経過フレーム加算
		effect.elapseFrame += 1.0f;

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

		for (int i = 0; i < maxEffects; i++) {
			EffectUpdate(effect[i], object, next, effectQuantity);
		}

		Novice::ScreenPrintf(0, 10, "Quantity : %d", effectQuantity);

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
