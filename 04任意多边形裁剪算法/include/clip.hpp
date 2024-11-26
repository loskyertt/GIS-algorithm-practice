#include "point.hpp"

#include <vector>

class PolygonClipper {
private:
    /* 裁剪窗口边界 */
    float x_min, y_min, x_max, y_max;

    /* 判断点是否在裁剪边界内 */
    inline bool inside(const Point& p, int edge) const;

    /* 计算多边形边 (s, p) 与指定裁剪边的交点 */
    Point intersect(const Point& s, const Point& p, int edge) const;

    /* 对多边形逐边裁剪 */
    vector<Point> clipAgainstEdge(const vector<Point>& input, int edge);

public:
    /* 初始化裁剪窗口的范围，两点分别对应左下角和右上角 */
    PolygonClipper(float x1, float y1, float x2, float y2);

    /* Sutherland-Hodgman 算法实现 */
    vector<Point> clip(const vector<Point>& subject);
};