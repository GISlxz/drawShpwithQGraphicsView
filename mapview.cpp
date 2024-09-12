#include "mapview.h"

MapView::MapView(QWidget *parent):QGraphicsView(parent)
{
    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); // 启用抗锯齿和平滑的像素转换
    setCursor(Qt::OpenHandCursor);   //设置鼠标光标
    setMouseTracking(true);       //设置鼠标跟踪
    setDragMode(ScrollHandDrag);   //设置拖动模式
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
    QGraphicsView::mousePressEvent(event);
}

void MapView::mouseReleaseEvent(QMouseEvent *event){
    if(currentState == MoveExtent){
        if (event->button() == Qt::LeftButton) {
            m_dragging = false;
        }
        QPoint dragMove = event->pos() - m_dragStart;
        emit mouseDragMove(dragMove);
    }
    QGraphicsView::mouseReleaseEvent(event);
}

void MapView::mouseMoveEvent(QMouseEvent *event)
{
    if(m_dragging){
        QPoint point=event->pos();      //QGraphicsView的坐标
        auto delta = point - m_dragStart;
        m_dragStart = event->pos();
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - delta.x());
        verticalScrollBar()->setValue(verticalScrollBar()->value() - delta.y());
        emit mouseMovePoint(point);     //发射信号
    }
    QGraphicsView::mouseMoveEvent(event);
}

void MapView::wheelEvent(QWheelEvent *event)
{
    const double scaleFactor = 0.001; // 定义缩放的速率
    auto angle = event->angleDelta().y()*scaleFactor;
    scale(1+angle, 1+angle);
    //emit mouseWheelMove(event->angleDelta().y()*scaleFactor);
}

MapView::InteractionState MapView::getCurrentState() const
{
    return currentState;
}

void MapView::setCurrentState(InteractionState newCurrentState)
{
    currentState = newCurrentState;
}
