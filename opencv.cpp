#include "opencv.h"

OpenCV::OpenCV(QObject *parent)
    : QObject{parent}
{

}

void OpenCV::createArucoMarkers()
{
    Mat outputMarker;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

    for(int i = 0; i < 50; i++)
    {
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert;
        string imageName = "4x4Marker_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }
}

void OpenCV::createKnownBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f> &corners)
{
    for(int i = 0; i < boardSize.height; i++)
    {
        for(int j = 0; j < boardSize.width; j++)
        {
            corners.push_back(Point3f(Point3f(j * squareEdgeLength, i * squareEdgeLength, 0.0f)));
        }
    }
}

void OpenCV::getChessBoardCorners(vector<Mat> images, vector<vector<Point2f> > &allFoundCorners, bool showResults)
{
    for(vector<Mat>::iterator iter = images.begin();iter != images.end(); iter++)
    {
        vector<Point2f> pointBuf;
        bool found = findChessboardCorners(*iter, Size(9, 6), pointBuf, cv::CALIB_CB_ADAPTIVE_THRESH
                                           | cv::CALIB_CB_NORMALIZE_IMAGE | cv::CALIB_CB_FAST_CHECK);
        if(found)
        {
            allFoundCorners.push_back(pointBuf);
        }
        if(showResults)
        {
            drawChessboardCorners(*iter, Size(9, 6), pointBuf, found);
            imshow("Looking for Corners", *iter);
            waitKey(0);
        }
    }
}

int OpenCV::startWebcamMonitoring(const Mat &cameraMatrix, const Mat &distanceCoeafficients, float arucoSquareDimension)
{
    Mat frame;

    vector<int> markerIds;
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    aruco::DetectorParameters parameters;

    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

    VideoCapture vid(0);  //设置监测摄像头

    if(!vid.isOpened())
    {
        return -1;
    }

    //namedWindow("Webcam", WINDOW_AUTOSIZE);

    vector<Point2f> image_points;
    vector<Vec3d> rotationVectors, translationVectors;

    while(true)
    {
        if(!vid.read(frame))
            break;

        aruco::detectMarkers(frame, markerDictionary, markerCorners, markerIds);
        //打印markerCorners信息
        for (int i = 0;i < markerIds.size(); i++)
        {
            //cout << markerIds[i];
            Point2f one_img_pt(markerCorners[i][0].x, markerCorners[i][0].y),
                    two_img_pt(markerCorners[i][1].x, markerCorners[i][1].y),
                    three_img_pt(markerCorners[i][2].x, markerCorners[i][2].y),
                    four_img_pt(markerCorners[i][3].x, markerCorners[i][3].y);

            //            Point3f one_obj_pt(5, 5, 0),
            //                    two_obj_pt(0, 5, 0),
            //                    three_obj_pt(0, 0, 0),
            //                    four_obj_pt(5, 0, 0);

            image_points.push_back(one_img_pt);
            image_points.push_back(two_img_pt);
            image_points.push_back(three_img_pt);
            image_points.push_back(four_img_pt);

            //            object_points.push_back(one_obj_pt);
            //            object_points.push_back(two_obj_pt);
            //            object_points.push_back(three_obj_pt);
            //            object_points.push_back(four_obj_pt);
        }


        aruco::estimatePoseSingleMarkers(markerCorners, arucoSquareDimension, cameraMatrix, distanceCoeafficients, rotationVectors, translationVectors);

        for(int i = 0; i < markerIds.size(); i++)
        {
            aruco::drawAxis(frame, cameraMatrix, distanceCoeafficients, rotationVectors[i], translationVectors[i], 0.1f);
            //            for (vector<Vec3d>::const_iterator l = rotationVectors.begin(); l != rotationVectors.end(); ++l) {
            //                cout << "rotationVectors" << *l << '\n';
            //            }
            //            for (vector<Vec3d>::const_iterator l = translationVectors.begin(); l != translationVectors.end(); ++l) {
            //                cout << "translationVectors" << *l << '\n';
            //            }
            //drawCube
            //            Cube cube;
            //            Mat rvecs(3,1,DataType<double>::type);
            //            Mat tvecs(3,1,DataType<double>::type);
            //            frame = cube.drawcube(frame, cameraMatrix, distanceCoeafficients, rvecs, tvecs, image_points);
        }

        //imshow("Webcam", frame);
        emit frameChanged(frame);
        if(waitKey(30) >= 0) break;
    }
    return 1;
}

void OpenCV::cameraCalibration(vector<Mat> calibrationsImages, Size boardSize, float squareEdgeLength, Mat &cameraMatrix, Mat &distanceCoeafficients)
{
    vector<vector<Point2f>> checkerboardImageSpacePoint;
    getChessBoardCorners(calibrationsImages, checkerboardImageSpacePoint, false);

    vector<vector<Point3f>> worldSpaceCornerPoints(1);

    createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(checkerboardImageSpacePoint.size(), worldSpaceCornerPoints[0]);

    vector<Mat> rVectors, tVectors;
    distanceCoeafficients = Mat::zeros(8 ,1, CV_64F);

    calibrateCamera(worldSpaceCornerPoints, checkerboardImageSpacePoint, boardSize, cameraMatrix, distanceCoeafficients, rVectors, tVectors);
}

bool OpenCV::saveCameraCalibration(string name, Mat cameraMatrix, Mat distanceCoefficients)
{
    ofstream outStream(name);
    if(outStream)
    {
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns = cameraMatrix.cols;

        outStream << rows << endl;
        outStream << columns << endl;

        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < columns; c++)
            {
                double value = cameraMatrix.at<double>(r, c);
                outStream << value << endl;
            }
        }

        rows = distanceCoefficients.rows;
        columns = distanceCoefficients.cols;

        outStream << rows << endl;
        outStream << columns << endl;

        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < columns; c++)
            {
                double value = distanceCoefficients.at<double>(r, c);
                outStream << value << endl;
            }
        }

        outStream.close();
        return true;
    }

    return 0;
}

bool OpenCV::loadCameraCalibration(string name, Mat &cameraMatrix, Mat &distanceCoeffients)
{
    ifstream inStream(name);
    if(inStream)
    {
        uint16_t rows;
        uint16_t columns;

        inStream >> rows;
        inStream >> columns;

        cameraMatrix = Mat(Size(columns, rows), CV_64F);

        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < columns; c++)
            {
                double read = 0.0f;
                inStream >> read;
                cameraMatrix.at<double>(r, c) = read;
                cout << cameraMatrix.at<double>(r, c) << "\n";
            }
        }

        //Distance Coefficients
        inStream >> rows;
        inStream >> columns;

        distanceCoeffients = Mat::zeros(rows, columns, CV_64F);

        for(int r = 0; r < rows; r++)
        {
            for(int c = 0; c < columns; c++)
            {
                double read = 0.0f;
                inStream >> read;
                distanceCoeffients.at<double>(r, c) = read;
                cout << distanceCoeffients.at<double>(r, c) << "\n";
            }
        }
        inStream.close();
        return true;
    }
    return false;
}

void OpenCV::cameraCalibrationProcess(Mat &cameraMatrix, Mat &distanceCoefficients)
{
    Mat frame;
    Mat drawToFrame;
    vector<Mat> savedImages;

    vector<vector<Point2f>> markerCorners, rejectedCadidates;

    VideoCapture vid(1);

    if(!vid.isOpened())
    {
        return;
    }

    int framesPerSecond = 20;

    namedWindow("Webcam", WINDOW_AUTOSIZE);

    while(true)
    {
        if(!vid.read(frame))
            break;

        vector<Vec2f> foundPoints;
        bool found = false;

        found = findChessboardCorners(frame, chessboardDimensions, foundPoints, cv::CALIB_CB_ADAPTIVE_THRESH
                                      | cv::CALIB_CB_NORMALIZE_IMAGE);
        frame.copyTo(drawToFrame);

        drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);

        if(found)
            imshow("Webcam", drawToFrame);
        else
            imshow("Webcam", frame);
        char character = waitKey(1000 / framesPerSecond);

        switch(character)
        {
        case ' ':
            //saving image
            if(found)
            {
                Mat temp;
                frame.copyTo(temp);
                savedImages.push_back(temp);
            }
            break;
        case 13:
            //start calibration
            if(savedImages.size() > 15)
            {
                cameraCalibration(savedImages, chessboardDimensions, calibrationSquareDimention, cameraMatrix, distanceCoefficients);
                saveCameraCalibration("MyCameraCalibration", cameraMatrix, distanceCoefficients);
            }
            break;
        case 27:
            //exit
            return;
            break;
        }
    }
}

void OpenCV::start()
{
    //相机校准
    //cameraCalibrationProcess(cameraMatrix, distanceCoefficients);
    loadCameraCalibration("D:/BaoHuSan/MyCameraCalibration", cameraMatrix, distanceCoefficients);
    startWebcamMonitoring(cameraMatrix, distanceCoefficients, arucoSquareDimension);
}
