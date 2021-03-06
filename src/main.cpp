#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "detect_pose.hpp"


using namespace cv;
using namespace std;


int SimpleGrab()
{
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "ERROR: Unable to open the camera" << endl;
        return 0;
    }

    Mat frame;
    cout << "Start grabbing, press a key on Live window to terminate" << endl;
    while(1) {
        cap >> frame;
        if (frame.empty()) {
            cerr << "ERROR: Unable to grab from the camera" << endl;
            break;
        }
        imshow("Live",frame);
        int key = cv::waitKey(5);
        key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
        if (key>=0)
        break;
    }

    cout << "Closing the camera" << endl;
    cap.release();
    destroyAllWindows();
    cout << "bye!" <<endl;
    return 0;
}

int ShowImg(int argc, char** argv)
{

    if ( argc != 2 )
    {
        printf("usage: GyroView <Image_Path>\n");
        return -1;
    }
    Mat image;
    image = imread( argv[1], 1 );
    if ( !image.data )
    {
        printf("No image data \n");
        return -1;
    }
    namedWindow("Gyro View", WINDOW_AUTOSIZE );
    imshow("Gyro View", image);
    waitKey(0);
    return 0;
}

int main(int argc, char** argv )
{


    return ArucoDetect();
}
