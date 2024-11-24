#pragma once

#include "point.hpp"
#include <vector>

using namespace std;

class Polygon {
private:
    vector<Point> vertices; // 多边形顶点集合

    /* 计算两个向量（从原点到点的向量）的叉积 */
    double crossProduct(const Point& a, const Point& b, const Point& origin = Point(0, 0)) const;

public:
    /* 添加顶点 */
    void addVertex(const Point& p);

    /* 计算多边形面积（不含孔） */
    double calculateArea() const;

    /* 计算多边形面积（含孔） */
    static double calculateAreaWithHoles(const vector<vector<Point>>& rings); // 这里不需要访问任何实例成员变量，所以设置为静态成员函数

    /* 边界点检测 */
    bool isOnSegment(const Point& p, const Point& segStart, const Point& segEnd) const;

    /* 判断点是否在多边形内（射线法） */
    bool containsPoint(const Point& p) const;

    /* 判断多边形的顺时、逆时方向（通过叉积） */
    bool isClockwiseVector();
};
