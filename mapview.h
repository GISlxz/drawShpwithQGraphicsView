#ifndef MAPVIEW_H
#define MAPVIEW_H

#include <QGraphicsView>
#include <QObject>
#include <QMouseEvent>
#include <QPoint>
#include <QScrollBar>

class MapView : public QGraphicsView
{
    Q_OBJECT

public:
    MapView(QWidget *parent = nullptr);
    ~MapView();

public:
    enum InteractionState {
        None,
        MoveExtent,
        ClickItem
    };

    // QWidget interface
    InteractionState getCurrentState() const;
    void setCurrentState(InteractionState newCurrentState);

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void wheelEvent(QWheelEvent *event);
//    void resizeEvent(QResizeEvent *event);

private:
    InteractionState currentState = MoveExtent; //鼠标状态
    QPoint m_dragStart; //拖动原始点
    bool m_dragging = false;
private:
    void GeoXYtoViewXY();

signals:
    void mouseMovePoint(QPoint point);      //鼠标移动
    void mouseDragMove(QPoint point);   //鼠标拖动
    void mouseWheelMove(float angle); //滚轮

};
#endif // MAPVIEW_H
