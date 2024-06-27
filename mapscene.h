#ifndef MAPSCENE_H
#define MAPSCENE_H

#include <QGraphicsScene>
#include <QObject>
#include "ogrsf_frmts.h"
#include "gdal_priv.h"
#include <QString>
#include <QDebug>
#include <QGraphicsPolygonItem>
#include <QPolygonF>
#include "geodata.h"

class MapScene : public QGraphicsScene
{
public:
    explicit MapScene(QObject *parent = nullptr);
    explicit MapScene(QRectF rect , QObject *parent = nullptr);
    ~MapScene();

    void setCurCS(const GDALDatasetUniquePtr& ptr);
    void setEnvelope(const OGREnvelope* const envelope);
    void setEnvelope(float minX,float minY,float maxX,float maxY);
    QPointF ScenePoint2CSPoint(const QPointF& scenePoint);
    QPointF CSPoint2ScenePoint(const QPointF& cSPoint);
    QString getCSName();
    void drawPolygons(GeoData* geodata);
    void addPolygontoScene(OGRFeatureUniquePtr& tarPolygon);
    void moveExtent(QPoint movePoint);
    void changeScale(float ratio);
    float scale() const;
    void setScale(float newScale);

private:
    OGRSpatialReference* m_curCS; //当前坐标系
    OGREnvelope m_envelope; //当前显示范围对应地理范围
    float m_scale; //缩放比
};


#endif // MAPSCENE_H
