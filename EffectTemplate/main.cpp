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
const char kWindowTitle[] = "プレイヤー死亡エフェクト";

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
void DeathParticleUpdate(Effect& playerDeathEffect, Player& player) {

	if (playerDeathEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		playerDeathEffect.position = { player.position.x, player.position.y };
		playerDeathEffect.velocity = { 7.0f, 7.0f };
		playerDeathEffect.size = { My::RandomF(5.0f, 15.0f, 1), playerDeathEffect.size.x };

		//経過フレーム初期化
		playerDeathEffect.elapseFrame = 0.0f;

		playerDeathEffect.position.x += (cosf(playerDeathEffect.theta) * 10);
		playerDeathEffect.position.y += -(sinf(playerDeathEffect.theta) * 10);

		//エフェクト表示
		playerDeathEffect.isEnd = false;

		//初期化フラグfalse
		playerDeathEffect.init = false;

	}

	if (playerDeathEffect.elapseFrame >= 100 || playerDeathEffect.size.x == 0) {

		//エフェクト消去
		playerDeathEffect.isEnd = true;

		//経過フレーム初期化
		playerDeathEffect.elapseFrame = 0.0f;

	}

	if (playerDeathEffect.isEnd == false) {

		playerDeathEffect.position.x += (cosf(playerDeathEffect.theta) * playerDeathEffect.velocity.x);
		playerDeathEffect.position.y += -(sinf(playerDeathEffect.theta) * playerDeathEffect.velocity.y);

		playerDeathEffect.velocity.x -= 0.25f;
		playerDeathEffect.velocity.y -= 0.25f;

		//経過フレーム加算
		playerDeathEffect.elapseFrame += 1.0f;

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

	//一回に実行するエフェクトの数
	int possibleTimes = 0;

	/******** エフェクト関係 **********/
	//表示可能エフェクト数
	const int maxEffects = 12;

	//エフェクト
	Effect playerDeathEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		playerDeathEffect[i] = {
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
				playerDeathEffect[i].theta = (360 / maxEffects) * i;
				playerDeathEffect[i].theta = playerDeathEffect[i].theta * (M_PI / 180.0f);
				playerDeathEffect[i].init = true;
			}
		}

		for (int i = 0; i < maxEffects; i++) {
			DeathParticleUpdate(playerDeathEffect[i], player);
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
			if (!playerDeathEffect[i].isEnd) {
				Novice::DrawQuad(
					playerDeathEffect[i].position.x - playerDeathEffect[i].size.x,
					playerDeathEffect[i].position.y + playerDeathEffect[i].size.y,

					playerDeathEffect[i].position.x + playerDeathEffect[i].size.x,
					playerDeathEffect[i].position.y + playerDeathEffect[i].size.y,

					playerDeathEffect[i].position.x - playerDeathEffect[i].size.x,
					playerDeathEffect[i].position.y - playerDeathEffect[i].size.y,

					playerDeathEffect[i].position.x + playerDeathEffect[i].size.x,
					playerDeathEffect[i].position.y - playerDeathEffect[i].size.y,

					0, 0,
					32, 32,

					circleTexture,
					WHITE
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
