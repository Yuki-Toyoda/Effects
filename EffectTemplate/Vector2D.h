#pragma once

#ifndef Vector2D_h_
#define Vector2D_h_
#include "Vector2.h"

class Vector2D
{

public:

	float x; // x成分
	float y; // y成分

public:

	//コンストラクタ
	
	Vector2D();
	Vector2D(float x, float y);
	Vector2D(const Vector2D& vector);
	~Vector2D();

	// セット関数
	/// <summary>
	/// ゼロベクトルにする
	/// </summary>
	void setZero();
	/// <summary>
	/// 正規化ベクトルにする
	/// </summary>
	void setNormalize();
	/// <summary>
	/// 大きさを変える
	/// </summary>
	/// <param name="length">大きさ</param>
	void setMagnitude(float length);
	/// <summary>
	/// 回転させる
	/// </summary>
	/// <param name="theta">回転角度</param>
	void setRotate(float theta);

	// const関数 
	/// <summary>
	/// 長さ
	/// </summary>
	/// <returns>float型</returns>
	float Length() const;
	/// <summary>
	/// 長さの二乗
	/// </summary>
	/// <returns>float型</returns>
	float LengthSquare() const;
	/// <summary>
	/// ベクトルとの距離
	/// </summary>
	/// <param name="other">ベクトル</param>
	/// <returns>float型</returns>
	float DistanceFrom(const Vector2D& other) const;
	/// <summary>
	/// 内積
	/// </summary>
	/// <param name="other">ベクトル</param>
	/// <returns>float型</returns>
	float Dot(const Vector2D& other) const;
	/// <summary>
	/// 外積
	/// </summary>
	/// <param name="other">ベクトル</param>
	/// <returns>float型</returns>
	float Cross(const Vector2D& other) const;
	/// <summary>
	/// 正規化
	/// </summary>
	/// <returns>Vector2D型</returns>
	Vector2D Normalized() const;
	/// <summary>
	/// 回転したベクトル
	/// </summary>
	/// <param name="theta">回転角度</param>
	/// <returns>Vector2D型</returns>
	Vector2D Rotated(float theta) const;
	/// <summary>
	/// 大きさを変えたベクトル
	/// </summary>
	/// <param name="length">大きさ</param>
	/// <returns>Vector2D型</returns>
	Vector2D Magnituded(float length) const;
	/// <summary>
	/// 反射ベクトル
	/// </summary>
	/// <param name="normal">正規化した壁の法線ベクトル</param>
	/// <returns>Vector2D型</returns>
	Vector2D Reflected(const Vector2D& normal) const;
	/// <summary>
	/// 壁ずりベクトル
	/// </summary>
	/// <param name="normal">正規化した壁の法線ベクトル</param>
	/// <returns>Vector2D型</returns>
	Vector2D AlongWall(const Vector2D& normal) const;

	// 比較関数
	
	/// <summary>
	/// ゼロベクトルであるか
	/// </summary>
	/// <returns>bool型</returns>
	bool isZero() const;

	// 演算子のオーバーロード

	Vector2D operator+() const;
	Vector2D operator-() const;
	Vector2D operator+(const Vector2D& other) const;
	Vector2D operator-(const Vector2D& other) const;
	Vector2D operator*(float s) const;
	Vector2D operator/(float s) const;
	const Vector2D& operator=(const Vector2D& other);
	const Vector2D& operator+=(const Vector2D& other);
	const Vector2D& operator-=(const Vector2D& other);
	const Vector2D& operator*=(float s);
	const Vector2D& operator/=(float s);
	bool operator==(const Vector2D& other) const;
	bool operator!=(const Vector2D& other) const;

	// 表示
	static void Printf(int y, const Vector2D& m);

};

Vector2D operator*(float s, const Vector2D& vector);

// NoviceのVector2

// 長さ
float Length(const Vector2& vector);
// 距離
float Distance(const Vector2& vector, const Vector2& other);
// 内積
float Dot(const Vector2& vector, const Vector2& other);
// 外積
float Cross(const Vector2& vector, const Vector2& other);
// 正規化
Vector2 Normalized(const Vector2& vector);
// 回転
Vector2 Rotated(const Vector2& vector, float theta);
// 大きさ
Vector2 Magnituded(const Vector2& vector, float length);

// 演算子のオーバーロード
Vector2 operator+(const Vector2& vector1, const Vector2& vector2);
Vector2 operator-(const Vector2& vector1, const Vector2& vector2);
Vector2 operator*(const Vector2& vector, float s);
Vector2 operator*(float s, const Vector2& vector);
Vector2 operator/(const Vector2& vector, float s);



#endif // Vector2D_h_