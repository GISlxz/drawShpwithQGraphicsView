#ifndef GISDATAMANAGER_H
#define GISDATAMANAGER_H
#include <QList>
#include "ogrsf_frmts.h"
#include "gdal_priv.h"
#include "geodata.h"
#include "mapscene.h"

class GISDataManager
{
public:
    GISDataManager();
    ~GISDataManager();


public:
    bool addGeoData(const char* fileDir);
    bool deleteGeoData();
    GeoData* getDataByindex(int index);

    int dataNum() const;

private:
    QList<GeoData*>* m_geoDataList;

private:
    int m_dataNum = 0;
};

#endif // GISDATAMANAGER_H
