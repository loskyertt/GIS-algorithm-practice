#include "clip.hpp"
#include <GL/glut.h>
#include <cmath>
#include <vector>

/* 裁剪窗口范围 */
float clipWindowXMin = 200, clipWindowYMin = 200, clipWindowXMax = 400, clipWindowYMax = 400;
vector<Point> originalPolygon;
vector<Point> clippedPolygon;

void initGL()
{
    glClearColor(1.0, 1.0, 1.0, 1.0);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0, 600, 0, 600);
}

void drawPolygon(const vector<Point>& polygon, float r, float g, float b, float alpha = 1.0f)
{
    glColor4f(r, g, b, alpha);
    glBegin(GL_POLYGON);
    for (const auto& point : polygon) {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void drawPolygonOutline(const vector<Point>& polygon, float r, float g, float b)
{
    glColor3f(r, g, b);
    glLineWidth(2.0);
    glBegin(GL_LINE_LOOP);
    for (const auto& point : polygon) {
        glVertex2f(point.x, point.y);
    }
    glEnd();
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);

    // 绘制裁剪窗口（红色边框）
    glColor3f(1.0, 0.0, 0.0);
    glLineWidth(3.0);
    glBegin(GL_LINE_LOOP);
    glVertex2f(clipWindowXMin, clipWindowYMin);
    glVertex2f(clipWindowXMax, clipWindowYMin);
    glVertex2f(clipWindowXMax, clipWindowYMax);
    glVertex2f(clipWindowXMin, clipWindowYMax);
    glEnd();

    // 绘制原始多边形（蓝色，半透明）
    drawPolygon(originalPolygon, 0.0, 0.0, 1.0, 0.5);

    // 执行裁剪
    PolygonClipper clipper(clipWindowXMin, clipWindowYMin, clipWindowXMax, clipWindowYMax);
    clippedPolygon = clipper.clip(originalPolygon);

    // 绘制裁剪后多边形（绿色，半透明）
    drawPolygon(clippedPolygon, 0.0, 1.0, 0.0, 0.5);

    glFlush();
}

int main(int argc, char** argv)
{
    // 测试用例：正八边形（凸边形）
    originalPolygon = {
        { 100, 300 }, { 150, 450 }, { 300, 500 }, { 450, 450 },
        { 500, 300 }, { 450, 150 }, { 300, 100 }, { 150, 150 }
    };

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(600, 600);
    glutCreateWindow("Sutherland-Hodgman Polygon Clipping");

    initGL();
    glutDisplayFunc(display);
    glutMainLoop();

    return 0;
}
