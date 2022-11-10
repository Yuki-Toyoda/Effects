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

	int texture;
};

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

/******** エフェクト更新処理 **********/
void DebrisAnniEffectUpdate(Effect& debrisEffect) {

	if (debrisEffect.init == true) {

		//エフェクトの位置、速度、サイズ初期化
		debrisEffect.position = { 640.0f, 360.0f };
		debrisEffect.size = { My::RandomF(10.0f, 20.0f,0), debrisEffect.size.x };
		debrisEffect.velocity = { My::RandomF(-10.0f, 10.0f,0), My::RandomF(-20.0f, 10.0f,0) };

		debrisEffect.acceleration = 0.98f;

		debrisEffect.theta = My::RandomF(0.0f, 360.0f, 0);

		//イージング用time変数の初期化
		debrisEffect.time = 0.0f;

		//経過フレーム初期化
		debrisEffect.elapseFrame = 0.0f;

		debrisEffect.texture = My::Random(0, 3);

		//透明度の初期化
		debrisEffect.currentAlpha = 0xFF;

		//エフェクト表示
		debrisEffect.isEnd = false;

		//初期化フラグfalse
		debrisEffect.init = false;

	}

	if (debrisEffect.elapseFrame > 100 || debrisEffect.time >= 1.0f) {

		//エフェクト消去
		debrisEffect.isEnd = true;

		//経過フレーム初期化
		debrisEffect.elapseFrame = 0.0f;

	}

	if (debrisEffect.isEnd == false) {

		debrisEffect.position.x += cosf(debrisEffect.theta) * debrisEffect.velocity.x;
		debrisEffect.position.y += debrisEffect.velocity.y;

		debrisEffect.velocity.y += debrisEffect.acceleration;

		debrisEffect.theta += 10.0f;

		//経過フレーム加算
		debrisEffect.elapseFrame += 1.0f;
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

	int debrisTex[4];
	debrisTex[0] = Novice::LoadTexture("./Debris1.png");
	debrisTex[1] = Novice::LoadTexture("./Debris2.png");
	debrisTex[2] = Novice::LoadTexture("./Debris3.png");
	debrisTex[3] = Novice::LoadTexture("./Debris4.png");

	/******** エフェクト関係 **********/
	//表示可能エフェクト数
	const int maxEffects = 10;

	//エフェクト
	Effect debrisEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		debrisEffect[i] = {
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
			true,
			debrisTex[0]
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

		if (keys[DIK_SPACE] && !preKeys[DIK_SPACE]) {
			for (int i = 0; i < maxEffects; i++) {
				debrisEffect[i].theta = My::Random((180 / maxEffects) * (i + 1) - 10, (180 / maxEffects) * (i + 1) + 10);
				debrisEffect[i].theta = debrisEffect[i].theta * (M_PI / 180.0f);
				debrisEffect[i].init = true;
			}
		}

		for (int i = 0; i < maxEffects; i++) {
			DebrisAnniEffectUpdate(debrisEffect[i]);
		}

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/
		/******** エフェクト描画 **********/
		for (int i = 0; i < maxEffects; i++) {
			if (!debrisEffect[i].isEnd) {
				Novice::DrawQuad(
					debrisEffect[i].position.x - debrisEffect[i].size.x,
					debrisEffect[i].position.y + debrisEffect[i].size.y,

					debrisEffect[i].position.x + debrisEffect[i].size.x,
					debrisEffect[i].position.y + debrisEffect[i].size.y,

					debrisEffect[i].position.x - debrisEffect[i].size.x,
					debrisEffect[i].position.y - debrisEffect[i].size.y,

					debrisEffect[i].position.x + debrisEffect[i].size.x,
					debrisEffect[i].position.y - debrisEffect[i].size.y,

					0, 0,
					32, 32,

					debrisTex[debrisEffect[i].texture],
					0xFFFFFFFF00 + debrisEffect[i].currentAlpha
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
