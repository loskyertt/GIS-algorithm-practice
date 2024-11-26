#include "point.hpp"
#include "polyline.hpp"
#include <GL/freeglut_std.h>

PolyLine* globalPly = nullptr;

// 全局回调函数
void displayCallback()
{
    if (globalPly) {
        globalPly->display();
    }
}

int main(int argc, char** argv)
{
    PolyLine ply;

    // 创建折线
    ply.addVertex(Point(150, 100));
    ply.addVertex(Point(200, 200));
    ply.addVertex(Point(300, 150));
    ply.addVertex(Point(400, 250));

    // 生成缓冲区数据
    ply.generateBuffer();

    // 初始化 GLUT
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Polyline Buffer Visualization");

    PolyLine::initOpenGL();

    // 全局指针
    globalPly = &ply;

    glutDisplayFunc(displayCallback);

    glutMainLoop();

    return 0;
}