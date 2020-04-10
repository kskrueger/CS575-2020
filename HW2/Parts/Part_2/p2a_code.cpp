//
// Created by Karter Krueger on 2/23/20.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat input = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_2/p2_board_3.jpg");
    imshow("Input", input);

    // Screenshots of the Pawns are read in and then we can use as structuring elements
    Mat pawn_black = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_2/Black_Pawn.png");
    Mat pawn_white = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_2/White_Pawn.png");

    cvtColor(pawn_black, pawn_black, COLOR_BGR2GRAY);
    threshold(pawn_black, pawn_black, 50, 255, THRESH_BINARY_INV);

    cvtColor(pawn_white, pawn_white, COLOR_BGR2GRAY);
    threshold(pawn_white, pawn_white, 50, 255, THRESH_BINARY_INV);

    Mat input_binary;
    cvtColor(input, input_binary, COLOR_BGR2GRAY);
    threshold(input_binary, input_binary, 100, 255, THRESH_BINARY_INV);

    Mat black_out;
    erode(input_binary, black_out, pawn_black, Point(pawn_black.cols/2, pawn_black.rows/2));
    Mat white_out;
    erode(input_binary, white_out, pawn_white, Point(pawn_white.cols/2, pawn_white.rows/2));

    Mat output;
    bitwise_or(black_out, white_out, output);

    int height = pawn_black.rows, width = pawn_black.cols;

    Mat canny;
    Canny(output, canny, 10, 10);
    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for (const auto& c : contours) {
        Rect r = boundingRect(c);
        rectangle(input, Point(r.x-width/2, r.y-height/2),
                Point(r.x+width/2, r.y+height/2), Scalar(255, 25, 255), 2);
    }

    imshow("Output", input);
    waitKey(0); // wait for keyboard
    return 0;
}
