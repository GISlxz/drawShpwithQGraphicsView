#ifndef GEODATAMODEL_H
#define GEODATAMODEL_H

#include <QAbstractListModel>
#include <QObject>
#include <QList>
#include "ogrsf_frmts.h"
#include "gdal_priv.h"
#include "geodata.h"
#include "mapscene.h"

class GeoDataModel : public QAbstractListModel
{
public:
    explicit GeoDataModel(QObject *parent = nullptr);
    ~GeoDataModel();

public:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

protected:
    QHash<int, QByteArray> roleNames() const override;

public:
    bool addGeoData(QString fileDir);
    bool deleteGeoData();
    GeoData* getDataByindex(int index);

    int dataNum() const;

private:
    QList<GeoData*>* m_geoDataList;

};

#endif // GEODATAMODEL_H
