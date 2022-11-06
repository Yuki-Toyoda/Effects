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
const char kWindowTitle[] = "吸収エフェクト";

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

/******** プレイヤー **********/
//position ... x, y座標
//radius ... 半径
//theta ... 角度
//degree ... 実角度
//speed ... 移動速度
struct Player {
	Vector2D position;
	float radius;
	float theta;
	float degree;
	float speed;
};
/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

/******** エフェクト更新処理 **********/
void EffectUpdate(Effect& boostEffect, Player& player) {

	/******** 初期化 **********/
	if (boostEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		boostEffect.position = { My::RandomF(player.position.x - 10.0f, player.position.x - 10.0f, 1), My::RandomF(player.position.y - 10.0f, player.position.y - 10.0f, 1) };
		boostEffect.size = { 20, 20 };

		//透明度
		boostEffect.currentAlpha = 0xFF;

		//イージング用変数をリセット
		boostEffect.time = 0.0f;

		//エフェクト表示
		boostEffect.isEnd = false;

		//初期化フラグfalse
		boostEffect.init = false;

	}

	/******** 終了処理 **********/
	if (boostEffect.elapseFrame >= 100 || boostEffect.currentAlpha > 0xFF || boostEffect.size.x <= 0 || boostEffect.size.y <= 0) {

		//エフェクト消去
		boostEffect.isEnd = true;

		//経過フレーム初期化
		boostEffect.elapseFrame = 0.0f;

		//初期化フラグfalse
		boostEffect.init = true;

	}

	/******** 更新処理 **********/
	if (boostEffect.isEnd == false) {

		boostEffect.size.x -= 0.25f;
		boostEffect.size.y -= 0.25f;

		boostEffect.currentAlpha -= 0x04;

		//経過フレーム加算
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
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	/*********************************
		変数宣言ここから
	*********************************/

	My::SetSrand();

	//矩形用テクスチャ読み込み
	int sampleTexture = Novice::LoadTexture("white1x1.png");
	int circleTexture = Novice::LoadTexture("./circle.png");

	//経過フレーム記録変数
	float frame = 0.0f;

	/******** エフェクト関係 **********/
	//表示可能エフェクト数
	const int maxEffects = 10;

	//エフェクト
	Effect boostEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		boostEffect[i] = {
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

	//プレイヤー
	Player player = {
		{640.0f, 360.0f},
		30.0f,
		0.0f,
		0.0f,
		5.0f
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

		//フレームが一定以上になったらエフェクト生成
		for (int i = 0; i < maxEffects; i++) {
			if (frame >= 30.0f) {
				if (boostEffect[i].isEnd == true) {
					boostEffect[i].init = true;
				}
				frame = 0.0f;
			}
			else {
				frame += 1.0f;
			}
		}

		//エフェクト更新処理
		for (int i = 0; i < maxEffects; i++) {
			EffectUpdate(boostEffect[i], player);
		}

		/******** 移動 **********/
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

		Novice::ScreenPrintf(0, 20, "frame : %4.2f", frame);

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
					32, 32,

					circleTexture,
					0xFFFFFFFF00 + boostEffect[i].currentAlpha
				);
			}
		}

		Novice::DrawQuad(
			player.position.x - player.radius,
			player.position.y + player.radius,

			player.position.x + player.radius,
			player.position.y + player.radius,

			player.position.x - player.radius,
			player.position.y - player.radius,

			player.position.x + player.radius,
			player.position.y - player.radius,

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