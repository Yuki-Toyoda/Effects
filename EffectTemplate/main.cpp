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
//回転角
//elapseFrame ... 存在時間
//init ... 初期化
//play ... エフェクトをプレイするか
//isEnd ... エフェクトが終了しているか
struct Effect {
	Vector2D startPosition;
	Vector2D position;
	Vector2D size;
	Vector2D velocity;
	float time;
	float acceleration;
	float theta;
	float elapseFrame;
	bool init;
	bool play;
	bool isEnd;
};

struct Player {

	Vector2D position;
	float radius;
	float speed;

};

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

/******** エフェクト更新処理 **********/
void BoostEffectUpdate(Effect& boostEffect, Player& player) {
	if (boostEffect.init && boostEffect.play) {
		//エフェクトの位置、速度、サイズ初期化
		boostEffect.position = { My::RandomF(player.position.x - 15.0f, player.position.x + 15.0f, 1), My::RandomF(player.position.y - 15.0f, player.position.y + 15.0f, 1) };
		boostEffect.startPosition = boostEffect.position;
		boostEffect.velocity = { My::RandomF(-1.0f, 1.0f, 1), 2.5f };
		boostEffect.acceleration = 0.01f;
		boostEffect.size = { 15.0f, 15.0f };

		//エフェクト表示
		boostEffect.isEnd = false;

		//初期化フラグfalse
		boostEffect.init = false;
	}

	if (boostEffect.size.x <= 0 || boostEffect.size.y <= 0) {

		boostEffect.isEnd = true;
		boostEffect.init = true;
		boostEffect.elapseFrame = 0.0f;

	}

	if (boostEffect.isEnd == false) {

		boostEffect.size.x -= boostEffect.velocity.y / 3.0f;
		boostEffect.size.y -= boostEffect.velocity.y / 3.0f;

		boostEffect.elapseFrame += 1.0f;

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

	float frame = 0.0f;

	/******** エフェクト関係 **********/
	//表示可能エフェクト数
	const int maxEffects = 20;

	//エフェクト
	Effect boostEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		boostEffect[i] = {
			{640.0f, 360.0f},
			{640.0f, 360.0f},
			{16.0f, 16.0f},
			{1.0f, 1.0f},
			0.15f,
			0.0f,
			0.0f,
			0,
			false,
			false,
			true
		};
	}

	Player player{
		{640.0f, 360.0f},
		30.0f,
		10.0f
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

		frame += 1.0f;

		for (int i = 0; i < maxEffects; i++) {
			if (frame >= 3.0f) {
				if (boostEffect[i].isEnd == true) {
					boostEffect[i].init = true;
					boostEffect[i].play = true;
					frame = 0.0f;
				}
			}

		}

		for (int i = 0; i < maxEffects; i++) {
			BoostEffectUpdate(boostEffect[i], player);
		}

		if (keys[DIK_W]) {
			player.position.y -= player.speed;
		}
		if (keys[DIK_A]) {
			player.position.x -= player.speed;
		}
		if (keys[DIK_S]) {
			player.position.y += player.speed;
		}
		if (keys[DIK_D]) {
			player.position.x += player.speed;
		}

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/

		/******** エフェクト描画 **********/
		for (int i = 0; i < maxEffects; i++) {
			if (!boostEffect[i].isEnd) {
				Novice::DrawQuad(
					boostEffect[i].position.x - boostEffect[i].size.x,
					boostEffect[i].position.y + boostEffect[i].size.y,

					boostEffect[i].position.x + boostEffect[i].size.x,
					boostEffect[i].position.y + boostEffect[i].size.y,

					boostEffect[i].position.x - boostEffect[i].size.x,
					boostEffect[i].position.y - boostEffect[i].size.y,

					boostEffect[i].position.x + boostEffect[i].size.x,
					boostEffect[i].position.y - boostEffect[i].size.y,

					0, 0,
					1, 1,

					sampleTexture,
					BLACK
				);
			}
		}

		Novice::DrawEllipse(
		
			player.position.x,
			player.position.y,
			player.radius,
			player.radius,
			0.0f,
			RED,
			kFillModeSolid
		
		);

		Novice::ScreenPrintf(0, 10, "frame : %4.2f", frame);
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
