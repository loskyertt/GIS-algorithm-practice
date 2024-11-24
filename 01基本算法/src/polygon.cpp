#include "polygon.hpp"
#include "point.hpp"
#include <cstddef>
#include <cstdlib>

double Polygon::crossProduct(const Point& a, const Point& b, const Point& origin) const
{
    return (a.x - origin.x) * (b.y - origin.y) - (a.y - origin.y) * (b.x - origin.x);
}

void Polygon::addVertex(const Point& p)
{
    vertices.push_back(p);
}

double Polygon::calculateArea() const
{
    if (vertices.size() < 3) {
        return 0;
    }

    double area = 0.0;
    for (size_t i = 0; i < vertices.size(); i++) {
        const Point& current = vertices[i];
        const Point& next = vertices[(i + 1) % vertices.size()]; // 获取下一个顶点（最后一个顶点的下一个是第一个顶点）
        area += crossProduct(current, next);
    }

    return abs(area) / 2.0;
}

double Polygon::calculateAreaWithHoles(const vector<vector<Point>>& rings)
{
    if (rings.empty()) {
        return 0;
    }
    double totalArea = 0.0;

    Polygon tempPolygon; // 目的：单独计算每个多边形的面积

    // 计算外环面积
    tempPolygon.vertices = rings[0];
    double outerArea = tempPolygon.calculateArea();

    // 减去所有内环（孔）面积
    for (size_t i = 1; i < rings.size(); i++) {
        tempPolygon.vertices = rings[i];
        outerArea -= tempPolygon.calculateArea();
    }

    return outerArea;
}

bool Polygon::isOnSegment(const Point& p, const Point& segStart, const Point& segEnd) const
{
    double cross = (p.y - segStart.y) * (segEnd.x - segStart.x) - (p.x - segStart.x) * (segEnd.y - segStart.y);
    if (abs(cross) > 1e-9)
        return false; // 不在直线上
    if (p.x < min(segStart.x, segEnd.x) || p.x > max(segStart.x, segEnd.x))
        return false;
    if (p.y < min(segStart.y, segEnd.y) || p.y > max(segStart.y, segEnd.y))
        return false;
    return true;
}

bool Polygon::containsPoint(const Point& p) const
{
    bool inside = false;
    size_t j = vertices.size() - 1;

    for (size_t i = 0; i < vertices.size(); i++) {
        // 条件1：检查测试点y坐标是否在边的y范围内
        // 条件2：检查射线与边的交点是否在测试点的右侧
        if (((vertices[i].y > p.y) != (vertices[j].y > p.y)) && (p.x < (vertices[j].x - vertices[i].x) * (p.y - vertices[i].y) / (vertices[j].y - vertices[i].y) + vertices[i].x)) {
            inside = !inside;
        }
        if (isOnSegment(p, vertices[i], vertices[j])) {
            return true; // 点在多边形边上
        }
        j = i;
    }

    return inside;
}

bool Polygon::isClockwiseVector()
{
    if (vertices.size() < 3) {
        return false;
    }

    size_t topPoint = 0;
    for (size_t i = 1; i < vertices.size(); ++i) {
        if (vertices[i].y > vertices[topPoint].y || (vertices[i].y == vertices[topPoint].y && vertices[i].x < vertices[topPoint].x)) {
            topPoint = i;
        }
    }

    // 获取前一个点和后一个点
    size_t prev = (topPoint - 1 + vertices.size()) % vertices.size();
    size_t next = (topPoint + 1) % vertices.size();

    // 计算向量叉积
    double croP = (vertices[topPoint].x - vertices[prev].x) * (vertices[next].y - vertices[topPoint].y) - (vertices[topPoint].y - vertices[prev].y) * (vertices[next].x - vertices[topPoint].x);

    return croP > 0;
}
