// Created by Karter Krueger on April 20, 2020

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat colorFilter(const Mat& image, const Scalar& low, const Scalar& high);
inline Mat downSize(Mat input, int scale) {
    resize(input, input, Size(input.cols/scale, input.rows/scale));
    return input;
}

int main() {
    VideoCapture video("/Users/kskrueger/CLionProjects/CV/src/cs575/HW3-2020/part_2a/p2a_mario_1.mp4");
    Mat frame;
    while (true) {
        video >> frame;
        if (frame.empty()) break;

        // hat color range: rgb(171, 35, 37), rgb(197, 16, 3), rgb(161, 29, 24)
        Mat red = colorFilter(frame, Scalar(0, 10, 150), Scalar(40, 40, 205));
        erode(red, red, getStructuringElement(MORPH_RECT, Size(3,3)));
        dilate(red, red, getStructuringElement(MORPH_RECT, Size(20, 20)));
        dilate(red, red, getStructuringElement(MORPH_RECT, Size(5, 25)));

        vector<vector<Point>> contours;
        findContours(red, contours, RETR_LIST, CHAIN_APPROX_SIMPLE);
        int maxH = 0;
        Rect rect;
        for (const auto& cont : contours) {
            Rect r = boundingRect(cont);
            int h = r.height;
            if (h > maxH && r.height/double(r.width) > 1.5) {
                maxH = h;
                rect = r;
            }
        }
        rectangle(frame, rect, Scalar(255, 0, 0), 2);

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
