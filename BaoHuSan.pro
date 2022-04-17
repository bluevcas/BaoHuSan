QT += quick multimedia network core gui qml network

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        baidufacedtec.cpp \
        baiduvoice.cpp \
        cvframe.cpp \
        main.cpp \
        opencv.cpp \
        opencvimageprovider.cpp \
        threadmanager.cpp \
        todolist.cpp \
        todomodel.cpp \
        tulinrobot.cpp \
        videostreamer.cpp \
        virtualpaint.cpp

RESOURCES += qml.qrc \
    assets.qrc \
    images.qrc

# Additional import path used to resolve QML module s in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

#include openCV4
INCLUDEPATH += E:\CEXTENDAPP\openCV\mingw73contrib\include
LIBS += E:\CEXTENDAPP\openCV\mingw73contrib\x64\mingw\lib\libopencv_*.a

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += targetports/LottieAnimation/LottieAnimation.qml

HEADERS += \
    baidufacedtec.h \
    baiduvoice.h \
    cvframe.h \
    opencv.h \
    opencvimageprovider.h \
    threadmanager.h \
    todolist.h \
    todomodel.h \
    tulinrobot.h \
    videostreamer.h \
    virtualpaint.h
