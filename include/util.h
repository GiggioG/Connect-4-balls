#pragma once

#include <iostream>
#include <fstream>
#include <SDL.h>

#define D(x) cerr << #x << " = " << (x) << " | " << __LINE__ << endl;
#define Dex(x) cerr << #x << " = " << (x) << " | " << __LINE__ << " " << __FILE__ << endl;
#define __FILE_NAME__ (strrchr(__FILE__, '\\' ) ? strrchr(__FILE__, '\\') + 1 : __FILE__)

using namespace std;

static string IMG_FOLDER = "img\\";
static string CONFIG_FOLDER = "config\\";
static string SOUND_FOLDER = "sound\\";

typedef Uint32 Time;

struct int2 {
	int x = 0, y = 0;

	void reset();
	void set(int2 b);
	void operator+=(int2 a);
	void operator-=(int2 a);
	void operator*=(int2 a);
	void operator/=(int2 a);
	int2 operator+(int2 a) const;
	int2 operator-(int2 a) const;
	int2 operator*(int2 a) const;
	int2 operator/(int2 a) const;
	int2 operator*(int a) const;
	int2 operator/(int a) const;
	int2 operator-() const;
	bool operator==(int2 a) const;
	bool operator!=(int2 a) const;
};

struct float2 {
	float x = 0, y = 0;

	void reset();
	void set(float2 b);
	void operator+=(float2 a);
	void operator-=(float2 a);
	void operator*=(float2 a);
	void operator/=(float2 a);
	float2 operator+(float2 a) const;
	float2 operator-(float2 a) const;
	float2 operator*(float2 a) const;
	float2 operator/(float2 a) const;
	float2 operator*(float a) const;
	float2 operator/(float a) const;
	float2 operator-() const;
	bool operator==(float2 a) const;
	bool operator!=(float2 a) const;
};

struct double2 {
	double x = 0, y = 0;

	void reset();
	void set(double2 b);
	void operator+=(double2 a);
	void operator-=(double2 a);
	void operator*=(double2 a);
	void operator/=(double2 a);
	double2 operator+(double2 a) const;
	double2 operator-(double2 a) const;
	double2 operator*(double2 a) const;
	double2 operator/(double2 a) const;
	double2 operator*(double a) const;
	double2 operator/(double a) const;
	double2 operator-() const;
	bool operator==(double2 a) const;
	bool operator!=(double2 a) const;
};

struct Rect {
	double2 center = { 0, 0 };
	double2 dims = { 0, 0 };
	SDL_Rect toUnrotatedSdlRect() const;
	Rect();
	Rect(double2 _center, double2 _dims);
};

struct RotatedRect : Rect {
	double angle = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	RotatedRect();
	RotatedRect(double2 _center, double2 _dims, double _angle, SDL_RendererFlip _flip);
	RotatedRect(const Rect& r);
};

enum SOUND {
	BACKGORUND_MUSIC
};