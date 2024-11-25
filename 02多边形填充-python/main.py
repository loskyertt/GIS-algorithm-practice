import math
import numpy as np
import matplotlib.pyplot as plt
from collections import namedtuple

Point = namedtuple("Point", ["x", "y"])

class EdgeTable:
    def __init__(self, x, dx, ymax):
        self.x = x
        self.dx = dx
        self.ymax = ymax
        self.next = None


def fill_polys(polys, num=1):
    poly_collection = []
    each_poly_max_xy = []
    each_poly_min_xy = []
    poly_max_x = float("-inf")
    poly_max_y = float("-inf")

    for poly in polys:
        points = []
        max_x = float("-inf")
        max_y = float("-inf")
        min_x = float("inf")
        min_y = float("inf")
        for point in poly:
            p = Point(*point)
            points.append(p)
            max_x = max(max_x, p.x)
            max_y = max(max_y, p.y)
            min_x = min(min_x, p.x)
            min_y = min(min_y, p.y)
        each_poly_max_xy.append(Point(max_x, max_y))
        each_poly_min_xy.append(Point(min_x, min_y))
        poly_max_x = max(poly_max_x, max_x)
        poly_max_y = max(poly_max_y, max_y)
        poly_collection.append(points)

    poly_len_x = poly_max_x + 1
    poly_len_y = poly_max_y + 1
    bg = np.zeros((poly_len_y, poly_len_x), dtype=np.uint8)  # 注意：y 是行，x 是列

    # build edge table
    count = -1
    for poly in poly_collection:
        count += 1
        point_num = len(poly)
        point_range_y = each_poly_max_xy[count].y + 1
        net = [None] * point_range_y
        for i in range(point_num):
            before = (i - 1 + point_num) % point_num
            after = (i + 1 + point_num) % point_num
            for nbr in [before, after]:
                if poly[nbr].y > poly[i].y:
                    x_ = poly[i].x
                    dx_ = (poly[nbr].x - poly[i].x) / (poly[nbr].y - poly[i].y)
                    ymax_ = poly[nbr].y
                    pnt = EdgeTable(x_, dx_, ymax_)
                    pnt.next = net[poly[i].y]
                    net[poly[i].y] = pnt

        # build activate edge table
        aet = EdgeTable(0, 0, 0)
        for i in range(each_poly_min_xy[count].y, point_range_y):
            pa = aet.next
            while pa:
                pa.x = pa.x + pa.dx
                pa = pa.next

            pa = aet
            pcur = aet.next
            pa.next = None
            while pcur:
                while pa.next and pcur.x >= pa.next.x:
                    pa = pa.next
                pt = pcur.next
                pcur.next = pa.next
                pa.next = pcur
                pcur = pt
                pa = aet

            pa = aet
            pcur = pa.next
            while pcur and i < each_poly_max_xy[count].y:
                if pcur.ymax == i:
                    pa.next = pcur.next
                    del pcur
                    pcur = pa.next
                else:
                    pcur = pcur.next
                    pa = pa.next

            pn = net[i]
            pa = aet
            while pn:
                while pa.next and pn.x >= pa.next.x:
                    pa = pa.next
                pt = pn.next
                pn.next = pa.next
                pa.next = pn
                pn = pt
                pa = aet

            pcur = aet.next
            while pcur and pcur.next:
                s = math.ceil(pcur.x) if pcur.dx > 0 else math.floor(pcur.x)
                e = math.ceil(pcur.next.x) if pcur.next.dx > 0 else math.floor(pcur.next.x)
                for x in range(s, e + 1):
                    bg[i][x] = num
                pcur = pcur.next

    # 可视化显示
    plt.imshow(bg, cmap="gray", origin="lower")
    plt.title("Polygon Fill Visualization")
    plt.xlabel("X-axis")
    plt.ylabel("Y-axis")
    plt.show()


if __name__ == "__main__":
    polys = [
        [
            [0, 5],
            [3, 2],
            [6, 1],
            [9, 3],
            [6, 4],
            [6, 7],
            [9, 8],
            [4, 10],
            [2, 9],
        ],
        [[2, 6], [4, 4], [5, 5], [4, 7]],
    ]
    fill_polys(polys)
