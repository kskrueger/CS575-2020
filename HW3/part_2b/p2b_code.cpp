// Created by Karter Krueger on April 25, 2020

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

Mat colorFilter(const Mat& image, const Scalar& low, const Scalar& high);
inline Mat downSize(const Mat& input, int scale) {
    Mat out;
    resize(input, out, Size(int(input.cols/scale), int(input.rows/scale)));
    return out;
}

int lastX = 0;
int t = 0;
int coinCount = 0;
int main() {
    VideoCapture video("/Users/kskrueger/CLionProjects/CV/src/cs575/HW3-2020/part_2b/p2b_mario_1.mp4");
    Mat frame;
    Mat last;
    Mat coin = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW3-2020/part_2b/Coin.png");
    cvtColor(coin, coin, COLOR_BGR2GRAY);
    erode(coin, coin, getStructuringElement(MORPH_RECT, Size(5,5)));
    imshow("Coin", coin);

    while (true) {
        video >> frame;
        if (frame.empty()) break;

        Mat coinColor = colorFilter(frame, Scalar(18,133,206), Scalar(125,192,255));
        dilate(coinColor, coinColor, getStructuringElement(MORPH_RECT, Size(13,13)));
        erode(coinColor, coinColor, getStructuringElement(MORPH_RECT, Size(7,7)));
        erode(coinColor, coinColor, coin);
        dilate(coinColor, coinColor, coin);

        vector<vector<Point>> contours;
        findContours(coinColor, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
        for (const auto& cont : contours) {
            Rect r = boundingRect(cont);
            if (r.y < 400) {
                //putText(frame, to_string(r.y) + "," + to_string(r.x), Point(r.x,r.y), FONT_HERSHEY_PLAIN, 2, Scalar(255, 0, 255), 2);
                //rectangle(frame, r, Scalar(255, 0, 0), 2);
                if (abs(r.x-lastX) > 55) {
                    cout << "COIN" << endl;
                    coinCount++;
                    t = 0;
                    lastX = 0;
                }
                if (t < 50) {
                    lastX = r.x;
                } else {
                    lastX = 0;
                }
            }
        }
        t++;

        putText(frame, "Coins: " + to_string(coinCount), Point(50, 50), FONT_HERSHEY_SIMPLEX, 2, Scalar(0, 255, 0), 2);
        imshow("Frame", frame);
        waitKey(1);
    }
    destroyAllWindows();
    return 0;
}

Mat colorFilter(const Mat& image, const Scalar& low, const Scalar& high) {
    Mat out;
    inRange(image, low, high, out);
    return out;
}
