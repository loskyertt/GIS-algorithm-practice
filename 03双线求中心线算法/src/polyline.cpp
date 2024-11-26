#include "polyline.hpp"
#include <GL/glut.h>
#include <cmath>

PolyLine::PolyLine(float R)
{
    this->R = R;
}

void PolyLine::addVertex(const Point& p)
{
    this->polyline.push_back(p);
}

/*
该函数计算的是从折线的某一段（由 p1 和 p2 确定）到垂直方向的单位向量，
并根据给定的距离 distance 返回一个向量，该向量用于表示从折线到缓冲区边线的偏移。
*/
Point PolyLine::perpendicularVector(Point p1, Point p2, float distance)
{
    float dx = p2.x - p1.x;
    float dy = p2.y - p1.y;
    float length = sqrt(dx * dx + dy * dy);
    return Point(-dy / length * distance, dx / length * distance);
}

Point PolyLine::intersection(Point a1, Point a2, Point b1, Point b2)
{
    float A1 = a2.y - a1.y;
    float B1 = a1.x - a2.x;
    float C1 = A1 * a1.x + B1 * a1.y;

    float A2 = b2.y - b1.y;
    float B2 = b1.x - b2.x;
    float C2 = A2 * b1.x + B2 * b1.y;

    float det = A1 * B2 - A2 * B1;
    if (std::abs(det) < 1e-6)
        return a1; // 平行或重叠时，返回一个默认值
    return Point((B2 * C1 - B1 * C2) / det, (A1 * C2 - A2 * C1) / det);
}

void PolyLine::generateBuffer()
{
    size_t n = polyline.size();
    // 不构成线
    if (n < 2)
        return;

    // 首点
    Point dir = perpendicularVector(polyline[0], polyline[1], R);
    leftBuffer.push_back(Point(polyline[0].x + dir.x, polyline[0].y + dir.y));
    rightBuffer.push_back(Point(polyline[0].x - dir.x, polyline[0].y - dir.y));

    // 转折点
    for (size_t i = 1; i < n - 1; ++i) {
        Point dir1 = perpendicularVector(polyline[i - 1], polyline[i], R);
        Point dir2 = perpendicularVector(polyline[i], polyline[i + 1], R);

        Point leftIntersect = intersection(
            Point(polyline[i].x + dir1.x, polyline[i].y + dir1.y),
            Point(polyline[i - 1].x + dir1.x, polyline[i - 1].y + dir1.y),
            Point(polyline[i].x + dir2.x, polyline[i].y + dir2.y),
            Point(polyline[i + 1].x + dir2.x, polyline[i + 1].y + dir2.y));
        Point rightIntersect = intersection(
            Point(polyline[i].x - dir1.x, polyline[i].y - dir1.y),
            Point(polyline[i - 1].x - dir1.x, polyline[i - 1].y - dir1.y),
            Point(polyline[i].x - dir2.x, polyline[i].y - dir2.y),
            Point(polyline[i + 1].x - dir2.x, polyline[i + 1].y - dir2.y));

        leftBuffer.push_back(leftIntersect);
        rightBuffer.push_back(rightIntersect);
    }

    // 尾点
    dir = perpendicularVector(polyline[n - 2], polyline[n - 1], R);
    leftBuffer.push_back(Point(polyline[n - 1].x + dir.x, polyline[n - 1].y + dir.y));
    rightBuffer.push_back(Point(polyline[n - 1].x - dir.x, polyline[n - 1].y - dir.y));
}

void PolyLine::display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制折线
    glColor3f(0.0, 0.0, 1.0); // 蓝色
    glBegin(GL_LINE_STRIP);
    for (const auto& p : polyline) {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // 绘制左缓冲区边线
    glColor3f(1.0, 0.0, 0.0); // 红色
    glBegin(GL_LINE_STRIP);
    for (const auto& p : leftBuffer) {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    // 绘制右缓冲区边线
    glColor3f(1.0, 0.0, 0.0); // 红色
    glBegin(GL_LINE_STRIP);
    for (const auto& p : rightBuffer) {
        glVertex2f(p.x, p.y);
    }
    glEnd();

    glFlush();
}

void PolyLine::initOpenGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0); // 白色背景
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 500.0, 0.0, 500.0); // 设置坐标系范围
}