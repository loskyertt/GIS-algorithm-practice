#include "edge.hpp"
#include <algorithm>
#include <climits>
#include <cmath>
#include <cstddef>
#include <iostream>
#include <utility>
#include <vector>

void EdgeTable::addVertex(const Point& p)
{
    vertices.push_back(p);
}

void EdgeTable::polygonFill()
{
    int y_min = INT_MAX, y_max = INT_MIN;

    // 计算y的范围并初始化边表
    for (const auto& v : vertices) {
        y_min = min(y_min, v.y);
        y_max = max(y_max, v.y);
    }

    vector<EdgeTableEntry> edgeTable(y_max - y_min + 1);

    // 构建边表
    int n = vertices.size();
    for (int i = 0; i < n; i++) {
        auto v1 = vertices[i];
        auto v2 = vertices[(i + 1) % n]; // 相邻顶点形成边

        if (v1.y == v2.y)
            continue; // 忽略水平边

        // 保证 v1 是 y 较小的点，v2 是 y 较大的点
        if (v1.y > v2.y)
            swap(v1, v2);

        // 构造边
        Edge edge;
        edge.x = v1.x;
        edge.dx = float(v2.x - v1.x) / (v2.y - v1.y); // 斜率倒数
        edge.y_max = v2.y;

        // 插入边到对应的边表位置
        edgeTable[v1.y - y_min].edges.push_back(edge);
    }

    // 活动边表 (AET)
    vector<Edge> AET;

    // 扫描线逐行填充
    for (int y = y_min; y <= y_max; y++) {
        // 将当前扫描线对应的边加入到 AET
        for (const auto& edge : edgeTable[y - y_min].edges) {
            AET.push_back(edge);
        }

        // 移除 AET 中 y == y_max 的边
        AET.erase(
            remove_if(AET.begin(), AET.end(),
                [y](const Edge& edge) { return edge.y_max == y; }),
            AET.end());

        // 按 x 坐标排序 AET
        sort(AET.begin(), AET.end(),
            [](const Edge& e1, const Edge& e2) { return e1.x < e2.x; });

        // 绘制当前扫描线上的填充区间
        for (size_t i = 0; i < AET.size(); i += 2) {
            int xStart = ceil(AET[i].x);
            int xEnd = floor(AET[i + 1].x);

            for (int x = xStart; x <= xEnd; x++) {
                cout << "(" << x << ", " << y << ") "; // 模拟填充
            }
        }
        cout << endl;

        // 更新 AET 中边的 x 值
        for (auto& edge : AET) {
            edge.x += edge.dx;
        }
    }
}
