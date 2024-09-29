#include "Engine.h"

#include "ConfigManager.h"

bool coordInRect(int2 coor, SDL_Rect rect)
{
    if (coor.x >= rect.x && coor.x <= rect.x + rect.w - 1 && coor.y >= rect.y && coor.y <= rect.y + rect.h - 1) {
        return true;
    }

    return false;
}
bool coordInRect(double2 coor, SDL_Rect rect)
{
    if (coor.x >= rect.x && coor.x <= rect.x + rect.w && coor.y >= rect.y && coor.y <= rect.y + rect.h) {
        return true;
    }

    return false;
}
double randToOne() {
	uint32_t rnd = rand();
	return (double)rnd / (double)RAND_MAX;
}


bool collRectRect(SDL_Rect a, SDL_Rect b)
{
    if (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y)
        return true;

    return false;
}
double distance(double2 a, double2 b){
    return sqrt((a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y));
}
bool pointInTriangle(double2 p, Triangle t){
    Triangle a(p, t.p2, t.p3);
    Triangle b(t.p1, p, t.p3);
    Triangle c(t.p1, t.p2, p);
    if(abs(a.S + b.S + c.S - t.S) <= 0.01){
        return true;
    }
    return false;
}
double angle2Points(double2 o, double2 p){
    return atan2((p.y-o.y), (p.x-o.x));// acos((p.x-o.x)/distance(o, p));
}
double fixAngle(double angle){
    return (angle>M_PI?angle-2*M_PI:(angle<-M_PI?angle+2*M_PI:angle));
}
double angle3Points(double2 a, double2 o, double2 b){
    return fixAngle(angle2Points(o, b) - angle2Points(o, a));
}
double sign(double a){
    return a/abs(a);
}
bool collLine(double2 p1, double2 p2, double2 p3, double2 p4){
    if(sign(angle3Points(p3, p1, p2)) != sign(angle3Points(p4, p1, p2)) && sign(angle3Points(p1, p3, p4)) != sign(angle3Points(p2, p3, p4))){
        return true;
    }
    return false;
}
bool collTriangleTriangle(Triangle a, Triangle b){
     //   if(pointInTriangle(a.p1))
    if(collLine(a.p1, a.p2, b.p1, b.p2) || collLine(a.p1, a.p2, b.p2, b.p3) || collLine(a.p1, a.p2, b.p1, b.p3) || 
        collLine(a.p2, a.p3, b.p2, b.p3) || collLine(a.p2, a.p3, b.p1, b.p3) || collLine(a.p1, a.p3, b.p1, b.p3)){
            return true;
    }
    if(pointInTriangle(a.p1, b) || pointInTriangle(b.p1, a)){
        return true;
    }
    return false;
}
double2 rotatePointAroundCenter(double2 p, double2 o, double angle){
    double a = angle2Points(o, p), r = distance(o, p);
    a += angle;
    return {o.x + cos(a)*r, o.y + sin(a)*r};
}
pair<Triangle, Triangle> rotatedRectToTriangles(RotatedRect r1){
    double2 p1 = {r1.center.x, r1.center.y}, p2 = {r1.center.x+r1.dims.x, r1.center.y};
    double2 p3 = {r1.center.x, r1.center.y+r1.dims.y}, p4 = {r1.center.x+r1.dims.x, r1.center.y+r1.dims.y};
    double2 c = {p1.x/2 + p4.x/2, p1.y/2 + p4.y/2};
    p1 = rotatePointAroundCenter(p1, c, r1.angle);
    p2 = rotatePointAroundCenter(p2, c, r1.angle);
    p3 = rotatePointAroundCenter(p3, c, r1.angle);
    p4 = rotatePointAroundCenter(p4, c, r1.angle);
    // double r = distance(c, p1);
    // double a1 = angle2Points(c, p1), a2 = angle2Points(c, p2), a3 = angle2Points(c, p3), a4 = angle2Points(c, p4);
    // a1 += r1.angle;
    // a2 += r1.angle;
    // a3 += r1.angle;
    // a4 += r1.angle;
    // p1 = {c.x + cos(a1)*r, c.y + sin(a1)*r};
    // p2 = {c.x + cos(a2)*r, c.y + sin(a2)*r};
    // p3 = {c.x + cos(a3)*r, c.y + sin(a3)*r};
    // p4 = {c.x + cos(a4)*r, c.y + sin(a4)*r};
    return {Triangle(p1, p2, p3), Triangle(p2, p3, p4)};
}
bool collRotatedRectRotatedRect(RotatedRect r1, RotatedRect r2){
    Triangle t1, t2, t3, t4;
    tie(t1, t2) = rotatedRectToTriangles(r1);
    tie(t3, t4) = rotatedRectToTriangles(r2);
    if(collTriangleTriangle(t1, t3) || collTriangleTriangle(t1, t4) || collTriangleTriangle(t2, t3) || collTriangleTriangle(t2, t4)){
        return true;
    }
    return false;
}
bool checkInOffBounds(SDL_Rect rect, int screenWidth, int screenHeight)
{
    if (rect.x + rect.w > screenWidth)
    {
        return true;
    }

    if (rect.x < 0)
    {
        return true;
    }

    if (rect.y + rect.h > screenHeight)
    {
        return true;
    }

    if (rect.y < 0)
    {
        return true;
    }

    return false;
}

int rangeMap(int from_low, int from_high, int to_low, int to_high, int val) {
    double from_diff = (from_high - from_low);
    double to_diff = (to_high - to_low);
    double proportion = ((val - from_low) / from_diff);
    int ret = ((int)round(proportion * to_diff) + to_low);
    /*if (to_low < 20) {
        std::cout << "rMap: " << from_low << " - " << from_high << "\t->\t" << to_low << " - " << to_high << "\t(" << val << ")\n";
        std::cout << "\t\t ret: " << ret << '\n';
    }*/
    return ret;
}

Triangle::Triangle(){}
Triangle::Triangle(double2 a, double2 b, double2 c){
    p1 = a;
    p2 = b;
    p3 = c;

    side1 = distance(p1, p2);
    side2 = distance(p1, p3);
    side3 = distance(p2, p3);
    p = (side1 + side2 + side3) / 2;
    S = sqrt(p * (p - side1) * (p - side2) * (p - side3)); //Heron's formula
}