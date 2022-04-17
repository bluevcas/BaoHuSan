#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "todolist.h"
#include "todomodel.h"
#include "baidufacedtec.h"
#include "baiduvoice.h"
#include "opencv.h"
#include "threadmanager.h"
#include "cvframe.h"
#include "opencvimageprovider.h"
#include "videostreamer.h"

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterType<ToDoModel>("ToDo", 1, 0, "ToDoModel");
    qmlRegisterUncreatableType<ToDoList>("ToDo", 1, 0, "ToDoList",
         QStringLiteral("ToDoList should not be createrd in QML"));
    qmlRegisterType<CVFrame>("com.app.cvframe", 1, 0, "CVFrame");
    qRegisterMetaType<cv::Mat>("Mat");

    ToDoList toDoList(&app);
    BaiduFaceDtec faceDtec(&app);
    BaiduVoice voiceFuc(&app);
    ThreadManager cvThread(&app);
    VideoStreamer videoStreamer;
    OpencvImageProvider *liveImageProvider(new OpencvImageProvider);

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty(QStringLiteral("toDoList"), &toDoList);
    engine.rootContext()->setContextProperty("faceDtec", &faceDtec);
    engine.rootContext()->setContextProperty("voiceFuc", &voiceFuc);
    engine.rootContext()->setContextProperty("cvThread", &cvThread);

    engine.rootContext()->setContextProperty("VideoStreamer", &videoStreamer);
    engine.rootContext()->setContextProperty("liveImageProvider", liveImageProvider);
    engine.addImageProvider("live", liveImageProvider);


    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
