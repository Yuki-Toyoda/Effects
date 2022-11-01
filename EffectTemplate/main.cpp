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
const char kWindowTitle[] = "EffectTemplate";

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
	bool start;
	bool isEnd;
};

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

/******** 衝突エフェクトの更新処理 **********/
void HitEffectUpdate(Effect& hitEffect) {

	if (hitEffect.start == true) {

		//エフェクトの位置、速度、サイズ初期化
		hitEffect.position = { 640.0f, 360.0f };
		hitEffect.velocity = { My::RandomF(5.0f, 7.0f, 1), My::RandomF(5.0f, 7.0f, 1) };
		hitEffect.size = { 16, 16 };

		//エフェクトが向かう方向をランダムにする
		hitEffect.theta = My::Random(0, 360);
		hitEffect.theta = hitEffect.theta * (M_PI / 180.0f);

		//エフェクト表示
		hitEffect.isEnd = false;

		//スタートフラグfalse
		hitEffect.start = false;

	}

	//エフェクトのサイズx,yのどちらかが0になる、または経過フレームが100以上になったら
	if (hitEffect.size.x <= 0 || hitEffect.size.y <= 0 || hitEffect.elapseFrame >= 100) {

		//エフェクト終了
		hitEffect.isEnd = true;

		//経過フレームを0に
		hitEffect.elapseFrame = 0;

	}

	if (hitEffect.isEnd == false) {

		hitEffect.position.x += (cosf(hitEffect.theta) * hitEffect.velocity.x);
		hitEffect.position.y += -(sinf(hitEffect.theta) * hitEffect.velocity.y);

		hitEffect.velocity.x += hitEffect.acceleration;
		hitEffect.velocity.y += hitEffect.acceleration;

		hitEffect.size -= {1.0f, 1.0f };

		hitEffect.elapseFrame++;

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

	//乱数
	My::SetSrand();

	//矩形用テクスチャ読み込み
	int sampleTexture = Novice::LoadTexture("white1x1.png");

	Vector2D MousePos;

	/******** エフェクト関係 **********/
	//表示可能エフェクト数
	const int maxEffects = 30;

	//命中エフェクト
	Effect hitEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		hitEffect[i] = {
			{640.0f, 360.0f},
			{16.0f, 16.0f},
			{1.0f, 1.0f},
			0.15f,
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

		/******** スペースキーを押したらエフェクト初期化 **********/
		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {

			for (int i = 0; i < maxEffects; i++) {
				hitEffect[i].start = true;
			}

		}

		for (int i = 0; i < maxEffects; i++) {
			HitEffectUpdate(hitEffect[i]);
		}

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/

		/******** エフェクト描画 **********/
		for (int i = 0; i < maxEffects; i++) {
			if (!hitEffect[i].isEnd) {
				Novice::DrawQuad(
					hitEffect[i].position.x - hitEffect[i].size.x,
					hitEffect[i].position.y + hitEffect[i].size.y,

					hitEffect[i].position.x + hitEffect[i].size.x,
					hitEffect[i].position.y + hitEffect[i].size.y,

					hitEffect[i].position.x - hitEffect[i].size.x,
					hitEffect[i].position.y - hitEffect[i].size.y,

					hitEffect[i].position.x + hitEffect[i].size.x,
					hitEffect[i].position.y - hitEffect[i].size.y,

					0, 0,
					1, 1,

					sampleTexture,
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