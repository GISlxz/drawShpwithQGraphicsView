#include "geodata.h"
#include <QDebug>

GeoData::GeoData(const char *fileDir):location(fileDir)
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
    name = extractFileName(fileDir);
    //qDebug()<<name<<" "<<location;
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

QString GeoData::getName() const
{
    return name;
}

void GeoData::setName(const QString &newName)
{
    name = newName;
}

QString GeoData::getLocation() const
{
    return location;
}

void GeoData::setLocation(const QString &newLocation)
{
    location = newLocation;
}

// 提取文件名的函数
QString GeoData::extractFileName(const char* filePath) {
    QString path(filePath);
    // 查找最后一个路径分隔符的位置
    int pos = path.lastIndexOf(QChar('/'));
    if (pos == -1) {
        pos = path.lastIndexOf(QChar('\\'));
    }
    // 提取路径分隔符之后的子字符串
    QString fileName = path.mid(pos + 1);
    return fileName;
}
