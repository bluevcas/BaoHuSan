#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <QObject>
#include <QThread>
#include "opencv.h"
#include "cvframe.h"
#include "opencvimageprovider.h"
#include "videostreamer.h"

class ThreadManager : public QObject
{
    Q_OBJECT

    QThread thread;
//    OpenCV openCV;
//    CVFrame cvFrame;
    VideoStreamer videoStreamer;
    OpencvImageProvider opencvImageProvider;

public:
    explicit ThreadManager(QObject *parent = nullptr);

    Q_INVOKABLE void runCapture();

signals:

    void updateView(QImage frame);

public slots:

    void receiveFrame(QImage frame);

};

#endif // THREADMANAGER_H
