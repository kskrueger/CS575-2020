// Created by Karter Krueger

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int colorCount(const Mat& image, const Scalar& color);

int main() {
    VideoCapture video("/Users/kskrueger/CLionProjects/CV/src/cs575/HW3-2020/part_3/p3_tetris_1.mp4");
    Mat frameIn;
    int rows = 0;
    int last_black = 11;
    Mat lastFrame;
    int i = 0;
    while (true) {
        video.set(CAP_PROP_POS_FRAMES, i);
        i+= 20;
        video >> frameIn;
        if (frameIn.empty()) break;

        Mat gray;
        cvtColor(frameIn, gray, COLOR_BGR2GRAY);
        Mat thresh;
        threshold(gray, thresh, 150, 255,THRESH_BINARY_INV);
        vector<vector<Point>> outerContour;
        findContours(thresh, outerContour, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
        Rect r;
        int area = 0;
        for (const auto& cont : outerContour) {
            int a = (int)contourArea(cont);
            if (a > area) {
                area = a;
                r = boundingRect(cont);
            }
        }
        Mat frame;
        frameIn(r).copyTo(frame);
        resize(frame, frame, Size(652, 1292));

        Mat bottomRow;
        frame(Rect(45, 1292-66-35, 607, 60)).copyTo(bottomRow);
        //imshow("Row", bottomRow);

        putText(frameIn, "Rows: " + to_string(rows), Point(20, 40), FONT_HERSHEY_PLAIN, 2, Scalar(0, 255, 0), 3);

        resize(frame, frame, Size(frame.cols/2, frame.rows/2));
        resize(frameIn, frameIn, Size(int(frameIn.cols/2), int(frameIn.rows/2)));
        imshow("Frame", frameIn);

        if (lastFrame.empty()) lastFrame = bottomRow;
        Mat diff;
        absdiff(bottomRow, lastFrame, diff);
        //imshow("Diff", diff);
        //imshow("Last", lastFrame);

        int black = colorCount(diff, Scalar(0, 0, 0));
        if (black < last_black-4) rows++;
        last_black = black;
        lastFrame = bottomRow;
        waitKey(10);
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
