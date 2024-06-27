#include "gisdatamanager.h"
#include <QDebug>
GISDataManager::GISDataManager()
{
    m_geoDataList = new QList<GeoData*>;
}

GISDataManager::~GISDataManager()
{
    // 首先检查列表指针是否为空
    if (m_geoDataList) {
        // 遍历列表，删除每个GeoData对象
        for (int i = 0; i < m_geoDataList->size(); ++i) {
            delete (*m_geoDataList)[i];  // 删除指针指向的对象
        }
        m_geoDataList->clear();  // 清空列表
        delete m_geoDataList;    // 删除列表指针本身
    }
    qDebug()<<"GISDataManager release";
}

bool GISDataManager::addGeoData(const char *fileDir)
{
    GeoData* geodata = new GeoData(fileDir);
    if(geodata){
        qDebug()<<"add success";
        m_geoDataList->append(std::move(geodata));
        m_dataNum++;
        return true;
    }
    qDebug()<<"add failed";
    return false;
}

GeoData* GISDataManager::getDataByindex(int index)
{
    return m_geoDataList->at(index);
}

int GISDataManager::dataNum() const
{
    return m_dataNum;
}
