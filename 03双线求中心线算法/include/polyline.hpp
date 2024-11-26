#pragma once

#include "point.hpp"
#include <vector>

using namespace std;

class PolyLine {
private:
    // 折线
    vector<Point> polyline;

    // 缓存区左右边线
    vector<Point> leftBuffer;
    vector<Point> rightBuffer;

    // 缓冲区半径
    float R;

public:
    /* 初始化缓冲区半径，默认是 20.0*/
    PolyLine(float R = 20.0f);

    /* 添加点构成（折）线 */
    void addVertex(const Point& p);

    /* 计算垂直方向的单位向量*/
    Point perpendicularVector(Point pl, Point p2, float distance);

    /* 计算转折点的缓冲区交点 */
    Point intersection(Point a1, Point a2, Point b1, Point b2);

    /* 生成缓冲区边线 */
    void generateBuffer();

    /* 渲染函数 */
    void display();

    /* 初始化 OpenGL */
    static void initOpenGL();
};