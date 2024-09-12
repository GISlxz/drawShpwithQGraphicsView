#ifndef GEODATA_H
#define GEODATA_H
#include "ogrsf_frmts.h"
#include "gdal_priv.h"
#include <QString>

class GeoData
{
public:
    GeoData(const char* fileDir);
    ~GeoData();
    const GDALDatasetUniquePtr& getDataUniquePtr() const;
    bool getNeedDraw() const;
    void setNeedDraw(bool newNeedDraw);

    QString getName() const;
    void setName(const QString &newName);
    QString getLocation() const;
    void setLocation(const QString &newLocation);

private:
    QString extractFileName(const char* filePath);

private:
    GDALDatasetUniquePtr m_GDALDatasetUniquePtr;
    bool needDraw;
    QString name;
    QString location; //文件位置
};

#endif // GEODATA_H
