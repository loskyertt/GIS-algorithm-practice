import numpy as np
from typing import List, Tuple


def scanline_fill_polygon(img: np.ndarray,
                          vertices: List[Tuple[int, int]],
                          color: Tuple[int, int, int] = (255, 255, 0)) -> np.ndarray:
    """用活动边表 (AET) 实现多边形填充算法，并在给定图片上填充多边形。

    Args:
        img: 图像数组。
        vertices: 格式为[(x1, y1), (x2, y2), ...]的多边形顶点坐标。
        color: 填充颜色，默认黄色 (255, 255, 0)。
    Returns:
        img: 填充后的图像。
    """
    # 不构成多边形
    if len(vertices) < 3:
        return img

    # 确保像素坐标是整数和去重
    vertices = list(dict.fromkeys((int(x), int(y)) for x, y in vertices))

    # 图像高度和宽度
    # 确定多边形在图片中的 y 范围
    h, w = img.shape[:2]
    y_min = max(0, int(min(y for _, y in vertices)))
    y_max = min(h-1, int(max(y for _, y in vertices)))

    # 边表
    edge_table = [[] for _ in range(y_min, y_max + 1)]

    # 构建边表
    for i in range(len(vertices)):
        x1, y1 = vertices[i]
        x2, y2 = vertices[(i + 1) % len(vertices)]

        # 跳过水平边（它们不会影响垂直扫描线的填充）
        if y1 == y2:
            continue

        # 边：y1 -> y2
        if y1 > y2:
            x1, x2 = x2, x1
            y1, y2 = y2, y1

        if y2 > y_min and y1 < y_max:
            idx = max(0, y1 - y_min)
            edge_table[idx].append({
                'x': x1,    # 边在当前扫描线上的起始 x 坐标
                'dx': (x2 - x1) / (y2 - y1),    # 斜率倒数，用于在每一条扫描线中更新 x 值
                'ymax': y2  # 边的上端点的 y 坐标，用于判断边何时失效
            })

    active_edges = []
    for y in range(y_min, y_max + 1):
        # 移除活动边表中已失效的边
        active_edges = [edge for edge in active_edges if edge['ymax'] > y]

        active_edges.extend(edge_table[y - y_min])

        if active_edges:
            active_edges.sort(key=lambda e: e['x'])

            x_coords = np.array([int(edge['x']) for edge in active_edges])
            for i in range(0, len(x_coords), 2):
                if i + 1 < len(x_coords):
                    x_start = max(0, x_coords[i])
                    x_end = min(w, x_coords[i + 1])
                    if x_start < x_end:
                        img[y, x_start:x_end] = color

            for edge in active_edges:
                edge['x'] += edge['dx']

    return img
