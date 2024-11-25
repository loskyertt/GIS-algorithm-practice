#include "point.hpp"
#include "polygon.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <iostream>

namespace bg = boost::geometry;

/* 1.测试示例：多边形面积 */
void testArea()
{
    // 1.多边形面积（不带孔）
    Polygon poly1;
    poly1.addVertex(Point(1, 1));
    poly1.addVertex(Point(3, 1));
    poly1.addVertex(Point(2, 4));
    double area = poly1.calculateArea(); // 面积应为3.0

    cout << "多边形面积（不带孔）为：" << area << endl;

    // 2.判断点与多边形的位置关系
    // bool isCon = poly1.containsPoint(Point(0, 0.5)); // 多边形外的点
    // if (isCon) {
    //     cout << "该点在多边形内" << endl;
    // } else {
    //     cout << "该点在多边形外" << endl;
    // }

    // 3.多边形面积（带孔）
    vector<vector<Point>> rings;

    // 外环
    vector<Point> outerRing = {
        Point(0, 0), Point(10, 0),
        Point(10, 10), Point(0, 10),
        Point(0, 0)
    };

    // 内环（孔）
    vector<Point> innerRing = {
        Point(2, 2), Point(8, 2),
        Point(8, 8), Point(2, 8),
        Point(2, 2)
    };

    rings.push_back(outerRing);
    rings.push_back(innerRing);

    double areaWithHole = Polygon::calculateAreaWithHoles(rings); // 面积应为外环面积(100) - 内环面积(36) = 64

    cout << "多边形面积（带孔）为：" << areaWithHole << endl;
}

/* 2.测试：多边形方向 */
void testDirection()
{
    // 3.判断多边形的顺时、逆时方向
    Polygon poly2;
    poly2.addVertex(Point(0, 0));
    poly2.addVertex(Point(0, 1));
    poly2.addVertex(Point(1, 1));
    poly2.addVertex(Point(1, 0));

    bool clockwiseVector = poly2.isClockwiseVector();

    if (clockwiseVector) {
        cout << "多边形是顺时针方向" << endl;
    } else {
        cout << "多边形是逆时针方向" << endl;
    }
}

/* 九交模型算法 */
void test_9IM()
{
    // 定义多边形和点
    bg::model::polygon<bg::model::d2::point_xy<double>> poly;
    bg::append(poly, bg::model::d2::point_xy<double>(0.0, 0.0));
    bg::append(poly, bg::model::d2::point_xy<double>(4.0, 0.0));
    bg::append(poly, bg::model::d2::point_xy<double>(4.0, 4.0));
    bg::append(poly, bg::model::d2::point_xy<double>(0.0, 4.0));
    bg::append(poly, bg::model::d2::point_xy<double>(0.0, 0.0));

    bg::model::d2::point_xy<double> point(2.0, 2.0);

    // 判断点是否在多边形内部
    if (bg::within(point, poly)) {
        std::cout << "点在多边形内。" << std::endl;
    } else {
        std::cout << "点在多边形外。" << std::endl;
    }
}

int main()
{
    // testArea();

    // testDirection();

    test_9IM();

    return 0;
}