QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    geodata.cpp \
    geodatamodel.cpp \
    gisdatamanager.cpp \
    main.cpp \
    mainwindow.cpp \
    mapscene.cpp \
    mapview.cpp

HEADERS += \
    geodata.h \
    geodatamodel.h \
    gisdatamanager.h \
    mainwindow.h \
    mapscene.h \
    mapview.h

FORMS += \
    mainwindow.ui

#导入GDAL库
INCLUDEPATH += D:/coding/OSGeo4w/apps/gdal-dev/include
LIBS += -LD:/coding/OSGeo4w/apps/gdal-dev/lib -lgdal_i

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
