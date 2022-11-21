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
const char kWindowTitle[] = "エフェクト";

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
	Vector2D endPosition;
	Vector2D size;
	Vector2D startSize;
	Vector2D velocity;
	float ofset;
	float startStrength;
	float strength;
	float amplitude;
	float acceleration;
	int degree;
	float theta;
	float nextFrame;
	float elapseFrame;
	float time;
	float easeTime;
	unsigned int currentAlpha;

	bool init;
	bool isEnd;
};

struct Object {
	Vector2D position;
	Vector2D radius;
	Vector2D startRadius;
	float offset;
	float velocity;
	float acceleration;
};

//表示可能エフェクト数
const int maxEffects = 50;

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

/******** エフェクト更新処理 **********/
void EffectUpdate(Effect& generateEffect, Object& object, bool& next, float& addTime) {
	if (generateEffect.init == true) {

		generateEffect.nextFrame = 1.0f;

		//位置等を初期化
		generateEffect.position = { My::RandomF(object.position.x - object.radius.x - 100, object.position.x + object.radius.x + 100, 1), My::RandomF(object.position.y - object.radius.y + object.offset + 100, object.position.y - object.radius.y + object.offset - 100, 1) };
		generateEffect.startPosition = { generateEffect.position.x, generateEffect.position.y };
		generateEffect.endPosition = { object.position.x, object.position.y - object.radius.y + object.offset };

		generateEffect.size = { My::RandomF(10.0f, 12.5f, 0), generateEffect.size.x };
		generateEffect.startSize = { generateEffect.size.x, generateEffect.size.x };

		generateEffect.strength = My::RandomF(40.0f, 50.0f, 0);
		generateEffect.startStrength = generateEffect.strength;
		generateEffect.amplitude = 1.0f;


		generateEffect.time = 0.0f;

		next = false;

		//エフェクト表示
		generateEffect.isEnd = false;

		//effectQuantity += 1;

		//初期化フラグfalse
		generateEffect.init = false;

	}

	if (generateEffect.time == 1.0f) {

		//エフェクト消去
		generateEffect.isEnd = true;

		//経過フレーム初期化
		generateEffect.elapseFrame = 0.0f;

	}

	if (generateEffect.elapseFrame == generateEffect.nextFrame && object.offset > 0.0f) {

		next = true;

	}

	if (generateEffect.isEnd == false) {

		generateEffect.endPosition = { object.position.x, (object.position.y - object.radius.y) + object.offset };

		if (generateEffect.time < 1.0f) {
			//粒子エフェクトのイージング処理
			generateEffect.time += addTime;
			generateEffect.easeTime = 1.0f - powf(1.0f - generateEffect.time, 3.0f);

			//粒子エフェクトのサイズ変更
			generateEffect.size.x = (1.1f - generateEffect.easeTime) * generateEffect.startSize.x + generateEffect.easeTime * 0;
			generateEffect.size.y = (1.1f - generateEffect.easeTime) * generateEffect.startSize.y + generateEffect.easeTime * 0;
			\
				//粒子エフェクトを徐々に上に
				generateEffect.easeTime = generateEffect.time * generateEffect.time;

			generateEffect.currentAlpha = (1.0 - generateEffect.easeTime) * 0xFF + generateEffect.easeTime * 0x00;
			generateEffect.position.x = (1.0 - generateEffect.easeTime) * generateEffect.startPosition.x + generateEffect.easeTime * generateEffect.endPosition.x;
			generateEffect.position.y = (1.0 - generateEffect.easeTime) * generateEffect.startPosition.y + generateEffect.easeTime * generateEffect.endPosition.y;
		}
		else {
			generateEffect.time = 1.0f;
		}

		if (addTime < 0.02f) {
			addTime += 0.0001f / maxEffects;
		}
		else {
			addTime = 0.02f;
		}

		//経過フレーム加算
		generateEffect.elapseFrame += 1.0f;

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

	/******** エフェクト関係 **********/

	float addTime = 0.01f;
	bool next = false;

	int mousePosX = 0;
	int mousePosY = 0;

	//エフェクト
	Effect generateEffect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		generateEffect[i] = {
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{1.0f, 1.0f},
			{1.0f, 1.0f},
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0,
			0.0f,
			1.0f,
			0.0f,
			0.0f,
			0.0f,
			0xFF,
			false,
			true
		};
	}

	Object object{
		{640.0f, 360.0f},
		{5.0f, 50.0f}
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

		Novice::GetMousePosition(&mousePosX, &mousePosY);

		object.position = { (float)mousePosX, (float)mousePosY };

		if (Novice::IsTriggerMouse(0)) {
			next = true;
			addTime = 0.01f;
			object.offset = object.radius.y * 2.0f;
		}

		if (object.offset > 0) {
			object.offset -= 0.5f;
		}
		else {
			object.offset = 0;
		}

		if (next == true) {
			for (int i = 0; i < maxEffects; i++) {
				if (next == true && generateEffect[i].isEnd == true) {
					next = false;
					generateEffect[i].init = true;
				}
			}
		}

		for (int i = 0; i < maxEffects; i++) {
			EffectUpdate(generateEffect[i], object, next, addTime);
		}

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/

		Novice::DrawQuad(
			object.position.x - object.radius.x,
			object.position.y - object.radius.y + object.offset,

			object.position.x + object.radius.x,
			object.position.y - object.radius.y + object.offset,

			object.position.x - object.radius.x,
			object.position.y + object.radius.y,

			object.position.x + object.radius.x,
			object.position.y + object.radius.y,

			1, 1,
			1, 1,

			sampleTexture,
			RED
		);

		/******** エフェクト描画 **********/
		for (int i = 0; i < maxEffects; i++) {
			if (!generateEffect[i].isEnd) {
				Novice::DrawQuad(
					generateEffect[i].position.x - generateEffect[i].size.x + generateEffect[i].ofset,
					generateEffect[i].position.y + generateEffect[i].size.y,

					generateEffect[i].position.x + generateEffect[i].size.x + generateEffect[i].ofset,
					generateEffect[i].position.y + generateEffect[i].size.y,

					generateEffect[i].position.x - generateEffect[i].size.x + generateEffect[i].ofset,
					generateEffect[i].position.y - generateEffect[i].size.y,

					generateEffect[i].position.x + generateEffect[i].size.x + generateEffect[i].ofset,
					generateEffect[i].position.y - generateEffect[i].size.y,

					0, 0,
					32, 32,

					circleTexture,
					0xFFFFFFFF00 + generateEffect[i].currentAlpha
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
