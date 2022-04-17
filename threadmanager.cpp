#include "threadmanager.h"

ThreadManager::ThreadManager(QObject *parent) : QObject(parent)
{

}

void ThreadManager::runCapture()
{
//    connect(&thread, &QThread::started,
//            &openCV, &OpenCV::start);

//    connect(&thread, &QThread::finished,
//            &openCV, &OpenCV::deleteLater);

////    connect(&openCV, &OpenCV::frameChanged,
////            this, &ThreadManager::receiveFrame);

////    connect(&openCV, &OpenCV::frameChanged,
////            &cvFrame, &CVFrame::setRawFrame);


////    connect(&cvFrame, &CVFrame::frameChanged,
////            this, &ThreadManager::receiveFrame);
//    connect(&videoStreamer,&VideoStreamer::newImage,
//            liveImageProvider,&OpencvImageProvider::updateImage);

//    openCV.moveToThread(&thread);
//    thread.start();

    connect(&thread, &QThread::started,
            &videoStreamer, &VideoStreamer::start);

    connect(&thread, &QThread::finished,
            &videoStreamer, &VideoStreamer::deleteLater);

    connect(&videoStreamer, &VideoStreamer::newImage,
            &opencvImageProvider, &OpencvImageProvider::updateImage);

    videoStreamer.moveToThread(&thread);
    thread.start();
}

void ThreadManager::receiveFrame(QImage frame)
{
    emit(updateView(frame));
}
