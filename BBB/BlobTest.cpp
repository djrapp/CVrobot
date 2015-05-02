#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#define WIDTH 320
#define HEIGHT 240

// proportional and derivative constants
#define KP 0.5
#define KD 1

using namespace cv;
using namespace std;

int main( int argc, char** argv )
{
    int frameNum = 0;

    VideoCapture cap(0);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);

    if(!cap.isOpened()){
        cout << "No camera attached, terminating execution" << endl;
        return -1;
    }

    SimpleBlobDetector::Params params;

    Mat frame, frameHSV, frameThreshold, outFrame;

    params.minThreshold = 1;
    params.maxThreshold = 255;
    params.filterByArea = true;
    params.minArea = 500;
    params.maxArea = 70000;
    params.blobColor = 255;
    params.filterByCircularity = false;
    params.filterByConvexity = false;
    params.filterByInertia = false;

    Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
    vector<KeyPoint> keypoints;

    float PV = 0, error = 0, prevError = 0;

    while(1)
    {
        cout << "Capturing" << endl;
        cap >> frame;
        cvtColor(frame, frameHSV, CV_BGR2HSV);  
        inRange(frameHSV, Scalar(80, 100, 30), Scalar(130, 255, 255), frameThreshold);
        detector->detect(frameThreshold, keypoints);
        if(keypoints.size()>0){
            float x = keypoints[0].pt.x;
            error = 100*(x-WIDTH/2)/(WIDTH/2);
            PV = KP * error + KD * (error - prevError);
            prevError = error;
            cout << "PV: " << PV << endl;
           /* if (x<WIDTH/2){
                cout << "Drive LEFT, " << 100*abs(x-WIDTH/2)/(WIDTH/2) << "%" <<endl;
            }
            else{
                cout << "Drive RIGHT, " << 100*abs(x-WIDTH/2)/(WIDTH/2) << "%" <<endl;
            }*/
        }
    }

};