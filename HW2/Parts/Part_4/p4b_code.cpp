//
// Created by Karter Krueger on 2/23/20.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat binary_piece(const std::string& filename);

int main() {
    Mat input = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_4/p4_board_3_grain.jpg");
    imshow("Input", input);

    // Screenshots of the pieces are read in and then we can use as structuring elements
    Mat pawn_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_4/Black_Pawn.png");
    Mat pawn_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_4/White_Pawn.png");
    Mat queen_black = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_4/Black_Queen_White.png");
    Mat queen_white = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_4/White_Queen.png");
    Mat queen_black_grey = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_4/Black_Queen.png");
    Mat queen_white_grey = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_4/White_Queen_Grey.png");

    Mat input_gray, input_binary;
    cvtColor(input, input_gray, COLOR_BGR2GRAY);
    threshold(input_gray, input_binary, 100, 255, THRESH_BINARY_INV);
    dilate(input_binary, input_binary, getStructuringElement(MORPH_RECT, Size(3,3)));

    Mat black_out;
    erode(input_binary, black_out, pawn_black, Point(pawn_black.cols/2, pawn_black.rows/2));
    Mat white_out;
    erode(input_binary, white_out, pawn_white, Point(pawn_white.cols/2, pawn_white.rows/2));

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

        int w = queen_black.cols, h = queen_black.rows;
        if (input_gray.at<uchar>(Point(r.x, r.y-5)) > 100 && r.y < 60) {
            // white top row
            if ((i+j)%2) {
                //grey background
                queen_white_grey.copyTo(input(Rect(r.x-w/2+1, r.y-h/2+1, w, h)));
            } else {
                queen_white.copyTo(input(Rect(r.x-w/2+1, r.y-h/2+1, w, h)));
            }
        } else if(input_gray.at<uchar>(Point(r.x, r.y-5)) <= 100 && r.y > 420){
            // black bottom row
            if ((i+j)%2) {
                //grey background
                queen_black_grey.copyTo(input(Rect(r.x-w/2+1, r.y-h/2+1, w, h)));
            } else {
                queen_black.copyTo(input(Rect(r.x-w/2+1, r.y-h/2+1, w, h)));
            }
        }
    }

    imshow("Output", input);
    waitKey(0); // wait for keyboard
    return 0;
}

Mat binary_piece(const std::string& filename) {
    Mat piece = imread(filename);
    cvtColor(piece, piece, COLOR_BGR2GRAY);
    threshold(piece, piece, 80, 255, THRESH_BINARY_INV);
    return piece;
}
