#include "geodatamodel.h"

GeoDataModel::GeoDataModel(QObject *parent)
    : QAbstractListModel{parent}
{
    m_geoDataList = new QList<GeoData*>;
}


GeoDataModel::~GeoDataModel()
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
    qDebug()<<"GISDataModel release";
}

int GeoDataModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_geoDataList->count();
}

QVariant GeoDataModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= m_geoDataList->size())
        return QVariant();

    if (role == Qt::DisplayRole) {
        return m_geoDataList->at(index.row())->getName();
    }

    return QVariant();
}

QHash<int, QByteArray> GeoDataModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[Qt::DisplayRole] = "display";
    return roles;
}

bool GeoDataModel::addGeoData(QString fileDir)
{
    GeoData* geodata = new GeoData(fileDir.toUtf8().constData());
    if(geodata){ //如果打开成功
        qDebug()<<"add success";
        m_geoDataList->append(std::move(geodata));
        return true;
    }
    qDebug()<<"add failed";
    return false;
}

GeoData* GeoDataModel::getDataByindex(int index)
{
    return m_geoDataList->at(index);
}

int GeoDataModel::dataNum() const
{
    return m_geoDataList->count();
}
