#include <gdal_priv.h>
#include <iostream>
#include <ogr_api.h>
#include <ogrsf_frmts.h>
#include <vector>

// 定义点类型
struct Point {
    double x, y;
};

void export_to_shapefile(const std::vector<Point>& center_line, const std::string& shapefile_path)
{
    // 注册所有的驱动
    GDALAllRegister();

    // 创建 Shapefile 驱动
    GDALDriver* driver = GetGDALDriverManager()->GetDriverByName("ESRI Shapefile");
    if (!driver) {
        std::cerr << "Shapefile driver not available!" << std::endl;
        return;
    }

    // 创建输出文件
    GDALDataset* dataset = driver->Create(shapefile_path.c_str(), 0, 0, 0, GDT_Unknown, nullptr);
    if (!dataset) {
        std::cerr << "Failed to create shapefile!" << std::endl;
        return;
    }

    // 创建图层
    OGRLayer* layer = dataset->CreateLayer("center_line", nullptr, wkbLineString, nullptr);
    if (!layer) {
        std::cerr << "Failed to create layer!" << std::endl;
        GDALClose(dataset);
        return;
    }

    // 创建中心线几何
    OGRLineString line;
    for (const auto& point : center_line) {
        line.addPoint(point.x, point.y);
    }

    // 创建要素
    OGRFeatureDefn* feature_defn = layer->GetLayerDefn();
    OGRFeature* feature = OGRFeature::CreateFeature(feature_defn);
    feature->SetGeometry(&line);

    // 添加要素到图层
    if (layer->CreateFeature(feature) != OGRERR_NONE) {
        std::cerr << "Failed to add feature to layer!" << std::endl;
    }

    // 清理资源
    OGRFeature::DestroyFeature(feature);
    GDALClose(dataset);

    std::cout << "Shapefile saved to " << shapefile_path << std::endl;
}

int main()
{
    // 示例中心线点
    std::vector<Point> center_line = { { 0, 0 }, { 5, 5 }, { 10, 0 } };

    // 导出为 Shapefile
    export_to_shapefile(center_line, "/home/sky/workspace/GIS算法实习/03双线求中心线算法/data/center_line.shp");

    return 0;
}
