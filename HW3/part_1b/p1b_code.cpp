//insert your code in part_1b/p1b_code.cpp
//edit the file extension and web template to match your programing language

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int colorCount(const Mat& image, const Scalar& color);

int main() {
    VideoCapture video("/Users/kskrueger/CLionProjects/CV/src/cs575/HW3-2020/part_1b/p1b_tetris_1.mp4");

    namedWindow("Frame", WINDOW_FREERATIO);

    Mat frame;
    while (true) {
        video >> frame;
        if (frame.empty()) break;
        int red = colorCount(frame, Scalar(0, 32, 235)); // rgb(230, 32, 0)
        int orange = colorCount(frame, Scalar(0, 163, 239)); // rgb(239, 163, 0)
        int yellow = colorCount(frame, Scalar(0, 251, 251)); // rgb(251, 251, 0)
        int green = colorCount(frame, Scalar(1, 145, 35)); // rgb(35, 145, 1)
        int cyan = colorCount(frame, Scalar(250, 250, 35)); // rgb(74, 253, 255)
        int dark_blue = colorCount(frame, Scalar(248, 33, 36)); // rgb(36, 33, 248)
        int purple = colorCount(frame, Scalar(123, 20, 116)); // rgb(116, 20, 123)

        putText(frame, "Red: " + to_string(red), Point(20, 40), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 32, 235), 2);
        putText(frame, "Orange: " + to_string(orange), Point(20, 80), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 163, 239), 2);
        putText(frame, "Yellow: " + to_string(yellow), Point(20, 120), FONT_HERSHEY_PLAIN, 1.5, Scalar(0, 251, 251), 2);
        putText(frame, "Green: " + to_string(green), Point(20, 160), FONT_HERSHEY_PLAIN, 1.5, Scalar(1, 145, 35), 2);
        putText(frame, "Cyan: " + to_string(cyan), Point(20, 200), FONT_HERSHEY_PLAIN, 1.5, Scalar(250, 250, 35), 2);
        putText(frame, "Dark Blue: " + to_string(dark_blue), Point(20, 240), FONT_HERSHEY_PLAIN, 1.5, Scalar(248, 33, 36), 2);
        putText(frame, "Purple: " + to_string(purple), Point(20, 280), FONT_HERSHEY_PLAIN, 1.5, Scalar(123, 20, 116), 2);

        imshow("Frame", frame);
        waitKey(20);
    }
    destroyAllWindows();

    return 0;
}

int colorCount(const Mat& image, const Scalar& color) {
    Mat out;
    int square_area = 3400;
    int range = 35;
    Scalar low(max(0.0, color[0]-range), max(0.0, color[1]-range), max(0.0, color[2]-range));
    Scalar high(min(255.0, color[0]+range), min(255.0, color[1]+range), min(255.0, color[2]+range));
    inRange(image, low, high, out);
    return countNonZero(out) / square_area;
}