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
	Vector2D velocity;
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
};

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

/******** エフェクト更新処理 **********/
void AnniEffectUpdate(Effect& anniEffect, Object& object) {

	if (anniEffect.init == true) {

		//位置等を初期化
		anniEffect.position = { My::RandomF(object.position.x - object.radius.x / 2, object.position.x + object.radius.x / 2, 1), My::RandomF(object.position.y - object.radius.y, object.position. y+ object.radius.y, 1) };
		anniEffect.startPosition = { anniEffect.position.x, anniEffect.position.y };
		anniEffect.endPosition = { anniEffect.startPosition.x, anniEffect.startPosition.y - 100 };
		anniEffect.velocity = { My::RandomF(5.0f, 7.0f, 1), My::RandomF(5.0f, 7.0f, 1) };
		anniEffect.size = { 5, 5 };

		anniEffect.time = 0.0f;

		//エフェクトが向かう方向をランダムにする
		anniEffect.theta = My::Random(0, 180);
		anniEffect.theta = anniEffect.theta * (M_PI / 180.0f);

		//エフェクト表示
		anniEffect.isEnd = false;

		//初期化フラグfalse
		anniEffect.init = false;

	}

	if (anniEffect.elapseFrame >= 100) {

		//エフェクト消去
		anniEffect.isEnd = true;

		object.radius = {0.0f, 0.0f};

		//経過フレーム初期化
		anniEffect.elapseFrame = 0.0f;

	}

	if (anniEffect.isEnd == false) {

		anniEffect.time += 0.01f;

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
	//表示可能エフェクト数
	const int maxEffects = 30;

	//エフェクト
	Effect anniEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		anniEffect[i] = {
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{1.0f, 1.0f},
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

		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			for (int i = 0; i < maxEffects; i++) {
				object.radius = {100.0f, 20.0f};
				anniEffect[i].init = true;
			}
		}

		for (int i = 0; i < maxEffects; i++) {
			AnniEffectUpdate(anniEffect[i], object);
		}

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/
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
					WHITE
				);
			}
		}

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
