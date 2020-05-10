//insert your code in part_1a/p1a_code.cpp
//edit the file extension and web template to match your programing language

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    VideoCapture video("/Users/kskrueger/CLionProjects/CV/src/cs575/HW3-2020/part_1a/p1a_tetris_1.mp4");

    Mat frame;
    while (true) {
        video >> frame;
        if (frame.empty()) break;
        Mat grey;
        cvtColor(frame, grey, COLOR_BGR2GRAY);
        imshow("Greyscale Out", grey);
        waitKey(1);
    }
    return 0;
}
