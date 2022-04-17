#ifndef CVFrame_H
#define CVFrame_H

#include <QObject>
#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>

#include <opencv2/core.hpp>
#include <opencv2/core/utility.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

using namespace cv;

class CVFrame : public QQuickPaintedItem
{
    Q_OBJECT

    Q_PROPERTY(QImage frame READ getFrame WRITE setFrame NOTIFY frameChanged)

    Mat rawFrame;
    QImage frame;

public:
    CVFrame(QQuickItem *parent = 0);

    void paint(QPainter *painter);
    Mat getRawFrame() const;
    Q_INVOKABLE void setRawFrame(const Mat &value);
    QImage getFrame() const;
    Q_INVOKABLE void setFrame(const QImage &value);

signals:
    void frameChanged(const QImage &value);

public slots:
    void convert2QImage();
};

#endif // CVFrame_H
