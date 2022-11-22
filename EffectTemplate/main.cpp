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
const char kWindowTitle[] = "埃エフェクト";

/******** ウィンドウサイズの指定 **********/
const int kWindowWidth = 1280; //x
const int kWindowHeight = 720; //y

/*********************************
	定数の宣言ここまで
*********************************/

/*********************************
	構造体宣言ここから
*********************************/

struct Quad {

	Vector2D q1;
	Vector2D q2;
	Vector2D q3;
	Vector2D q4;

};

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
	Quad rotate;
	Quad point;
	Vector2D position;
	Vector2D startPosition;
	Vector2D endPosition;
	Vector2D size;
	Vector2D startSize;
	Vector2D velocity;
	float startStrength;
	float strength;
	float amplitude;
	float acceleration;
	float theta;
	float nextFrame;
	float elapseFrame;
	float time;
	float colorTime;
	float easeTime;
	unsigned int color;

	bool fade;
	bool init;
	bool isEnd;
};

struct Object {
	Vector2D position;
	Vector2D radius;
	Vector2D startRadius;
	float velocity;
	float acceleration;
};

//表示可能エフェクト数
const int maxEffects = 30;

/*********************************
	構造体宣言ここまで
*********************************/

/*********************************
	関数宣言ここから
*********************************/

//clamp関数
	//返り値 ... float型の数値
	//number ... 範囲を制限したい数値
	//min ... 最小値
	//max ... 最大値
float clamp(float number, float min, float max) {
	if (number > max) {
		return number = max;
	}
	else if (number < min) {
		return number = min;
	}
	else {
		return number;
	}
}

//イーズイン関数(int型)
	//int型のイージング結果を返すイーズイン関数
	//返り値 ... int型のイージング後の数値
	//start ... int型の開始値
	//end ... int型の終了値
int IntEaseIn(float t, long int start, long int end) {
	float easeT = t * t;
	return (1.0f - easeT) * start + easeT * end;
}

//イーズアウト関数(int型)
	//int型のイージング結果を返すイーズアウト関数
	//返り値 ... int型のイージング後の数値
	//start ... int型の開始値
	//end ... int型の終了値
int IntEaseOut(float t, long int start, long int end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (1.0f - easeT) * start + easeT * end;
}

//イーズインアウト関数(int型)
	//int型のイージング結果を返すイーズインアウト関数
	//返り値 ... int型のイージング後の数値
	//start ... int型の開始値
	//end ... int型の終了値
int IntEaseInOut(float t, long int start, long int end) {
	float easeT = (-cosf(M_PI * t) - 1.0f) / 2.0f;
	return (1.0f - easeT) * start + easeT * end;
}

//イーズイン関数(float型)
	//float型のイージング結果を返すイーズイン関数
	//返り値 ... float型のイージング後の数値
	//start ... float型の開始値
	//end ... float型の終了値
float easeIn(float t, float start, float end) {
	float easeT = t * t;
	return (1.0f - easeT) * start + easeT * end;
}

//イーズアウト関数(float型)
	//float型のイージング結果を返すイーズアウト関数
	//返り値 ... float型のイージング後の数値
	//start ... float型の開始値
	//end ... float型の終了値
float easeOut(float t, float start, float end) {
	float easeT = 1.0f - powf(1.0f - t, 3.0f);
	return (1.0f - easeT) * start + easeT * end;
}

//イーズインアウト関数(float型)
	//float型のイージング結果を返すイーズインアウト関数
	//返り値 ... float型のイージング後の数値
	//start ... float型の開始値
	//end ... float型の終了値
float easeInOut(float t, float start, float end) {
	float easeT = (-cosf(M_PI * t) - 1.0f) / 2.0f;
	return (1.0f - easeT) * start + easeT * end;
}

//カラーイージング関数(unsigned int型)
	//unsigned int型のイージング結果を返すイーズアウト関数
	//色を徐々に変化させる。現在はイーズアウトのみ対応
	//返り値 ... unsigned int型のイージング後の数値
	//start ... unsigned int型の開始色
	//end ... unsigned int型の終了色
unsigned int ColorEasing(float t, unsigned int startColor, unsigned int endColor) {
	unsigned int red = IntEaseOut(t, (((startColor & 0xFF000000) >> 24) & 0xFF), (((endColor & 0xFF000000) >> 24) & 0xFF));
	unsigned int green = IntEaseOut(t, (((startColor & 0x00FF0000) >> 16) & 0xFF), (((endColor & 0x00FF0000) >> 16) & 0xFF));
	unsigned int blue = IntEaseOut(t, (((startColor & 0x0000FF00) >> 8) & 0xFF), (((endColor & 0x0000FF00) >> 8) & 0xFF));
	unsigned int alpha = IntEaseOut(t, (((startColor & 0x000000FF)) & 0xFF), (((endColor & 0x000000FF)) & 0xFF));

	return (red << 24) + (green << 16) + (blue << 8) + alpha;
}

/******** エフェクト更新処理 **********/
void EffectUpdate(Effect& effect, Object& object, bool& next, int& effectQuantity) {
	if (effect.init == true) {
		
		effect.nextFrame = 10;

		effect.point = {
			{ -effect.size.x, -effect.size.y},
			{effect.size.x, -effect.size.y},
			{-effect.size.x, effect.size.y},
			{effect.size.x, effect.size.y}
		};

		//位置等を初期化
		effect.position = { My::RandomF(50.0f, kWindowWidth - 50.0f, 1), My::RandomF(50.0f, kWindowHeight - 50.0f, 1) };
		effect.startPosition = { effect.position.x, effect.position.y };
		effect.endPosition = { effect.position.x - My::RandomF(200, 300, 1), effect.position.y};

		effect.size = { My::RandomF(8.5f, 10.5f, 0), effect.size.x };
		effect.startSize = { effect.size.x, effect.size.x };

		effect.strength = My::RandomF(60.0f, 90.0f, 0);
		effect.startStrength = effect.strength;
		effect.amplitude = 0.05f;

		effect.time = 0.0f;

		next = false;

		effect.colorTime = 0.0f;
		effect.fade = false;

		//エフェクト表示
		effect.isEnd = false;

		//初期化フラグfalse
		effect.init = false;

	}

	if (effect.time >= 1.0f) {

		//エフェクト消去
		effect.isEnd = true;

		//経過フレーム初期化
		effect.elapseFrame = 0.0f;

	}

	if (effect.elapseFrame == effect.nextFrame) {

		next = true;

	}

	if (effect.isEnd == false) {

		//粒子エフェクトのイージング処理
		effect.time += 0.005f;
		if (effect.time < 1.0f) {
			effect.position.x = easeInOut(effect.time, effect.startPosition.x, effect.endPosition.x);
		}
		else {
			effect.time = 1.0f;
		}

		if (effect.colorTime < 1.0f && effect.fade == false) {
			effect.colorTime += 0.01f;
			effect.color = ColorEasing(effect.colorTime, 0x9effce00, 0xb7ffff55);
		}
		else if(effect.colorTime < 1.0f && effect.fade == true) {
			effect.colorTime += 0.01f;
			effect.color = ColorEasing(effect.colorTime, 0xb7ffff55, 0x9effce00);
		}
		else if(effect.colorTime > 1.0f && effect.fade == false) {
			effect.fade = true;
			effect.colorTime = 0.0f;
		}
		else {
			effect.colorTime = 1.0f;
		}

		effect.theta += M_PI / effect.strength;
		effect.position.y += sinf(effect.theta);

		//経過フレーム加算
		effect.elapseFrame += 1.0f;

	}

}

/*********************************
	関数宣言ここまで
*********************************/

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

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

	int effectQuantity = 0;
	bool next = false;

	int mousePosX = 0;
	int mousePosY = 0;

	//エフェクト
	Effect effect[maxEffects];
	for (int i = 0; i < maxEffects; i++) {
		effect[i] = {
			{{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f}},
			{{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f},
			{0.0f, 0.0f}},
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
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0.0f,
			0xFFFFFFFF,
			false,
			false,
			true
		};
	}

	Object object{
		{640.0f, 360.0f},
		{10.0f, 10.0f}
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
		}


		if (next == true) {
			for (int i = 0; i < maxEffects; i++) {
				if (next == true && effect[i].isEnd == true) {
					effect[i].init = true;
					next = false;
				}
			}
		}

		if (object.radius.x > 0) {
			object.radius.x -= object.velocity;
			object.radius.y -= object.velocity;
			object.velocity += object.acceleration;
		}
		else {
			object.radius.x = 0;
		}

		for (int i = 0; i < maxEffects; i++) {
			EffectUpdate(effect[i], object, next, effectQuantity);
		}

		Novice::ScreenPrintf(0, 10, "Quantity : %d", effectQuantity);

		/*********************************
			更新処理ここまで
		*********************************/

		/*********************************
			描画処理ここから
		*********************************/

		Novice::DrawQuad(
			0, 0,
			kWindowWidth, 0,
			0, kWindowHeight,
			kWindowWidth, kWindowHeight,
			0, 0,
			1, 1,
			sampleTexture,
			BLACK
		);

		Novice::DrawQuad(
			object.position.x - object.radius.x / 2,
			object.position.y - object.radius.y / 2,

			object.position.x + object.radius.x / 2,
			object.position.y - object.radius.y / 2,

			object.position.x - object.radius.x / 2,
			object.position.y + object.radius.y / 2,

			object.position.x + object.radius.x / 2,
			object.position.y + object.radius.y / 2,

			0, 0,
			1, 1,

			sampleTexture,
			RED
		);

		Novice::SetBlendMode(kBlendModeAdd);

		/******** エフェクト描画 **********/
		for (int i = 0; i < maxEffects; i++) {
			if (!effect[i].isEnd) {
				Novice::DrawQuad(
					effect[i].rotate.q1.x,
					effect[i].rotate.q1.y,

					effect[i].rotate.q2.x,
					effect[i].rotate.q2.y,

					effect[i].rotate.q3.x,
					effect[i].rotate.q3.y,

					effect[i].rotate.q4.x,
					effect[i].rotate.q4.y,

					0, 0,
					32, 32,

					circleTexture,
					effect[i].color
				);
			}
		}

		Novice::SetBlendMode(kBlendModeNormal);

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
