#pragma once

#include <vector>
using namespace std;

// 边
struct Edge {
    float x; // 当前扫描线与边的交点 x 坐标
    float y; // 斜率的倒数 1/k
    int y_max; // 边的 y 坐标最大值（边终止的 y 值）
};

// 边表。按 y 坐标排序
struct EdgeTableEntry {
    vector<Edge> edges;
};