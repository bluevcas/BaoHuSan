#include "cvframe.h"

Mat CVFrame::getRawFrame() const
{
    return rawFrame;
}

void CVFrame::setRawFrame(const Mat &value)
{
    rawFrame = value;
    convert2QImage();
}

QImage CVFrame::getFrame() const
{
    return frame;
}

void CVFrame::setFrame(const QImage &value)
{
    frame = value;
    CVFrame::update();
    emit(frameChanged(frame));
}

void CVFrame::convert2QImage()
{
    Mat tempMat;
    cvtColor(rawFrame, tempMat, COLOR_RGB2BGR);
    QImage tempImage((uchar*) tempMat.data, tempMat.cols,tempMat.rows, tempMat.step, QImage::Format_RGB888);

    frame = tempImage;
    frame.detach();
    CVFrame::update();
    emit(frameChanged(frame));
}

CVFrame::CVFrame(QQuickItem *parent): QQuickPaintedItem(parent)
{

}

void CVFrame::paint(QPainter *painter)
{
    frame.scaled(1000, 800, Qt::IgnoreAspectRatio, Qt::FastTransformation);
    painter->drawImage(0, 0, frame, 0, 0, -1, -1, Qt::AutoColor);
}
