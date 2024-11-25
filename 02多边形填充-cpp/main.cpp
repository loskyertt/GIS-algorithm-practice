#include "edge.hpp"
#include "point.hpp"
#include <iostream>

void test()
{
    EdgeTable et;
    et.addVertex(Point(2, 1));
    et.addVertex(Point(6, 1));
    et.addVertex(Point(8, 4));
    et.addVertex(Point(4, 6));
    et.addVertex(Point(1, 4));

    cout << "填充多边形的像素坐标：" << endl;
    et.polygonFill();
}

int main()
{
    test();

    return 0;
}