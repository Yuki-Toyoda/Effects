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
	float PlusTime;
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

/******** サークルエフェクト更新処理 **********/
void HealCircleEffectUpdate(HealEffect& healCircleEffect, Player& player) {

	/******** 初期化 **********/
	if (healCircleEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		healCircleEffect.position = { player.position.x, player.position.y };
		healCircleEffect.velocity = { My::RandomF(5.0f, 7.0f, 1), My::RandomF(5.0f, 7.0f, 1) };
		healCircleEffect.size = { 1, 1 };

		healCircleEffect.time = 0.0f;
		healCircleEffect.PlusTime = 0.06f;

		healCircleEffect.currentAlpha = 0xFF;

		//エフェクト表示
		healCircleEffect.isEnd = false;

		//初期化フラグfalse
		healCircleEffect.init = false;

	}

	/******** 終了処理 **********/
	if (healCircleEffect.elapseFrame >= 100 || healCircleEffect.time > 0.55f) {

		//エフェクトを徐々に透明に
		healCircleEffect.currentAlpha -= 0x0A;

		//透明度が0になったら
		if (healCircleEffect.currentAlpha > 0xFF) {
			//エフェクト消去
			healCircleEffect.isEnd = true;
			//経過フレーム初期化
			healCircleEffect.elapseFrame = 0.0f;
		}

	}

	/******** 更新処理 **********/
	if (healCircleEffect.isEnd == false) {

		//エフェクト位置をプレイヤーに固定
		healCircleEffect.position = { player.position.x, player.position.y };

		//円のイージング処理
		healCircleEffect.time += healCircleEffect.PlusTime;
		if (healCircleEffect.PlusTime >= 0.0f) {
			healCircleEffect.PlusTime -= 0.001f;
		}
		else {
			healCircleEffect.PlusTime = 0.001f;
		}
		healCircleEffect.easeTime = 1.0f - powf(1.0f - healCircleEffect.time, 3.0f);

		//円のサイズを変える
		healCircleEffect.size.x = (1.0 - healCircleEffect.easeTime) * 1 + healCircleEffect.easeTime * 75;
		healCircleEffect.size.y = (1.0 - healCircleEffect.easeTime) * 1 + healCircleEffect.easeTime * 75;

		//経過フレーム加算
		healCircleEffect.elapseFrame += 1.0f;

	}

}

/******** 粒子エフェクト更新処理 **********/
void HealParticleEffectUpdate(HealEffect& healParticleEffect, Player& player) {

	/******** 初期化 **********/
	if (healParticleEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		healParticleEffect.position = { My::RandomF(player.position.x - 50.0f, player.position.x + 50.0f, 1), My::RandomF(player.position.y - 35.0f, player.position.y + 45.0f, 1) };
		healParticleEffect.startPosition = { My::RandomF(15.0f, 25.0f, 1), healParticleEffect.position.y };
		healParticleEffect.velocity = { My::RandomF(5.0f, 7.0f, 1), My::RandomF(5.0f, 7.0f, 1) };
		healParticleEffect.size = { 1, 1 };

		healParticleEffect.time = My::RandomF(0.0f, 0.1f, 1);
		healParticleEffect.PlusTime = 0.03f;

		healParticleEffect.currentAlpha = 0xFF;

		//エフェクト表示
		healParticleEffect.isEnd = false;

		//初期化フラグfalse
		healParticleEffect.init = false;

	}

	/******** 終了処理 **********/
	if (healParticleEffect.elapseFrame >= 100 || healParticleEffect.time > 0.25f) {

		//エフェクトを徐々に透明に
		healParticleEffect.currentAlpha -= 0x06;

		//透明度が0になったら
		if (healParticleEffect.currentAlpha > 0xFF) {
			//エフェクト消去
			healParticleEffect.isEnd = true;
			//経過フレーム初期化
			healParticleEffect.elapseFrame = 0.0f;
		}

	}

	/******** 更新処理 **********/
	if (healParticleEffect.isEnd == false) {

		//粒子エフェクトのイージング処理
		healParticleEffect.time += healParticleEffect.PlusTime;
		if (healParticleEffect.PlusTime >= 0.0f) {
			healParticleEffect.PlusTime -= 0.00001f;
		}
		else {
			healParticleEffect.PlusTime = 0.001f;
		}
		healParticleEffect.easeTime = 1.0f - powf(1.0f - healParticleEffect.time, 3.0f);

		//粒子エフェクトのサイズ変更
		healParticleEffect.size.x = (1.0 - healParticleEffect.easeTime) * 0.1f + healParticleEffect.easeTime * 5;
		healParticleEffect.size.y = (1.0 - healParticleEffect.easeTime) * 0.1f + healParticleEffect.easeTime * 5;

		//粒子エフェクトを徐々に上に
		healParticleEffect.easeTime = healParticleEffect.time * healParticleEffect.time;
		healParticleEffect.position.y = (1.0 - healParticleEffect.easeTime) * healParticleEffect.startPosition.y + healParticleEffect.easeTime * (healParticleEffect.startPosition.y - healParticleEffect.startPosition.x);

		//経過フレーム加算
		healParticleEffect.elapseFrame += 1.0f;

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
	const int maxCircleEffects = 1; //サークル
	const int maxParticleEffects = 10; //粒子

	//サークルエフェクト
	HealEffect healCircleEffect[maxCircleEffects];
	for (int i = 0; i < maxCircleEffects; i++) {
		healCircleEffect[i] = {
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{1.0f, 1.0f},
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

	//粒子エフェクト
	HealEffect healParticleEffect[maxParticleEffects];
	for (int i = 0; i < maxParticleEffects; i++) {
		healParticleEffect[i] = {
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{1.0f, 1.0f},
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
			for (int i = 0; i < maxCircleEffects; i++) {
				healCircleEffect[i].init = true;
			}
			for (int i = 0; i < maxParticleEffects; i++) {
				healParticleEffect[i].init = true;
			}
		}

		for (int i = 0; i < maxCircleEffects; i++) {
			HealCircleEffectUpdate(healCircleEffect[i], player);
		}
		for (int i = 0; i < maxParticleEffects; i++) {
			HealParticleEffectUpdate(healParticleEffect[i], player);
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

		/******** エフェクト描画 **********/
		for (int i = 0; i < maxCircleEffects; i++) {
			if (!healCircleEffect[i].isEnd) {
				Novice::DrawQuad(
					healCircleEffect[i].position.x - healCircleEffect[i].size.x,
					healCircleEffect[i].position.y + healCircleEffect[i].size.y,

					healCircleEffect[i].position.x + healCircleEffect[i].size.x,
					healCircleEffect[i].position.y + healCircleEffect[i].size.y,

					healCircleEffect[i].position.x - healCircleEffect[i].size.x,
					healCircleEffect[i].position.y - healCircleEffect[i].size.y,

					healCircleEffect[i].position.x + healCircleEffect[i].size.x,
					healCircleEffect[i].position.y - healCircleEffect[i].size.y,

					0, 0,
					32, 32,

					wireCircleTexture,
					0xFFFFFFFF00 + healCircleEffect[i].currentAlpha
				);
			}
		}

		/******** エフェクト描画 **********/
		for (int i = 0; i < maxParticleEffects; i++) {
			if (!healParticleEffect[i].isEnd) {
				Novice::DrawQuad(
					healParticleEffect[i].position.x - healParticleEffect[i].size.x,
					healParticleEffect[i].position.y + healParticleEffect[i].size.y,

					healParticleEffect[i].position.x + healParticleEffect[i].size.x,
					healParticleEffect[i].position.y + healParticleEffect[i].size.y,

					healParticleEffect[i].position.x - healParticleEffect[i].size.x,
					healParticleEffect[i].position.y - healParticleEffect[i].size.y,

					healParticleEffect[i].position.x + healParticleEffect[i].size.x,
					healParticleEffect[i].position.y - healParticleEffect[i].size.y,

					0, 0,
					32, 32,

					circleTexture,
					0xFFFFFFFF00 + healParticleEffect[i].currentAlpha
				);
			}
		}

		Novice::ScreenPrintf(0, 10, "x : %4.2f y : %4.2f", healParticleEffect[0].position.x, healParticleEffect[0].position.y);
		Novice::ScreenPrintf(0, 30, "x : %4.2f y : %4.2f", healParticleEffect[1].position.x, healParticleEffect[1].position.y);
		Novice::ScreenPrintf(0, 50, "x : %4.2f y : %4.2f", healParticleEffect[2].position.x, healParticleEffect[2].position.y);
		Novice::ScreenPrintf(0, 70, "x : %4.2f y : %4.2f", healParticleEffect[3].position.x, healParticleEffect[3].position.y);

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
