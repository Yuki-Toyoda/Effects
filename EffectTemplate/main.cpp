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
const char kWindowTitle[] = "回復エフェクト";

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
struct HealEffect {
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
void HealEffectUpdate(HealEffect& healEffect, Player& player) {

	if (healEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		healEffect.position = { player.position.x, player.position.y };
		healEffect.velocity = { My::RandomF(5.0f, 7.0f, 1), My::RandomF(5.0f, 7.0f, 1) };
		healEffect.size = { 1, 1 };

		healEffect.time = 0.0f;

		healEffect.currentAlpha = 0xFF;

		//エフェクトが向かう方向をランダムにする
		healEffect.theta = My::Random(0, 180);
		healEffect.theta = healEffect.theta * (M_PI / 180.0f);

		//エフェクト表示
		healEffect.isEnd = false;

		//初期化フラグfalse
		healEffect.init = false;

	}

	if (healEffect.elapseFrame >= 100 || healEffect.time < 1.00f) {

		if (healEffect.currentAlpha > 0xFF) {
			//エフェクト消去
			healEffect.isEnd = true;
			//経過フレーム初期化
			healEffect.elapseFrame = 0.0f;
		}
		else {
			healEffect.currentAlpha -= 0x06;
		}

	}

	if (healEffect.isEnd == false) {

		healEffect.position = { player.position.x, player.position.y };

		if (healEffect.time < 1.35f) {
			healEffect.time += 0.035f;
		}
		healEffect.easeTime = 1.0f - powf(1.0f - healEffect.time, 3.0f);

		healEffect.size.x = (1.0 - healEffect.easeTime) * 1 + healEffect.easeTime * 75;
		healEffect.size.y = (1.0 - healEffect.easeTime) * 1 + healEffect.easeTime * 75;

		//経過フレーム加算
		healEffect.elapseFrame += 1.0f;

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
	int wireCircleTexture = Novice::LoadTexture("./wireCircle.png");

	/******** エフェクト関係 **********/
	//表示可能エフェクト数
	const int maxEffects = 1;

	//エフェクト
	HealEffect healEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		healEffect[i] = {
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

		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			for (int i = 0; i < maxEffects; i++) {
				healEffect[i].init = true;
			}
		}

		for (int i = 0; i < maxEffects; i++) {
			HealEffectUpdate(healEffect[i], player);
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
		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/
		/******** エフェクト描画 **********/
		for (int i = 0; i < maxEffects; i++) {
			if (!healEffect[i].isEnd) {
				Novice::DrawQuad(
					healEffect[i].position.x - healEffect[i].size.x,
					healEffect[i].position.y + healEffect[i].size.y,

					healEffect[i].position.x + healEffect[i].size.x,
					healEffect[i].position.y + healEffect[i].size.y,

					healEffect[i].position.x - healEffect[i].size.x,
					healEffect[i].position.y - healEffect[i].size.y,

					healEffect[i].position.x + healEffect[i].size.x,
					healEffect[i].position.y - healEffect[i].size.y,

					0, 0,
					32, 32,

					wireCircleTexture,
					0xFFFFFFFF00 + healEffect[i].currentAlpha
				);
			}
		}

		Novice::DrawQuad(
			player.position.x - player.radius,
			player.position.y - player.radius, 

			player.position.x + player.radius,
			player.position.y - player.radius,

			player.position.x - player.radius,
			player.position.y + player.radius,

			player.position.x + player.radius,
			player.position.y + player.radius,

			0, 0,
			1, 1,

			sampleTexture,
			RED
		);

		Novice::ScreenPrintf(0, 10, "time %4.2f", healEffect[0].time);

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
