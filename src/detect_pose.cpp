
#include "detect_pose.hpp"
#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/aruco.hpp>


using namespace cv;
using namespace std;


const float MARKER_SIZE = 2.1f;

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

int ArucoDetect() {
    
    // Diccionario de marcadores usado
    Ptr<aruco::Dictionary> markerDictionary = aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME::DICT_4X4_50);
    // Id de los marcadores reconocidos
    vector<int> markerIds;
    // Esquinas de los marcadores identificados
    vector<vector<Point2f>> markerCorners, rejectedCandidates;
    // Parámetros para detección
    Ptr<aruco::DetectorParameters> parameters;

    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "ERROR: No se pudo acceder a la cámara" << endl;
        return 1;
    }

    Mat iFrame, oFrame;
    cout << "Procesando video, presione una tecla para terminar" << endl;

    while (true) {
        
        cap >> iFrame;
        
        if (iFrame.empty()) {
            cerr << "ERROR: No se obtuvo respuesta de la cámara." << endl;
            break;
        }

        aruco::detectMarkers(iFrame, markerDictionary, markerCorners, markerIds);
        
        if (markerIds.size() > 0)
        {
            aruco::drawDetectedMarkers(iFrame, markerCorners, markerIds);

            vector<Mat> rVectors, tVectors;

            //aruco::estimatePoseSingleMarkers(markerCorners, MARKER_SIZE, rVectors, tVectors);

        }
        imshow("GyroView", iFrame);

        int key = cv::waitKey(5);
        key = (key==255) ? -1 : key; //#Solve bug in 3.2.0
        if (key>=0)
            break;        

    }
    return 0;
}