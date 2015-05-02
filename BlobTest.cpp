#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/opencv.hpp"

#define WIDTH 640
#define HEIGHT 480


using namespace cv;
using namespace std;


int main( int argc, char** argv )
{
    int frameNum = 0;

    VideoCapture cap(1);
    cap.set(CV_CAP_PROP_FRAME_WIDTH, WIDTH);
    cap.set(CV_CAP_PROP_FRAME_HEIGHT, HEIGHT);

    cvNamedWindow("Raw Video", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Blob Detection", CV_WINDOW_AUTOSIZE);
    cvNamedWindow("Filtered Video", CV_WINDOW_AUTOSIZE);


    if(!cap.isOpened()) return -1;

    Mat frame, frameHSV, frameThreshold, outFrame;

    SimpleBlobDetector::Params params;

    params.minThreshold = 1;
    params.maxThreshold = 255;
    params.filterByArea = true;
    params.minArea = 500;
    params.maxArea = 70000;
    params.blobColor = 255;
    params.filterByCircularity = false;
    params.filterByConvexity = false;
    params.filterByInertia = false;

    SimpleBlobDetector detector(params);
    vector<KeyPoint> keypoints;

    while(1)
    {
        cap >> frame;

        cvtColor(frame, frame, CV_BGR2HSV);  
        inRange(frame, Scalar(80, 100, 20), Scalar(130, 255, 255), frame);

        detector.detect(frame, keypoints);
        drawKeypoints(frame, keypoints, outFrame, Scalar(0,0,255), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

        if(keypoints.size()>0){
            float x = keypoints[0].pt.x;
            if (x<WIDTH/2){
                cout << "Drive LEFT, " << 100*abs(x-WIDTH/2)/(WIDTH/2) << "%" <<endl;
                line(outFrame, Point(WIDTH/2,HEIGHT-50), Point(x,HEIGHT-50), Scalar(0,0,255), 3, 8);
            }
            else{
                cout << "Drive RIGHT, " << 100*abs(x-WIDTH/2)/(WIDTH/2) << "%" <<endl;
                line(outFrame, Point(WIDTH/2,HEIGHT-50), Point(x,HEIGHT-50), Scalar(0,255,0), 3, 8);
            }
        }

        imshow("Raw Video", frame);
        imshow("Blob Detection", outFrame);

        char c = cvWaitKey(33);
        if( c == 27 ) break;
    }

    cvDestroyWindow("Raw Video");
    cvDestroyWindow("Blob Detection");
    cvDestroyWindow("Filtered Video");

    
};
