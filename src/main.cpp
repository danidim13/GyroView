#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>


using namespace cv;


int Playback()
{
    VideoCapture cap;
    cap.cv::VideoCapture::open(0);

    if (!cap.isOpened())
        return -1;
    Mat edge;//,gray,c_edge,lpf_gaussian_blurr;
    namedWindow("Original_Video",1);

    for(;;)
    {
    	Mat video;
    	cap >> video;//capture live feed

        //convert to b&w
        cvtColor(video,edge,CV_BGR2GRAY);
        //Canny(gray,c_edge,0,30,3);
        //GaussianBlur(video,lpf_gaussian_blurr,Size(9,9),1.5,1.5);

        imshow("Original_Video",edge);

        if(waitKey(30)>= 0) break;
    }
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


    return Playback();
}
