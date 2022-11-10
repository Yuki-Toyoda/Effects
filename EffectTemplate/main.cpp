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
const char kWindowTitle[] = "消滅エフェクト";

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
void AnniEffectUpdate(Effect& anniEffect, Object& object, bool& next, int& effectQuantity) {
	if (anniEffect.init == true) {

		//位置等を初期化
		anniEffect.position = { My::RandomF(object.position.x - object.radius.x / 2, object.position.x + object.radius.x / 2, 1), My::RandomF(object.position.y - object.radius.y / 2, object.position.y+ object.radius.y / 2, 1) };
		anniEffect.startPosition = { anniEffect.position.x, anniEffect.position.y };
		anniEffect.endPosition = { anniEffect.startPosition.x, anniEffect.position.y - My::RandomF(200.0f, 300.0f, 1) };

		anniEffect.size = { My::RandomF(5.0f, 7.5f, 0), anniEffect.size.x };
		anniEffect.startSize = { anniEffect.size.x, anniEffect.size.x };

		anniEffect.strength = My::RandomF(60.0f, 100.0f, 0);
		anniEffect.startStrength = anniEffect.strength;
		anniEffect.amplitude = 0.5f;

		anniEffect.time = 0.0f;

		next = false;

		//エフェクト表示
		anniEffect.isEnd = false;

		effectQuantity += 1;

		//初期化フラグfalse
		anniEffect.init = false;

	}

	if (anniEffect.size.x <= 0.0f) {

		//エフェクト消去
		anniEffect.isEnd = true;

		//経過フレーム初期化
		anniEffect.elapseFrame = 0.0f;

	}

	if (anniEffect.elapseFrame >= 3.0f - (3.0f / maxEffects) * effectQuantity * 1.5f && object.radius.x > 0.0f) {

		next = true;

		//経過フレーム初期化
		anniEffect.elapseFrame = 0.0f;
	}

	if (object.radius.x < 0.0f) {
		effectQuantity = 0;
	}

	if (anniEffect.isEnd == false) {

		//粒子エフェクトのイージング処理
		anniEffect.time += 0.01f;
		anniEffect.easeTime = 1.0f - powf(1.0f - anniEffect.time, 3.0f);

		//粒子エフェクトのサイズ変更
		anniEffect.size.x = (1.0 - anniEffect.easeTime) * anniEffect.startSize.x + anniEffect.easeTime * 0;
		anniEffect.size.y = (1.0 - anniEffect.easeTime) * anniEffect.startSize.y + anniEffect.easeTime * 0;

		anniEffect.theta += M_PI / anniEffect.strength;
		anniEffect.position.x += sinf(anniEffect.theta);

		anniEffect.strength = (1.0 - anniEffect.easeTime) * anniEffect.startStrength + anniEffect.easeTime * 30;

		//粒子エフェクトを徐々に上に
		anniEffect.easeTime = anniEffect.time * anniEffect.time;

		anniEffect.currentAlpha = (1.0 - anniEffect.easeTime) * 0xFF + anniEffect.easeTime * 0x00;
		anniEffect.position.y = (1.0 - anniEffect.easeTime) * anniEffect.startPosition.y + anniEffect.easeTime * anniEffect.endPosition.y;

		//経過フレーム加算
		anniEffect.elapseFrame += 1.0f;

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
	char keys[256] = {0};
	char preKeys[256] = {0};

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
	Effect anniEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		anniEffect[i] = {
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
			0xFF,
			false,
			true
		};
	}

	Object object{
		{640.0f, 360.0f},
		{0.0f, 0.0f}
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

		if (Novice::IsTriggerMouse(0)) {

			Novice::GetMousePosition(&mousePosX, &mousePosY);

			object.position = { (float)mousePosX, (float)mousePosY };

			object.radius = { 200.0f, 200.0f };
			object.velocity = 1.0f;
			object.acceleration = 0.05f;
			next = true;

		}


		if (next == true) {
			for (int i = 0; i < maxEffects; i++) {
				if (next == true && anniEffect[i].isEnd == true) {
					anniEffect[i].init = true;
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
			AnniEffectUpdate(anniEffect[i], object, next, effectQuantity);
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
			if (!anniEffect[i].isEnd) {
				Novice::DrawQuad(
					anniEffect[i].position.x - anniEffect[i].size.x,
					anniEffect[i].position.y + anniEffect[i].size.y,

					anniEffect[i].position.x + anniEffect[i].size.x,
					anniEffect[i].position.y + anniEffect[i].size.y,

					anniEffect[i].position.x - anniEffect[i].size.x,
					anniEffect[i].position.y - anniEffect[i].size.y,

					anniEffect[i].position.x + anniEffect[i].size.x,
					anniEffect[i].position.y - anniEffect[i].size.y,

					0, 0,
					32, 32,

					circleTexture,
					0xFFFFFFFF00 + anniEffect[i].currentAlpha
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
