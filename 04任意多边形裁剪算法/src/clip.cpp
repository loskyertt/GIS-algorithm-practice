#include "clip.hpp"

inline bool PolygonClipper::inside(const Point& p, int edge) const
{
    switch (edge) {
    case 0:
        return p.x >= x_min; // 左边界
    case 1:
        return p.x <= x_max; // 右边界
    case 2:
        return p.y >= y_min; // 下边界
    case 3:
        return p.y <= y_max; // 上边界
    default:
        return false;
    }
}

Point PolygonClipper::intersect(const Point& s, const Point& p, int edge) const
{
    Point i;
    switch (edge) {
    case 0: // 左边界
        if (abs(p.x - s.x) < 1e-6) {
            i.x = x_min;
            i.y = s.y;
        } else {
            i.x = x_min;
            i.y = s.y + (p.y - s.y) * (x_min - s.x) / (p.x - s.x);
        }
        break;
    case 1: // 右边界
        if (abs(p.x - s.x) < 1e-6) {
            i.x = x_max;
            i.y = s.y;
        } else {
            i.x = x_max;
            i.y = s.y + (p.y - s.y) * (x_max - s.x) / (p.x - s.x);
        }
        break;
    case 2: // 下边界
        if (abs(p.y - s.y) < 1e-6) {
            i.y = y_min;
            i.x = s.x;
        } else {
            i.y = y_min;
            i.x = s.x + (p.x - s.x) * (y_min - s.y) / (p.y - s.y);
        }
        break;
    case 3: // 上边界
        if (abs(p.y - s.y) < 1e-6) {
            i.y = y_max;
            i.x = s.x;
        } else {
            i.y = y_max;
            i.x = s.x + (p.x - s.x) * (y_max - s.y) / (p.y - s.y);
        }
        break;
    }
    return i;
}

vector<Point> PolygonClipper::clipAgainstEdge(const vector<Point>& input, int edge)
{
    vector<Point> output;
    Point prevPoint = input.back();

    for (const auto& currPoint : input) {
        if (inside(currPoint, edge)) {
            if (!inside(prevPoint, edge)) {
                output.push_back(intersect(prevPoint, currPoint, edge));
            }
            output.push_back(currPoint);
        } else if (inside(prevPoint, edge)) {
            output.push_back(intersect(prevPoint, currPoint, edge));
        }
        prevPoint = currPoint;
    }
    return output;
}

PolygonClipper::PolygonClipper(float x1, float y1, float x2, float y2)
    : x_min(x1)
    , y_min(y1)
    , x_max(x2)
    , y_max(y2)
{
}

vector<Point> PolygonClipper::clip(const vector<Point>& subject)
{
    vector<Point> output = subject;
    for (int edge = 0; edge < 4; ++edge) {
        output = clipAgainstEdge(output, edge);
    }
    return output;
}