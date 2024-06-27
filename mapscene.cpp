#include "mapscene.h"

MapScene::MapScene(QObject *parent)
    : QGraphicsScene{parent}
{

}

MapScene::MapScene(QRectF rect, QObject *parent):QGraphicsScene(rect,parent)
{

}

MapScene::~MapScene()
{
    qDebug()<<"MapScene release";
    delete m_curCS;
}

void MapScene::setCurCS(const GDALDatasetUniquePtr& ptr)
{
    //获取图层
    OGRLayer* poLayer = ptr->GetLayer(0);
    if (poLayer == nullptr) {
        qDebug() << "Layer not found";
        return;
    }
    m_curCS = poLayer->GetSpatialRef();
    if(m_curCS!=nullptr){
        qDebug() << "坐标设置成功";
    }
    else {
        qDebug() << "未知坐标";
    }
}

void MapScene::setEnvelope(const OGREnvelope* const envelope)
{
    if(envelope!=nullptr){
        m_envelope = *envelope;
        qDebug() << "Set Extent: \nMinX=" << m_envelope.MinX << ", MinY=" << m_envelope.MinY;
        qDebug() << ", MaxX=" << m_envelope.MaxX << ", MaxY=" << m_envelope.MaxY;
    }
    else{
        qDebug() << "未知范围";
    }
    //保持横纵缩放一致
    auto rect = this->sceneRect();
    float x_scale = (m_envelope.MaxX-m_envelope.MinX)/(rect.right()-rect.left());
    float y_scale = (m_envelope.MaxY-m_envelope.MinY)/(rect.bottom()-rect.top());
    if(x_scale>y_scale){
        m_envelope.MinY = (m_envelope.MaxY + m_envelope.MinY)/2 - (m_envelope.MaxY - m_envelope.MinY)/2 * x_scale/y_scale;
        m_envelope.MaxY = (m_envelope.MaxY + m_envelope.MinY)/2 + (m_envelope.MaxY - m_envelope.MinY)/2 * x_scale/y_scale;
        m_scale = x_scale;
    }
    else{
        m_envelope.MinX = (m_envelope.MaxX + m_envelope.MinX)/2 - (m_envelope.MaxX - m_envelope.MinX)/2 * y_scale/x_scale;
        m_envelope.MaxX = (m_envelope.MaxX + m_envelope.MinX)/2 + (m_envelope.MaxX - m_envelope.MinX)/2 * y_scale/x_scale;
        m_scale = y_scale;
    }
}

QPointF MapScene::ScenePoint2CSPoint(const QPointF &scenePoint)
{
    auto scene_rect = this->sceneRect();
    float X = (scenePoint.x()-scene_rect.left())/scene_rect.width()*(m_envelope.MaxX-m_envelope.MinX)+m_envelope.MinX;
    float Y = (scene_rect.bottom()-scenePoint.y())/scene_rect.height()*(m_envelope.MaxY-m_envelope.MinY)+m_envelope.MinY;
    return QPointF(X,Y);
}

QPointF MapScene::CSPoint2ScenePoint(const QPointF &cSPoint)
{
    auto scene_rect = this->sceneRect();
    float X = (cSPoint.x()-m_envelope.MinX)/(m_envelope.MaxX-m_envelope.MinX)*scene_rect.width()+scene_rect.left();
    float Y = scene_rect.bottom()-(cSPoint.y()-m_envelope.MinY)/(m_envelope.MaxY-m_envelope.MinY)*scene_rect.height();
    return QPointF(X,Y);
}

QString MapScene::getCSName()
{
    if(m_curCS != nullptr){
        char* pszSRS_WKT = nullptr;
        m_curCS->exportToWkt(&pszSRS_WKT);
        auto coordSystem = QString(pszSRS_WKT);
        // 找到 "GEOGCS[" 的起始位置
        int geogcsIndex = coordSystem.indexOf("GEOGCS[");
        if (geogcsIndex == -1) {
            return QString(); // 如果找不到，返回空字符串
        }

        // 找到第一个双引号的位置
        int firstQuoteIndex = coordSystem.indexOf('"', geogcsIndex);
        if (firstQuoteIndex == -1) {
            return QString(); // 如果找不到，返回空字符串
        }

        // 找到第二个双引号的位置
        int secondQuoteIndex = coordSystem.indexOf('"', firstQuoteIndex + 1);
        if (secondQuoteIndex == -1) {
            return QString(); // 如果找不到，返回空字符串
        }

        // 提取双引号之间的内容
        QString coordName = coordSystem.mid(firstQuoteIndex + 1, secondQuoteIndex - firstQuoteIndex - 1);

        return coordName;
    }
    else{
        qDebug() << "未知坐标系";
        return nullptr;
    }
}

void MapScene::drawPolygons(GeoData *geodata)
{
    OGRLayer* poLayer = geodata->getDataUniquePtr()->GetLayer(0);
    if (poLayer == nullptr) {
        qDebug() << "无效图层";
        return;
    }
    for(auto& poFeature:poLayer)
        addPolygontoScene(poFeature);
}

void MapScene::addPolygontoScene(OGRFeatureUniquePtr &tarPolygon)
{
    QPolygonF polygon;
    auto poGeometry = tarPolygon->GetGeometryRef();
    if (poGeometry != nullptr && wkbFlatten(poGeometry->getGeometryType()) == wkbPolygon) {
        // 输出多边形的顶点
        OGRPolygon* poPolygon = static_cast<OGRPolygon*>(poGeometry);
        OGRLinearRing* poExteriorRing = poPolygon->getExteriorRing();
        if (poExteriorRing != nullptr) {
            int nVertices = poExteriorRing->getNumPoints();
            for (int i = 0; i < nVertices; i++) {
                polygon<<CSPoint2ScenePoint({poExteriorRing->getX(i),poExteriorRing->getY(i)});
            }
        }
    }
    QGraphicsPolygonItem* polygonItem = new QGraphicsPolygonItem(polygon);
    this->addItem(polygonItem);
}

void MapScene::moveExtent(QPoint movePoint)
{
    m_envelope.MaxX -= movePoint.x()*m_scale;
    m_envelope.MaxY += movePoint.y()*m_scale;
    m_envelope.MinX -= movePoint.x()*m_scale;
    m_envelope.MinY += movePoint.y()*m_scale;
}

void MapScene::changeScale(float ratio)
{
    m_scale = m_scale*(ratio);
    m_envelope.MinY = (m_envelope.MaxY + m_envelope.MinY)/2 - (m_envelope.MaxY - m_envelope.MinY)/2 * (ratio);
    m_envelope.MaxY = (m_envelope.MaxY + m_envelope.MinY)/2 + (m_envelope.MaxY - m_envelope.MinY)/2 * (ratio);
    m_envelope.MinX = (m_envelope.MaxX + m_envelope.MinX)/2 - (m_envelope.MaxX - m_envelope.MinX)/2 * (ratio);
    m_envelope.MaxX = (m_envelope.MaxX + m_envelope.MinX)/2 + (m_envelope.MaxX - m_envelope.MinX)/2 * (ratio);
}

float MapScene::scale() const
{
    return m_scale;
}

void MapScene::setScale(float newScale)
{
    m_scale = newScale;
}
