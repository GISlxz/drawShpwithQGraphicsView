#ifndef GEODATA_H
#define GEODATA_H
#include "ogrsf_frmts.h"
#include "gdal_priv.h"

class GeoData
{
public:
    GeoData(const char* fileDir);
    ~GeoData();
    const GDALDatasetUniquePtr& getDataUniquePtr() const;
    bool getNeedDraw() const;
    void setNeedDraw(bool newNeedDraw);

private:
    GDALDatasetUniquePtr m_GDALDatasetUniquePtr;
    bool needDraw;
};

#endif // GEODATA_H
