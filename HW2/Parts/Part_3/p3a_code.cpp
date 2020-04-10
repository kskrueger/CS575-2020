//
// Created by Karter Krueger on 2/23/20.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat input = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/p3_board_3.jpg");
    imshow("Input", input);

    // Screenshots of the Rooks are read in and then we can use as structuring elements
    Mat rook_black = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/Black_Rook.png");
    Mat rook_white = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/White_Rook.png");

    cvtColor(rook_black, rook_black, COLOR_BGR2GRAY);
    threshold(rook_black, rook_black, 100, 255, THRESH_BINARY_INV);

    cvtColor(rook_white, rook_white, COLOR_BGR2GRAY);
    threshold(rook_white, rook_white, 100, 255, THRESH_BINARY_INV);

    Mat input_gray, input_binary;
    cvtColor(input, input_gray, COLOR_BGR2GRAY);
    threshold(input_gray, input_binary, 100, 255, THRESH_BINARY_INV);

    Mat black_out;
    erode(input_binary, black_out, rook_black, Point(rook_black.cols/2, rook_black.rows/2));
    Mat white_out;
    erode(input_binary, white_out, rook_white, Point(rook_white.cols/2, rook_white.rows/2));

    Mat output;
    bitwise_or(black_out, white_out, output);

    Mat color(input.size(), input.type(), Scalar(255, 0, 0));
    Mat mask = Mat::zeros(input.size(), CV_8UC1);

    Mat canny;
    Canny(output, canny, 10, 10);
    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for (const auto& c : contours) {
        Rect r = boundingRect(c);
        if (input_gray.at<uchar>(Point(r.x, r.y)) > 100) {
            // white
            rook_white.copyTo(mask(cv::Rect(r.x-24, r.y-24, rook_white.cols, rook_white.rows)));
        } else {
            // black
            rook_black.copyTo(mask(cv::Rect(r.x-24, r.y-24, rook_white.cols, rook_white.rows)));
        }
    }

    color.copyTo(input, mask);
    imshow("Output", input);
    waitKey(0); // wait for keyboard
    return 0;
}
