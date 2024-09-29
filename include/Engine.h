 #pragma once

#include <cmath>
#include <string>
#include <float.h>
#include <utility>

#include <SDL.h>
#include <SDL_TTF.h>

#include "util.h"

struct Triangle {
    double2 p1, p2, p3;

    double side1, side2, side3, p, S;
    Triangle(double2 a, double2 b, double2 c);
    Triangle();
};

double distance(double2 a, double2 b);
bool pointInTriangle(double2 p, Triangle t);
double angle2Points(double2 o, double2 p);
double angle3Points(double2 a, double2 o, double2 b);
double sign(double a);
bool collLine(double2 p1, double2 p2, double2 p3, double2 p4);
bool collTriangleTriangle(Triangle a, Triangle b);
bool coordInRect(int2 coor, SDL_Rect rect);
bool coordInRect(double2 coor, Rect rect);
bool collRectRect(SDL_Rect a, SDL_Rect b);
bool collTriangleTriangle(Triangle a, Triangle b);
bool collRotatedRectRotatedRect(RotatedRect r1, RotatedRect r2);
bool checkInOffBounds(SDL_Rect rect, int screenWidth, int screenHeight);
int rangeMap(int from_low, int from_high, int to_low, int to_high, int val);
double randToOne();
double2 rotatePointAroundCenter(double2 p, double2 o, double angle);