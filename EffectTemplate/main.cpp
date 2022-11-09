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
const char kWindowTitle[] = "弾消滅エフェクト";

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
//startPosition ... 初期座標
//endPosition ... 終端座標
//size ... 矩形のサイズ
//startSize ... 矩形の初期サイズ
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
	float acceleration;
	float theta;
	float elapseFrame;
	float time;
	float easeTime;
	unsigned int currentAlpha;

	bool init;
	bool isEnd;
};

struct Bullet {
	Vector2D position;
	float radius;
};

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

/******** エフェクト更新処理 **********/
void BulletAnniEffectUpdate(Effect& bulletAnniEffect, Bullet& bullet) {

	if (bulletAnniEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		bulletAnniEffect.position = { bullet.position.x, bullet.position.y };
		bulletAnniEffect.size = { My::RandomF(5.0f, 7.0f,0), My::RandomF(5.0f, 7.0f,0) };
		bulletAnniEffect.startSize = { bulletAnniEffect.size.x, bulletAnniEffect.size.y };

		//始端の座標と終端の座標の決定
		bulletAnniEffect.startPosition = { bulletAnniEffect.position.x, bulletAnniEffect.position.y };
		bulletAnniEffect.endPosition = { bullet.position.x + cosf(bulletAnniEffect.theta) * My::RandomF(40, 80,0), bullet.position.y + sinf(bulletAnniEffect.theta) * My::RandomF(40, 80,0) };

		//イージング用time変数の初期化
		bulletAnniEffect.time = 0.0f;

		//透明度の初期化
		bulletAnniEffect.currentAlpha = 0xFF;

		//エフェクト表示
		bulletAnniEffect.isEnd = false;

		//初期化フラグfalse
		bulletAnniEffect.init = false;

	}

	if (bulletAnniEffect.elapseFrame > 100 || bulletAnniEffect.time >= 1.0f) {

		//エフェクト消去
		bulletAnniEffect.isEnd = true;

		//経過フレーム初期化
		bulletAnniEffect.elapseFrame = 0.0f;

	}

	if (bulletAnniEffect.isEnd == false) {

		///粒子エフェクトが指定された位置まで移動したら
		if (bulletAnniEffect.time <= 1.0f) {

			//粒子エフェクトのイージング処理
			bulletAnniEffect.time += 0.03f;
			bulletAnniEffect.easeTime = 1.0f - powf(1.0f - bulletAnniEffect.time, 3.0f);

			//座標
			bulletAnniEffect.position.x = (1.0 - bulletAnniEffect.easeTime) * bulletAnniEffect.startPosition.x + bulletAnniEffect.easeTime * bulletAnniEffect.endPosition.x;
			bulletAnniEffect.position.y = (1.0 - bulletAnniEffect.easeTime) * bulletAnniEffect.startPosition.y + bulletAnniEffect.easeTime * bulletAnniEffect.endPosition.y;

			//透明度
			bulletAnniEffect.currentAlpha = (1.0 - bulletAnniEffect.easeTime) * 0xFF + bulletAnniEffect.easeTime * 0x00;

			//大きさ
			bulletAnniEffect.size.x = (1.0 - bulletAnniEffect.easeTime) * bulletAnniEffect.startSize.x + bulletAnniEffect.easeTime * 0;
			bulletAnniEffect.size.y = (1.0 - bulletAnniEffect.easeTime) * bulletAnniEffect.startSize.y + bulletAnniEffect.easeTime * 0;
		}
		//経過フレーム加算
		bulletAnniEffect.elapseFrame += 1.0f;
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
	const int maxEffects = 6;

	//エフェクト
	Effect bulletAnniEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		bulletAnniEffect[i] = {
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
			0xFF,
			false,
			true
		};
	}

	Bullet bullet = {
		{640.0f, 360.0f},
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

		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			for (int i = 0; i < maxEffects; i++) {
				bulletAnniEffect[i].theta = My::Random((360 / maxEffects) * (i + 1) - 30, (360 / maxEffects) * (i + 1) + 30);
				bulletAnniEffect[i].theta = bulletAnniEffect[i].theta * (M_PI / 180.0f);
				bulletAnniEffect[i].init = true;
			}
		}

		for (int i = 0; i < maxEffects; i++) {
			BulletAnniEffectUpdate(bulletAnniEffect[i], bullet);
		}

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/
		/******** エフェクト描画 **********/
		for (int i = 0; i < maxEffects; i++) {
			if (!bulletAnniEffect[i].isEnd) {
				Novice::DrawQuad(
					bulletAnniEffect[i].position.x - bulletAnniEffect[i].size.x,
					bulletAnniEffect[i].position.y + bulletAnniEffect[i].size.y,

					bulletAnniEffect[i].position.x + bulletAnniEffect[i].size.x,
					bulletAnniEffect[i].position.y + bulletAnniEffect[i].size.y,

					bulletAnniEffect[i].position.x - bulletAnniEffect[i].size.x,
					bulletAnniEffect[i].position.y - bulletAnniEffect[i].size.y,

					bulletAnniEffect[i].position.x + bulletAnniEffect[i].size.x,
					bulletAnniEffect[i].position.y - bulletAnniEffect[i].size.y,

					0, 0,
					1, 1,

					sampleTexture,
					0xFFFFFFFF00 + bulletAnniEffect[i].currentAlpha
				);
			}
		}

		Novice::DrawQuad(
			bullet.position.x - bullet.radius,
			bullet.position.y + bullet.radius,

			bullet.position.x + bullet.radius,
			bullet.position.y + bullet.radius,

			bullet.position.x - bullet.radius,
			bullet.position.y - bullet.radius,

			bullet.position.x + bullet.radius,
			bullet.position.y - bullet.radius,

			0, 0,
			32, 32,

			circleTexture,
			RED
		);

		Novice::ScreenPrintf(0, 10, "isEnd : %d", bulletAnniEffect[0].isEnd);
		Novice::ScreenPrintf(0, 30, "isEnd : %d", bulletAnniEffect[1].isEnd);
		Novice::ScreenPrintf(0, 50, "isEnd : %d", bulletAnniEffect[2].isEnd);
		Novice::ScreenPrintf(0, 70, "isEnd : %d", bulletAnniEffect[3].isEnd);
		Novice::ScreenPrintf(0, 90, "isEnd : %d", bulletAnniEffect[4].isEnd);
		Novice::ScreenPrintf(0, 110, "isEnd : %d", bulletAnniEffect[5].isEnd);

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
