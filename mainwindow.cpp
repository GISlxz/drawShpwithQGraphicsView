#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //------------------------初始化GDAL--------------------------------
    //设置proj库地址
    const char* proj_path = "D:/coding/OSGeo4w/share/proj";
    CPLSetConfigOption("PROJ_LIB", proj_path);
    //初始化矢量数据
    //支持中文路径
    CPLSetConfigOption("GDAL_FILENAME_IS_UTF8", "YES");
    //属性表字符编码
    CPLSetConfigOption("SHAPE_ENCODING", "");
    OGRRegisterAll();

    //------------------------初始化（组件及容器）------------------------
    //------------------------读取矢量数据（测试）--------------------------------
    m_GISDataManager = new GISDataManager();
    QRectF rect(0,0,600,400);
    m_scene = new MapScene(rect,this);
    ui->view->setScene((QGraphicsScene*)m_scene);

    QString shpFileDIR1 = "E:/课题组/数据/垦利区/target_区域.shp";
    QString shpFileDIR2 = "E:/课题组/数据/东营市/山东省_东营市_全市水体(河流、人工湖、水渠等)分布矢量shp";
    m_GISDataManager->addGeoData(shpFileDIR1.toUtf8().constData());
    m_GISDataManager->addGeoData(shpFileDIR2.toUtf8().constData());
    m_scene->setCurCS(m_GISDataManager->getDataByindex(0)->getDataUniquePtr());
    OGREnvelope envelope;
    m_GISDataManager->getDataByindex(0)->getDataUniquePtr()->GetLayer(0)->GetExtent(&envelope);
    m_scene->setEnvelope(&envelope);

    //绘制所有多边形
    drawAllData();

    //设置底部状态栏
    //坐标系显示
    labCurCS = new QLabel("当前坐标系："+m_scene->getCSName());
    labCurCS->setMinimumWidth(300);
    ui->statusbar->addWidget(labCurCS);
    //坐标显示
    labViewCord=new QLabel("View 坐标："); //创建状态栏上的标签
    labViewCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labViewCord);

    labSceneCord=new QLabel("Scene 坐标：");
    labSceneCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labSceneCord);

    labScale=new QLabel();
    labScale->setText(QString::asprintf("缩放比：%.6f",m_scene->scale()));
    labScale->setMinimumWidth(150);
    ui->statusbar->addWidget(labScale);

    labItemCord=new QLabel("Item 坐标：");
    labItemCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labItemCord);

    labCSCord=new QLabel("CS 坐标：");
    labCSCord->setMinimumWidth(150);
    ui->statusbar->addWidget(labCSCord);

    ui->view->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform); // 启用抗锯齿和平滑的像素转换
    ui->view->setCursor(Qt::OpenHandCursor);   //设置鼠标光标
    ui->view->setMouseTracking(true);       //设置鼠标跟踪
    ui->view->setDragMode(QGraphicsView::RubberBandDrag);   //设置拖动模式

    connect(ui->view,&MapView::mouseMovePoint,this, &MainWindow::do_mouseMovePoint);
    connect(ui->view,&MapView::mouseDragMove,this, &MainWindow::do_mouseDragPoint);
    connect(ui->view,&MapView::mouseWheelMove,this, &MainWindow::do_mouseWheelMove);
}

MainWindow::~MainWindow()
{
    delete m_GISDataManager;
    delete ui;
}

void MainWindow::drawAllData()
{
    for(int index = 0;index<m_GISDataManager->dataNum();index++){
        auto data = m_GISDataManager->getDataByindex(index);
        if(data->getNeedDraw()){
            m_scene->drawPolygons(data);
        }
    }
}

//鼠标移动，point是视图的坐标,物理坐标
void MainWindow::do_mouseMovePoint(QPoint point)
{
    labViewCord->setText(QString::asprintf("View 坐标：%d,%d", point.x(),point.y()));
    QPointF pointScene=ui->view->mapToScene(point);     //转换到Scene坐标
    labSceneCord->setText(QString::asprintf("Scene 坐标：%.0f,%.0f", pointScene.x(),pointScene.y()));
    QPointF pointCS = m_scene->ScenePoint2CSPoint(pointScene);
    labCSCord->setText(QString::asprintf("CS 坐标：%.2f,%.2f", pointCS.x(),pointCS.y()));
}

void MainWindow::do_mouseDragPoint(QPoint point)
{
    ui->view->setCursor(Qt::OpenHandCursor);
    m_scene->moveExtent(point);
    m_scene->clear();
    drawAllData();
}

void MainWindow::do_mouseWheelMove(float angle)
{
    m_scene->changeScale(1+angle);
    m_scene->clear();
    drawAllData();
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if(ui->view->getCurrentState() == MapView::MoveExtent){
        ui->view->setCursor(Qt::ClosedHandCursor);
    }
}


