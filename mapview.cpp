#include "mapview.h"
#include    <QMouseEvent>
#include    <QPoint>

MapView::MapView(QWidget *parent):QGraphicsView(parent)
{
}

MapView::~MapView()
{
    qDebug()<<"MapView release";
}

void MapView::mousePressEvent(QMouseEvent *event)
{
    if(currentState == MoveExtent){
        if(event->button() == Qt::LeftButton) {
            m_dragStart = event->pos();
            m_dragging = true;
        }
    }
    event->ignore(); //事件冒泡
}

void MapView::mouseReleaseEvent(QMouseEvent *event){
    if(currentState == MoveExtent){
        if (event->button() == Qt::LeftButton) {
            m_dragging = false;
        }
        QPoint dragMove = event->pos() - m_dragStart;
        emit mouseDragMove(dragMove);
    }
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    QPoint point=event->pos();      //QGraphicsView的坐标
    emit mouseMovePoint(point);     //发射信号
    QGraphicsView::mouseMoveEvent(event);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = -0.001; // 定义缩放的速率
    emit mouseWheelMove(event->angleDelta().y()*scaleFactor);
}

MapView::InteractionState MapView::getCurrentState() const
{
    return currentState;
}

void MapView::setCurrentState(InteractionState newCurrentState)
{
    currentState = newCurrentState;
}
