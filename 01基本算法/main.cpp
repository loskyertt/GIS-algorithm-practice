#include "point.hpp"
#include "polygon.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/adapted/boost_variant.hpp>
#include <boost/geometry/geometries/linestring.hpp>
#include <boost/geometry/geometries/point.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/io/wkt/wkt.hpp>
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

    // 2.多边形面积（带孔）
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

/* 3.测试：点与多边形的位置关系 */
void test_topology()
{
    // 手动实现
    Polygon poly3;
    poly3.addVertex(Point(1, 1));
    poly3.addVertex(Point(3, 1));
    poly3.addVertex(Point(2, 4));

    // 2.判断点与多边形的位置关系
    bool isCon = poly3.containsPoint(Point(0, 0.5)); // 多边形外的点
    if (isCon) { // 可以额为添加一个判断条件用于判断点在多边形上（这里按照点在多边形内处理）
        cout << "点在多边形内。" << endl;
    } else {
        cout << "点在多边形外。" << endl;
    }

    cout << "--------------------------" << endl;

    // boost 库实现
    // 定义多边形
    bg::model::polygon<bg::model::d2::point_xy<double>> poly;
    bg::append(poly, bg::model::d2::point_xy<double>(1, 1));
    bg::append(poly, bg::model::d2::point_xy<double>(3, 1));
    bg::append(poly, bg::model::d2::point_xy<double>(2, 4));
    bg::append(poly, bg::model::d2::point_xy<double>(1, 1)); // 确保多边形闭合
    // 定义点
    bg::model::d2::point_xy<double> point(0, 0.5);

    // 判断点是否在多边形内部
    if (bg::within(point, poly)) {
        cout << "点在多边形内。" << endl;
    } else {
        cout << "点在多边形外。" << endl;
    }
}

/* 4.测试：boost 完整的九交模型算法 */
void test_9IM()
{
    typedef bg::model::point<double, 2, bg::cs::cartesian> Point;
    typedef bg::model::polygon<Point> Polygon;
    typedef bg::model::linestring<Point> LineString;

    Polygon polygon;
    bg::read_wkt("POLYGON((0 0, 0 5, 5 5, 5 0, 0 0))", polygon);

    LineString line;
    bg::read_wkt("LINESTRING(2 2, 6 6)", line);

    auto relation_matrix = bg::relation(polygon, line);

    // 直接获取矩阵的字符串表示
    string relation = relation_matrix.str();

    cout << "DE-9IM 矩阵: " << relation << endl;

    // 如果需要访问单个位置，可以直接访问字符串的相应位置
    cout << "Matrix Breakdown:" << endl;
    cout << "Interior-Interior: " << relation[0] << endl;
    cout << "Interior-Boundary: " << relation[1] << endl;
    cout << "Interior-Exterior: " << relation[2] << endl;
    cout << "Boundary-Interior: " << relation[3] << endl;
    cout << "Boundary-Boundary: " << relation[4] << endl;
    cout << "Boundary-Exterior: " << relation[5] << endl;
    cout << "Exterior-Interior: " << relation[6] << endl;
    cout << "Exterior-Boundary: " << relation[7] << endl;
    cout << "Exterior-Exterior: " << relation[8] << endl;
}

int main()
{
    // testArea();

    // testDirection();

    // test_topology();

    test_9IM();

    return 0;
}