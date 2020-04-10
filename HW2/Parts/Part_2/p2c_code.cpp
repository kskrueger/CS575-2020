//
// Created by Karter Krueger on 2/23/20.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;
using namespace cv;

int main() {
    Mat input = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_2/p2_board_3.jpg");
    imshow("Input", input);

    // Screenshots of the Pawns are read in and then we can use as structuring elements
    Mat pawn_black = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_2/Black_Pawn.png");
    Mat pawn_white = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_2/White_Pawn.png");
    Mat pawn_black_grey = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_2/Black_Pawn_Grey.png");
    Mat pawn_white_grey = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_2/White_Pawn_Grey.png");

    Mat pawn_black_e;
    cvtColor(pawn_black, pawn_black_e, COLOR_BGR2GRAY);
    threshold(pawn_black_e, pawn_black_e, 50, 255, THRESH_BINARY_INV);

    Mat pawn_white_e;
    cvtColor(pawn_white, pawn_white_e, COLOR_BGR2GRAY);
    threshold(pawn_white_e, pawn_white_e, 50, 255, THRESH_BINARY_INV);

    Mat input_gray, input_binary;
    cvtColor(input, input_gray, COLOR_BGR2GRAY);
    threshold(input_gray, input_binary, 100, 255, THRESH_BINARY_INV);

    Mat black_out;
    erode(input_binary, black_out, pawn_black_e, Point(pawn_black_e.cols/2, pawn_black_e.rows/2));
    Mat white_out;
    erode(input_binary, white_out, pawn_white_e, Point(pawn_white_e.cols/2, pawn_white_e.rows/2));

    Mat output;
    bitwise_or(black_out, white_out, output);

    Mat canny;
    Canny(output, canny, 10, 10);
    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for (const auto& c : contours) {
        Rect r = boundingRect(c);
        int i = (int)round((r.x)/(input.cols/8.0));
        int j = (int)round((r.y)/(input.rows/8.0));
        if (input_gray.at<uchar>(Point(r.x, r.y)) > 100) {
            //white pawn
            if ((i+j)%2) {
                //grey background
                pawn_black_grey.copyTo(input(cv::Rect(r.x-24, r.y-24, 50, 50)));
            } else {
                pawn_black.copyTo(input(cv::Rect(r.x-24, r.y-24, 50, 50)));
            }
        } else {
            //black pawn
            if ((i+j)%2) {
                //grey background
                pawn_white_grey.copyTo(input(cv::Rect(r.x-24, r.y-24, 50, 50)));
            } else {
                pawn_white.copyTo(input(cv::Rect(r.x-24, r.y-24, 50, 50)));
            }
        }
    }

    imshow("Output", input);
    waitKey(0); // wait for keyboard
    return 0;
}
