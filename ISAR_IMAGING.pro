QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    databaseinterface.cpp \
    main.cpp \
    mainwindow.cpp \
    p_display.cpp \
    textslider.cpp

HEADERS += \
    databaseinterface.h \
    mainwindow.h \
    p_display.h \
    textslider.h

FORMS += \
    databaseinterface.ui \
    mainwindow.ui

# resources
TRANSLATIONS += $${PWD}/resources/language/zh_CN.ts
RESOURCES += ISAR_IMAGING.qrc
RC_FILE += $${PWD}/resources/img/ICON.rc

CONFIG += lrelease
CONFIG += embed_translations

# set up qwt
win32 {
    DEFINES += QT_DLL QWT_DLL
}

win32 {
    CONFIG(release, debug|release): LIBS += -L$${PWD}/qwt/lib/ -lqwt
    CONFIG(debug, debug|release): LIBS += -L$${PWD}/qwt/lib/ -lqwtd
}

INCLUDEPATH += $${PWD}/qwt/include
DEPENDPATH += $${PWD}/qwt/lib

# set up cuda
LIBS += -L$${PWD}/cuda/lib/ -lCudaTest

INCLUDEPATH += $${PWD}/cuda/include
DEPENDPATH += $${PWD}/cuda/lib

CONFIG(release, debug|release): {
    BUILD_MODE = release
}
else {
    BUILD_MODE = debug
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
