#ifndef OPENCV_H
#define OPENCV_H

#include <QObject>
#include <opencv2/core.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>

#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;
using namespace cv;

class OpenCV : public QObject
{
    Q_OBJECT
public:
    explicit OpenCV(QObject *parent = nullptr);
    void createArucoMarkers();
    void createKnownBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners);
    void getChessBoardCorners(vector<Mat> images, vector<vector<Point2f>>& allFoundCorners, bool showResults = false);
    int startWebcamMonitoring(const Mat& cameraMatrix, const Mat& distanceCoeafficients, float arucoSquareDimension);
    void cameraCalibration(vector<Mat> calibrationsImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix,
                           Mat& distanceCoeafficients);
    bool saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients);
    bool loadCameraCalibration(string name, Mat& cameraMatrix, Mat& distanceCoeffients);
    void cameraCalibrationProcess(Mat& cameraMatrix, Mat& distanceCoefficients);
    void start();
signals:
    void frameChanged(cv::Mat frame);
private:
    //init arguments
    const float calibrationSquareDimention = 0.022f;
    const float arucoSquareDimension = 0.0610f;
    const Size chessboardDimensions = Size(9, 6);
    //frame to qml
    Mat frame;
    //init arguments
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);
    Mat distanceCoefficients;
};

#endif // OPENCV_H
