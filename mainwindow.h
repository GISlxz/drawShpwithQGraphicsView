#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "mapscene.h"
#include "gisdatamanager.h"
#include "mapview.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;

    MapView* m_MapView;   //view
    MapScene  *m_scene;     //场景
    GISDataManager *m_GISDataManager;

private:
    void drawAllData();

private:
    void do_mouseMovePoint(QPoint point);
    void do_mouseDragPoint(QPoint point);
    void do_mouseWheelMove(float angle);

private:
    QLabel  *labCurCS; //当前坐标系
    QLabel  *labViewCord; //当前view坐标
    QLabel  *labSceneCord; //当前scene坐标
    QLabel  *labScale;
    QLabel  *labCSCord; //当前地理坐标
    QLabel  *labItemCord; //当前要素坐标
    QLabel  *labItemInfo; //当前要素信息

    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event);
};
#endif // MAINWINDOW_H
