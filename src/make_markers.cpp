#include <iostream>
#include <fstream>
#include <sstream>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;

const float calibrationSquareDimention = 0.021; // tamaño de cada cuadro del tablero
const float ArucoMarkerSize = 0.015; // tamaño del marcador
const cv::Size chessboardDimensions = cv::Size(7, 9); // Dimensiones del tablero (esquinas internas)


void createKnownBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners)
{
    for (size_t i = 0; i < boardSize.height; i++) {
        for (size_t j = 0; j < boardSize.width; j++) {
                corners.push_back(Point3f(j*squareEdgeLength, i*squareEdgeLength, 0.0f));
        }
    }
}

void getChessboardCorners(vector<Mat> images, vector<vector<Point2f>>& allFoundCorners, bool showResults = false)
{
    for (vector<Mat>::iterator it = images.begin(); it != images.end(); it++) {
        vector<Point2f> pointBuf;
        bool found = findChessboardCorners(*it, Size(9,6), pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            allFoundCorners.push_back(pointBuf);
        }

        if (showResults) {
            drawChessboardCorners(*it, Size(9, 6), pointBuf, found);
            imshow("Calibracion", *it);
            waitKey(0);
        }
    }
}

void CreateArucoMarkers()
{
    Mat outputMarker;
    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);

    for(int i = 0; i < 50; i++) {
        aruco::drawMarker(markerDictionary, i, 500, outputMarker, 1);
        ostringstream convert;
        string imageName = "4x4Marker_";
        convert << imageName << i << ".jpg";
        imwrite(convert.str(), outputMarker);
    }
}

void InteractiveCalibrate() {
    Mat frame;
    Mat drawToFrame;
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

    Mat distanceCoefficients;

    vector<Mat> savedImages;

    vector<vector<Point2f>> markerCorners, rejectedCandidates;

    VideoCapture vid(0);

    if (!vid.isOpened()) {
        return;
    }

    int fps = 20;

    namedWindow("PiCamera", CV_WINDOW_AUTOSIZE);

    while(true) {
        if (!vid.read(frame))
            break;

        vector<Vec2f> foundPoints;
        bool found = false;

        found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE | CV_CALIB_CB_FAST_CHECK); // 
        frame.copyTo(drawToFrame);
        drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);

        if (found) {
            imshow("PiCamera", drawToFrame);

        } else {
            imshow("PiCamera", frame);
        }

        char character = waitKey(1000/fps);

    }

    return;
}

int main(int argc, char** argv )
{
    InteractiveCalibrate();
    return 0;
}
