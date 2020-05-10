//insert your code in part_1c/p1c_code.cpp
//edit the file extension and web template to match your programing language

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

using namespace std;
using namespace cv;

int colorCount(const Mat& image, const Scalar& color);

int main() {
    VideoCapture video("/Users/kskrueger/CLionProjects/CV/src/cs575/HW3-2020/part_1c/p1c_tetris_1.mp4");
    Mat frame;
    int rows = 0;
    int last_black = 11;
    Mat lastFrame;
    int i = 0;
    while (true) {
        video.set(CAP_PROP_POS_FRAMES, i);
        i+= 20;
        video >> frame;
        if (frame.empty()) break;
        Mat bottomRow;
        frame(Rect(0, video.get(CAP_PROP_FRAME_HEIGHT)-66, video.get(CAP_PROP_FRAME_WIDTH), 60)).copyTo(bottomRow);
        imshow("Row", bottomRow);

        putText(frame, "Rows: " + to_string(rows), Point(20, 40), FONT_HERSHEY_PLAIN, 2, Scalar(240, 240, 240), 3);

        resize(frame, frame, Size(frame.cols/2, frame.rows/2));
        imshow("Frame", frame);

        if (lastFrame.empty()) lastFrame = bottomRow;
        Mat diff;
        absdiff(bottomRow, lastFrame, diff);
        imshow("Diff", diff);
        imshow("Last", lastFrame);

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
