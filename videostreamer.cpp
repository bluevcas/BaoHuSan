#include "videostreamer.h"

VideoStreamer::VideoStreamer()
{
    connect(&tUpdate,&QTimer::timeout,this,&VideoStreamer::streamVideo);

}

VideoStreamer::~VideoStreamer()
{
    cap.release();
    tUpdate.stop();
}

void VideoStreamer::streamVideo()
{
    cap>>frame;

    //图片处理代码


    QImage img = QImage(frame.data,frame.cols,frame.rows,QImage::Format_RGB888).rgbSwapped();
    emit newImage(img);
}

void VideoStreamer::start()
{
    openVideoCamera();
}

void VideoStreamer::openVideoCamera()
{
    cap.open(1);

    //double fps = cap.get(cv::CAP_PROP_FPS);
    //tUpdate.start(1000/fps);
    tUpdate.start(5);
}
