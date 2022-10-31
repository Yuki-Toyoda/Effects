#include "Vector2D.h"
#include <cmath>
#include <Novice.h>

Vector2D::Vector2D() : x(0), y(0) {
}

Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D::Vector2D(const Vector2D& vector) : Vector2D(vector.x, vector.y) {}

Vector2D::~Vector2D(){}

void Vector2D::setZero(){
	x = 0;
	y = 0;
}

void Vector2D::setNormalize() {
	*this = Normalized();
}

void Vector2D::setMagnitude(float length){
	*this = Magnituded(length);
}

void Vector2D::setRotate(float theta){
	*this = Rotated(theta);
}

float Vector2D::Length() const{
	return sqrtf(LengthSquare());
}

float Vector2D::LengthSquare() const{
	return Dot(*this);
}

float Vector2D::DistanceFrom(const Vector2D& other) const{
	return (other - *this).Length();
}

float Vector2D::Dot(const Vector2D& other) const{
	return x * other.x + y * other.y;
}

float Vector2D::Cross(const Vector2D& other) const{
	return x * other.y - y * other.x;
}

Vector2D Vector2D::Normalized() const{
	return Length() != 0 ? *this / Length() : *this;
}

Vector2D Vector2D::Rotated(float theta) const{
	float s = sinf(theta);
	float c = cosf(theta);
	return{ x * c - y * s ,y * c + x * s };
}

Vector2D Vector2D::Magnituded(float length) const{
	return Normalized() * length;
}

Vector2D Vector2D::Reflected(const Vector2D& normal) const {
	return normal.Dot(-*this) * 2 * normal + *this;
}

Vector2D Vector2D::AlongWall(const Vector2D& normal) const {
	return normal.Dot(-*this) * normal + *this;
}

bool Vector2D::isZero() const{
	return x == 0.0 && y == 0.0;
}

Vector2D Vector2D::operator+() const{
	return *this;
}
Vector2D Vector2D::operator-() const{
	return { -x,-y };
}
Vector2D Vector2D::operator+(const Vector2D& other) const{
	return { x + other.x, y + other.y };
}
Vector2D Vector2D::operator-(const Vector2D& other) const{
	return { x - other.x, y - other.y };
}
Vector2D Vector2D::operator*(float s) const{
	return { x * s, y * s };
}
Vector2D Vector2D::operator/(float s) const{
	return { x * (1.0f / s), y * (1.0f / s) };
}
const Vector2D& Vector2D::operator=(const Vector2D& other) {
	x = other.x;
	y = other.y;
	return *this;
}
const Vector2D& Vector2D::operator+=(const Vector2D& other){
	x += other.x;
	y += other.y;
	return *this;
}
const Vector2D& Vector2D::operator-=(const Vector2D& other){
	x -= other.x;
	y -= other.y;
	return *this;
}
const Vector2D& Vector2D::operator*=(float s){
	x *= s;
	y *= s;
	return *this;
}
const Vector2D& Vector2D::operator/=(float s){
	x /= s;
	y /= s;
	return *this;
}
bool Vector2D::operator==(const Vector2D& other) const{
	return x == other.x && y == other.y;
}
bool Vector2D::operator!=(const Vector2D& other) const{
	return x != other.x && y != other.y;
}

Vector2D operator*(float s, const Vector2D& vector) {
	return { s * vector.x,s * vector.y };
}





float Length(const Vector2& vector) {
	return sqrtf(Dot(vector, vector));
}

float Distance(const Vector2& vector, const Vector2& other) {
	return Length(other - vector);
}

float Dot(const Vector2& vector, const Vector2& other) {
	return vector.x * other.x + vector.y * other.y;
}

float Cross(const Vector2& vector, const Vector2& other) {
	return vector.x * other.y - vector.y * other.x;
}

Vector2 Normalized(const Vector2& vector) {
	return Length(vector) != 0 ? vector / Length(vector) : vector;
}

Vector2 Rotated(const Vector2& vector, float theta) {
	float s = sinf(theta);
	float c = cosf(theta);
	return{ vector.x * c - vector.y * s ,vector.y * c + vector.x * s };
}

Vector2 Magnituded(const Vector2& vector, float length) {
	return Normalized(vector) * length;
}

Vector2 operator+(const Vector2& vector1, const Vector2& vector2){
	return { vector1.x + vector2.x,vector1.y + vector2.y };
}
Vector2 operator-(const Vector2& vector1, const Vector2& vector2){
	return { vector1.x - vector2.x,vector1.y - vector2.y };
}
Vector2 operator*(const Vector2& vector, float s){
	return { vector.x * s,vector.y * s };
}
Vector2 operator*(float s, const Vector2& vector){
	return { s * vector.x,s * vector.y };
}
Vector2 operator/(const Vector2& vector, float s){
	return { vector.x / s,vector.y / s };
}

void Vector2D::Printf(int y, const Vector2D& m) {
	Novice::ScreenPrintf(0, y * 60, "x:%.02f y:%.02f", m.x, m.y);
}