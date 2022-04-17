#ifndef VIRTUALPAINT_H
#define VIRTUALPAINT_H

#include <QObject>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace cv;
using namespace std;

class VirtualPaint : public QObject
{
    Q_OBJECT
public:
    explicit VirtualPaint(QObject *parent = nullptr);
    Point getContours(Mat image);
    vector<vector<int>> findColor(Mat img);
    void drawOnCanvas(vector<vector<int>> newPoints, vector<Scalar> myColorValues);

signals:

private:
    Mat img;
    VideoCapture cap;
    vector<vector<int>> newPoints;
    vector<vector<int>> myColors{ {124,48,117,143,170,255}, // Purple
                                  {68,72,156,102,126,255} };// Green
    vector<Scalar> myColorValues{ {255,0,255},		// Purple
                                  {0,255,0} };// Green
};

#endif // VIRTUALPAINT_H
