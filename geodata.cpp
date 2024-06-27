#include "geodata.h"
#include <QDebug>

GeoData::GeoData(const char *fileDir)
{
    m_GDALDatasetUniquePtr = GDALDatasetUniquePtr((GDALDataset *) GDALOpenEx(fileDir, GDAL_OF_VECTOR, NULL, NULL, NULL));
    if (m_GDALDatasetUniquePtr == nullptr) {
        qDebug() << "打开文件失败" << fileDir;
        return;
    }
    else{
        qDebug() << "加载成功 : " << fileDir;
    }
    needDraw = true;
}

GeoData::~GeoData()
{

}

const GDALDatasetUniquePtr& GeoData::getDataUniquePtr() const
{
    return m_GDALDatasetUniquePtr;
}

bool GeoData::getNeedDraw() const
{
    return needDraw;
}

void GeoData::setNeedDraw(bool newNeedDraw)
{
    needDraw = newNeedDraw;
}
