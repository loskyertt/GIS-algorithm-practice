#pragma once

#include <cstdlib>

using namespace std;

struct Point {
    float x, y;

    Point(float x_ = 0, float y_ = 0)
        : x(x_)
        , y(y_)
    {
    }

    // 运算重载：比较两个点是否相等
    bool operator==(const Point& other) const
    {
        return abs(x - other.x) < 1e-6 && std::abs(y - other.y) < 1e-6;
    }
};