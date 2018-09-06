#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include <opencv2/opencv.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/aruco.hpp>
#include <opencv2/calib3d.hpp>

using namespace cv;
using namespace std;


const float calibrationSquareDimention = 0.02233; // tamaño de cada cuadro del tablero
// const float ArucoMarkerSize = 0.02236; // tamaño del marcador
const cv::Size chessboardDimensions = cv::Size(7, 9); // Dimensiones del tablero (esquinas internas)


void createKnownBoardPosition(Size boardSize, float squareEdgeLength, vector<Point3f>& corners)
{

    for (size_t i = 0; i < boardSize.height; i++) {
        for (size_t j = 0; j < boardSize.width; j++) {
                corners.push_back(Point3f(j*squareEdgeLength, i*squareEdgeLength, 0.0f));
        }
    }
}

void getChessboardCorners(vector<Mat> images, Size boardSize, vector<vector<Point2f>>& allFoundCorners, bool showResults = false)
{
    for (vector<Mat>::iterator it = images.begin(); it != images.end(); it++) {
        vector<Point2f> pointBuf;
        bool found = findChessboardCorners(*it, boardSize, pointBuf, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE);

        if (found) {
            allFoundCorners.push_back(pointBuf);
        }

        if (showResults) {
            drawChessboardCorners(*it, boardSize, pointBuf, found);
            imshow("Calibracion", *it);
            waitKey(0);
        }
    }
}


int calibrateFromFolder(String path)
{

}

void cameraCalibration(vector<Mat> calibrationImages, Size boardSize, float squareEdgeLength, Mat& cameraMatrix, Mat& distanceCoef)
{

    // Aquí guardamos los puntos detectados por la cámara (2D)
    vector<vector<Point2f>> chessboardImageSpacePoints;
    getChessboardCorners(calibrationImages, boardSize, chessboardImageSpacePoints, false);

    // Aquí guardamos la posición esperada de las esquinas del tablero (3d)
    vector<vector<Point3f>> worldSpaceCornerPoints(1);
    createKnownBoardPosition(boardSize, squareEdgeLength, worldSpaceCornerPoints[0]);
    worldSpaceCornerPoints.resize(chessboardImageSpacePoints.size(), worldSpaceCornerPoints[0]);

    // Finalmente se obtienen los parámetros de la cámara
    vector<Mat> rVectors, tVectors;
    distanceCoef = Mat::zeros(8, 1, CV_64F);
    calibrateCamera(worldSpaceCornerPoints, chessboardImageSpacePoints, boardSize, cameraMatrix, distanceCoef, rVectors, tVectors);

}


bool SaveCalibration (string name, Mat cameraMatrix, Mat distanceCoef)
{
    ofstream outStream(name);
    if (outStream) {
        uint16_t rows = cameraMatrix.rows;
        uint16_t columns = cameraMatrix.cols;

        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < columns; c++) {
                double value = cameraMatrix.at<double>(r, c);
                outStream << value << endl;
            }
        }

        rows = distanceCoef.rows;
        columns = distanceCoef.cols;

        for (size_t r = 0; r < rows; r++) {
            for (size_t c = 0; c < columns; c++) {
                double value = distanceCoef.at<double>(r, c);
                outStream << value << endl;
            }
        }

        outStream.close();
        return true;
    }
    return false;
}


void InteractiveCalibrate() {
    Mat frame;
    Mat drawToFrame;
    Mat cameraMatrix = Mat::eye(3, 3, CV_64F);

    Mat distanceCoef;

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

        found = findChessboardCorners(frame, chessboardDimensions, foundPoints, CV_CALIB_CB_ADAPTIVE_THRESH | CV_CALIB_CB_NORMALIZE_IMAGE ); // | CV_CALIB_CB_FAST_CHECK
        frame.copyTo(drawToFrame);
        drawChessboardCorners(drawToFrame, chessboardDimensions, foundPoints, found);

        if (found) {
            imshow("PiCamera", drawToFrame);

        } else {
            imshow("PiCamera", frame);
        }

        char character = waitKey(1000/fps);

        switch (character) {
            case -1:
                // Nothing was pressed
                break;
            case ' ':
                // save image
                if (found) {
                    Mat tmp;
                    frame.copyTo(tmp);
                    savedImages.push_back(tmp);
                } else {
                    cerr << "No chessboard was found" << endl;
                }
                break;
            case 10:
                // start calibration
                if (savedImages.size() > 15) {
                    cameraCalibration(savedImages, chessboardDimensions, calibrationSquareDimention, cameraMatrix, distanceCoef);
                    SaveCalibration("calibration.txt", cameraMatrix, distanceCoef);
                } else {
                    cerr << "No enough images were saved, at least 15 required (" << savedImages.size() << " saved)." << endl;
                }
                break;
            case 27:
                // exit
                return;
                break;
            default:
                cerr << "Unspecified keycode: " << (int) character << endl;
                break;
        }
    }

    return;
}

int main(int argc, char** argv )
{
    InteractiveCalibrate();
    return 0;
}
