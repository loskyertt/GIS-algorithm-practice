from polygonFill import scanline_fill_polygon
import cv2


def clip_image_with_polygon(image_path: str, vertices: list, output_path: str) -> None:
    """裁剪图片，填充指定多边形区域，并将结果保存到文件。

    Args:
        image_path: 原始图片路径。
        vertices: 多边形顶点坐标列表[(x1, y1), (x2, y2), ...]。
        output_path: 结果图片保存路径。
    """
    # 读取图片
    img = cv2.imread(image_path)
    if img is None:
        raise FileNotFoundError(f"无法找到图片：{image_path}")

    # 填充多边形区域
    filled_img = scanline_fill_polygon(img, vertices)

    # 保存结果
    cv2.imwrite(output_path, filled_img)
    print(f"结果保存至：{output_path}")


if __name__ == "__main__":
    # 示例：裁剪图片并填充多边形
    image_path = r"data\LinearDisp.png"
    output_path = r"data\output_filled_map.jpg"

    # 多边形顶点坐标 (根据实际图片分辨率调整)
    polygon_vertices = [(50, 50), (200, 80), (180, 200), (30, 180)]

    # 裁剪并填充
    clip_image_with_polygon(image_path, polygon_vertices, output_path)
