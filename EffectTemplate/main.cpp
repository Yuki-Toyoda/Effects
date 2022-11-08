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

/******** 粒子エフェクト更新処理 **********/
void DeathCircleUpdate(Effect& playerDeathCircleEffect, Player& player,bool& playParticle) {

	if (playerDeathCircleEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		playerDeathCircleEffect.position = { player.position.x, player.position.y };
		playerDeathCircleEffect.velocity = { 7.0f, 7.0f };
		playerDeathCircleEffect.size = { 1.0f, 1.0f };

		//経過フレーム初期化
		playerDeathCircleEffect.elapseFrame = 0.0f;

		//エフェクト表示
		playerDeathCircleEffect.isEnd = false;

		//初期化フラグfalse
		playerDeathCircleEffect.init = false;

	}

	if (playerDeathCircleEffect.elapseFrame >= 100 || playerDeathCircleEffect.size.x <= 0) {

		//エフェクト消去
		playerDeathCircleEffect.isEnd = true;

		//経過フレーム初期化
		playerDeathCircleEffect.elapseFrame = 0.0f;

		playParticle = true;
		playerDeathCircleEffect.size.x = 1;

	}

	if (playerDeathCircleEffect.isEnd == false) {

		playerDeathCircleEffect.size.x += playerDeathCircleEffect.velocity.x;
		playerDeathCircleEffect.size.y += playerDeathCircleEffect.velocity.y;

		playerDeathCircleEffect.velocity.x -= 0.35f;
		playerDeathCircleEffect.velocity.y -= 0.35f;

		//経過フレーム加算
		playerDeathCircleEffect.elapseFrame += 1.0f;

	}
}

/******** 粒子エフェクト更新処理 **********/
void DeathParticleUpdate(Effect& playerDeathEffect, Player& player) {

	if (playerDeathEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		playerDeathEffect.position = { player.position.x, player.position.y };
		playerDeathEffect.velocity = { My::RandomF(15.0f, 20.0f, 1), My::RandomF(15.0f, 20.0f, 1) };
		playerDeathEffect.size = { 10.0f, 10.0f };

		playerDeathEffect.acceleration = 0.7f;

		//経過フレーム初期化
		playerDeathEffect.elapseFrame = 0.0f;

		//エフェクト表示
		playerDeathEffect.isEnd = false;

		//初期化フラグfalse
		playerDeathEffect.init = false;

	}

	if (playerDeathEffect.elapseFrame >= 100) {

		//エフェクト消去
		//playerDeathEffect.isEnd = true;

		//経過フレーム初期化
		playerDeathEffect.elapseFrame = 0.0f;

	}

	if (playerDeathEffect.isEnd == false) {

		playerDeathEffect.position.x += (cosf(playerDeathEffect.theta) * playerDeathEffect.velocity.x);
		playerDeathEffect.position.y += (sinf(playerDeathEffect.theta) * playerDeathEffect.velocity.y);

		if (playerDeathEffect.velocity.x > 0.0f || playerDeathEffect.velocity.y > 0.0f) {
			playerDeathEffect.velocity.x -= playerDeathEffect.acceleration;
			playerDeathEffect.velocity.y -= playerDeathEffect.acceleration;
		}
		else {
			playerDeathEffect.velocity.x = 0.0f;
			playerDeathEffect.velocity.y = 0.0f;
		}

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
	bool playParticle = false;

	/******** エフェクト関係 **********/

	const int maxCircleEffect = 1;

	//表示可能パーティクルエフェクト数
	const int maxParticleEffects = 12;

	//サークルエフェクト
	Effect playerDeathCircleEffect[maxCircleEffect];
	for (int i = 0; i < maxCircleEffect; i++) {
		playerDeathCircleEffect[i] = {
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

	//パーティクルエフェクト
	Effect playerDeathParticleEffect[maxParticleEffects];
	for (int i = 0; i < maxParticleEffects; i++) {
		playerDeathParticleEffect[i] = {
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
			for (int i = 0; i < maxCircleEffect; i++) {
				playerDeathCircleEffect[i].init = true;
			}
		}

		if (playParticle == true) {
			for (int i = 0; i < maxParticleEffects; i++){
				playerDeathParticleEffect[i].theta = (360 / maxParticleEffects) * i;
				playerDeathParticleEffect[i].theta = playerDeathParticleEffect[i].theta * (M_PI / 180.0f);
				playerDeathParticleEffect[i].init = true;
			}
			playParticle = false;
		}

		for (int i = 0; i < maxCircleEffect; i++) {
			DeathCircleUpdate(playerDeathCircleEffect[i], player, playParticle);
		}

		for (int i = 0; i < maxParticleEffects; i++) {
			DeathParticleUpdate(playerDeathParticleEffect[i], player);
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

		/******** サークルエフェクト描画 **********/
		for (int i = 0; i < maxCircleEffect; i++) {
			if (!playerDeathCircleEffect[i].isEnd) {
				Novice::DrawQuad(
					playerDeathCircleEffect[i].position.x - playerDeathCircleEffect[i].size.x,
					playerDeathCircleEffect[i].position.y + playerDeathCircleEffect[i].size.y,

					playerDeathCircleEffect[i].position.x + playerDeathCircleEffect[i].size.x,
					playerDeathCircleEffect[i].position.y + playerDeathCircleEffect[i].size.y,

					playerDeathCircleEffect[i].position.x - playerDeathCircleEffect[i].size.x,
					playerDeathCircleEffect[i].position.y - playerDeathCircleEffect[i].size.y,

					playerDeathCircleEffect[i].position.x + playerDeathCircleEffect[i].size.x,
					playerDeathCircleEffect[i].position.y - playerDeathCircleEffect[i].size.y,

					0, 0,
					32, 32,

					wireCircleTexture,
					WHITE
				);
			}
		}

		/******** パーティクルエフェクト描画 **********/
		for (int i = 0; i < maxParticleEffects; i++) {
			if (!playerDeathParticleEffect[i].isEnd) {
				Novice::DrawQuad(
					playerDeathParticleEffect[i].position.x - playerDeathParticleEffect[i].size.x,
					playerDeathParticleEffect[i].position.y + playerDeathParticleEffect[i].size.y,

					playerDeathParticleEffect[i].position.x + playerDeathParticleEffect[i].size.x,
					playerDeathParticleEffect[i].position.y + playerDeathParticleEffect[i].size.y,

					playerDeathParticleEffect[i].position.x - playerDeathParticleEffect[i].size.x,
					playerDeathParticleEffect[i].position.y - playerDeathParticleEffect[i].size.y,

					playerDeathParticleEffect[i].position.x + playerDeathParticleEffect[i].size.x,
					playerDeathParticleEffect[i].position.y - playerDeathParticleEffect[i].size.y,

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

		Novice::ScreenPrintf(0, 20, "playParticle : %d", playParticle);
		Novice::ScreenPrintf(0, 40, "isEnd : %d", playerDeathParticleEffect[0].isEnd);
		Novice::ScreenPrintf(0, 60, "isEnd : %d", playerDeathParticleEffect[1].isEnd);
		Novice::ScreenPrintf(0, 80, "isEnd : %d", playerDeathParticleEffect[2].isEnd);
		Novice::ScreenPrintf(0, 100, "isEnd : %d", playerDeathParticleEffect[3].isEnd);
		Novice::ScreenPrintf(0, 120, "isEnd : %d", playerDeathParticleEffect[4].isEnd);
		

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
