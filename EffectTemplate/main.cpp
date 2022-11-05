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
const char kWindowTitle[] = "破片エフェクト";

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
//isEnd ... エフェクトが終了しているか
struct Effect {
	Vector2D position;
	Vector2D size;
	Vector2D velocity;
	float acceleration;
	float theta;
	int elapseFrame;
	bool init;
	bool isEnd;
};

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

/******** エフェクト更新処理 **********/
void EffectUpdate(Effect& debrisEffect) {

	if (debrisEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		debrisEffect.position = { 640.0f, 360.0f };
		debrisEffect.velocity = { My::RandomF(5.0f, 7.0f, 1), My::RandomF(5.0f, 7.0f, 1) };
		debrisEffect.size = { 5, 5 };

		//エフェクトが向かう方向をランダムにする
		debrisEffect.theta = My::Random(0, 180);
		debrisEffect.theta = debrisEffect.theta * (M_PI / 180.0f);

		//エフェクト表示
		debrisEffect.isEnd = false;

		//初期化フラグfalse
		debrisEffect.init = false;

	}

	if (debrisEffect.position.y >= kWindowHeight || debrisEffect.elapseFrame >= 100) {

		debrisEffect.isEnd = true;

		debrisEffect.elapseFrame = 0;

	}

	if (debrisEffect.isEnd == false) {



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
	Effect effect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		effect[i] = {
			{640.0f, 360.0f},
			{1.0f, 1.0f},
			{1.0f, 1.0f},
			0.8f,
			0.0f,
			0,
			false,
			true
		};
	}
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



		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/
		/******** エフェクト描画 **********/
		for (int i = 0; i < maxEffects; i++) {
			if (!effect[i].isEnd) {
				Novice::DrawQuad(
					effect[i].position.x - effect[i].size.x,
					effect[i].position.y + effect[i].size.y,

					effect[i].position.x + effect[i].size.x,
					effect[i].position.y + effect[i].size.y,

					effect[i].position.x - effect[i].size.x,
					effect[i].position.y - effect[i].size.y,

					effect[i].position.x + effect[i].size.x,
					effect[i].position.y - effect[i].size.y,

					0, 0,
					1, 1,

					circleTexture,
					WHITE
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
