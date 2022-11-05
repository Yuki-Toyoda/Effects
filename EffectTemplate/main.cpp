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
void EffectUpdate(Effect& absorptionEffect, Player& player) {

	if (absorptionEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		absorptionEffect.position = { player.position.x, player.position.y };
		absorptionEffect.size = { 15, 15 };

		//エフェクトが向かう方向をランダムにする
		absorptionEffect.theta = My::Random(0, 360);
		absorptionEffect.theta = absorptionEffect.theta * (M_PI / 180.0f);

		absorptionEffect.position.x += (cosf(absorptionEffect.theta) * 150);
		absorptionEffect.position.y += -(sinf(absorptionEffect.theta) * 150);
		
		absorptionEffect.startPosition.x = absorptionEffect.position.x;
		absorptionEffect.startPosition.y = absorptionEffect.position.y;

		absorptionEffect.currentAlpha = 0xFF;

		absorptionEffect.time = 0.0f;

		//エフェクト表示
		absorptionEffect.isEnd = false;

		//初期化フラグfalse
		absorptionEffect.init = false;

	}

	if (absorptionEffect.elapseFrame >= 100 || absorptionEffect.time >= 1.0f || absorptionEffect.currentAlpha > 0xFF) {

		//エフェクト消去
		absorptionEffect.isEnd = true;

		//経過フレーム初期化
		absorptionEffect.elapseFrame = 0.0f;

		//初期化フラグfalse
		absorptionEffect.init = true;

	}

	if (absorptionEffect.isEnd == false) {

		absorptionEffect.time += 0.01f;
		absorptionEffect.easeTime = 1.0f - powf(1.0f - absorptionEffect.time, 3.0f);
		
		absorptionEffect.position.x = (1.0f - absorptionEffect.easeTime) * absorptionEffect.startPosition.x + absorptionEffect.easeTime * player.position.x;
		absorptionEffect.position.y = (1.0f - absorptionEffect.easeTime) * absorptionEffect.startPosition.y + absorptionEffect.easeTime * player.position.y;

		absorptionEffect.size.x -= 0.25f;
		absorptionEffect.size.y -= 0.25f;

		absorptionEffect.currentAlpha -= 0x04;

		//経過フレーム加算
		absorptionEffect.elapseFrame += 1.0f;

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

	float frame = 0.0f;

	/******** エフェクト関係 **********/
	//表示可能エフェクト数
	const int maxEffects = 15;

	//エフェクト
	Effect absorptionEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		absorptionEffect[i] = {
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

	Player player = {
		{640.0f, 360.0f},
		30.0f,
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

		for (int i = 0; i < maxEffects; i++) {
			if (frame >= 90.0f) {
				if (absorptionEffect[i].isEnd == true) {
					absorptionEffect[i].init = true;
					frame = 0.0f;
				}
			}
			else {
				frame += 1.0f;
			}
		}

		for (int i = 0; i < maxEffects; i++) {
			EffectUpdate(absorptionEffect[i], player);
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
			if (!absorptionEffect[i].isEnd) {
				Novice::DrawQuad(
					absorptionEffect[i].position.x - absorptionEffect[i].size.x,
					absorptionEffect[i].position.y + absorptionEffect[i].size.y,

					absorptionEffect[i].position.x + absorptionEffect[i].size.x,
					absorptionEffect[i].position.y + absorptionEffect[i].size.y,

					absorptionEffect[i].position.x - absorptionEffect[i].size.x,
					absorptionEffect[i].position.y - absorptionEffect[i].size.y,

					absorptionEffect[i].position.x + absorptionEffect[i].size.x,
					absorptionEffect[i].position.y - absorptionEffect[i].size.y,

					0, 0,
					32, 32,

					circleTexture,
					0xFFFFFFFF00 + absorptionEffect[i].currentAlpha
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
