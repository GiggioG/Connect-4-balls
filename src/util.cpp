#include "util.h"

void int2::reset() {
	x = 0;
	y = 0;
}

void int2::set(int2 b) {
	x = b.x;
	y = b.y;
}

void int2::operator+=(int2 a) {
	x += a.x;
	y += a.y;
}

void int2::operator-=(int2 a) {
	x -= a.x;
	y -= a.y;
}

void int2::operator*=(int2 a) {
	x *= a.x;
	y *= a.y;
}

void int2::operator/=(int2 a) {
	x /= a.x;
	y /= a.y;
}

int2 int2::operator+(int2 a) const {
	return { x + a.x, y + a.y };
}

int2 int2::operator-(int2 a) const {
	return { x - a.x, y - a.y };
}

int2 int2::operator*(int2 a) const {
	return { x * a.x, y * a.y };
}

int2 int2::operator/(int2 a) const {
	return { x / a.x, y / a.y };
}

int2 int2::operator*(int a) const {
	return { x * a, y * a };
}

int2 int2::operator/(int a) const {
	return { x / a, y / a };
}

int2 int2::operator-() const {
	return { -x, -y };
}

bool int2::operator==(int2 a) const {
	return (x == a.x && y == a.y);
}

bool int2::operator!=(int2 a) const {
	return (x != a.x || y != a.y);
}

void float2::reset() {
	x = 0;
	y = 0;
}

void float2::set(float2 b) {
	x = b.x;
	y = b.y;
}

void float2::operator+=(float2 a) {
	x += a.x;
	y += a.y;
}

void float2::operator-=(float2 a) {
	x -= a.x;
	y -= a.y;
}

void float2::operator*=(float2 a) {
	x *= a.x;
	y *= a.y;
}

void float2::operator/=(float2 a) {
	x /= a.x;
	y /= a.y;
}

float2 float2::operator+(float2 a) const {
	return { x + a.x, y + a.y };
}

float2 float2::operator-(float2 a) const {
	return { x - a.x, y - a.y };
}

float2 float2::operator*(float2 a) const {
	return { x * a.x, y * a.y };
}

float2 float2::operator/(float2 a) const {
	return { x / a.x, y / a.y };
}

float2 float2::operator*(float a) const {
	return { x * a, y * a };
}

float2 float2::operator/(float a) const {
	return { x / a, y / a };
}

float2 float2::operator-() const {
	return { -x, -y };
}

bool float2::operator==(float2 a) const {
	return (x == a.x && y == a.y);
}

bool float2::operator!=(float2 a) const {
	return (x != a.x || y != a.y);
}

void double2::reset() {
	x = 0;
	y = 0;
}

void double2::set(double2 b) {
	x = b.x;
	y = b.y;
}

void double2::operator+=(double2 a) {
	x += a.x;
	y += a.y;
}

void double2::operator-=(double2 a) {
	x -= a.x;
	y -= a.y;
}

void double2::operator*=(double2 a) {
	x *= a.x;
	y *= a.y;
}

void double2::operator/=(double2 a) {
	x /= a.x;
	y /= a.y;
}

double2 double2::operator+(double2 a) const {
	return { x + a.x, y + a.y };
}

double2 double2::operator-(double2 a) const {
	return { x - a.x, y - a.y };
}

double2 double2::operator*(double2 a) const {
	return { x * a.x, y * a.y };
}

double2 double2::operator/(double2 a) const {
	return { x / a.x, y / a.y };
}

double2 double2::operator*(double a) const {
	return { x * a, y * a };
}

double2 double2::operator/(double a) const {
	return { x / a, y / a };
}

double2 double2::operator-() const {
	return { -x, -y };
}

bool double2::operator==(double2 a) const {
	return (x == a.x && y == a.y);
}

bool double2::operator!=(double2 a) const {
	return (x != a.x || y != a.y);
}

SDL_Rect Rect::toUnrotatedSdlRect() const {
	return {
		(int)(center.x - dims.x / 2),
		(int)(center.y - dims.y / 2),
		(int)(dims.x),
		(int)(dims.y)
	};
}

Rect::Rect() {}

Rect::Rect(double2 _center, double2 _dims) {
	center = _center;
	dims = _dims;
}

RotatedRect::RotatedRect() {}

RotatedRect::RotatedRect(double2 _center, double2 _dims, double _angle, SDL_RendererFlip _flip) {
	center = _center;
	dims = _dims;
	angle = _angle;
	flip = _flip;
}

RotatedRect::RotatedRect(const Rect& r) {
	center = r.center;
	dims = r.dims;
	angle = 0;
	flip = SDL_FLIP_NONE;
}
