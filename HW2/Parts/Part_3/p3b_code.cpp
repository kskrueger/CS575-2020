//
// Created by Karter Krueger on 2/23/20.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

using namespace std;
using namespace cv;

Mat binary_piece(const std::string& filename);
void color_pieces(const Mat& input, const Mat& piece_black, const Mat& piece_white, const Scalar& color);

int main() {
    Mat input = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/p3_board_3.jpg");
    imshow("Input", input);

    // Screenshots of the pieces are read in and then we can use as structuring elements
    Mat rook_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/Black_Rook.png");
    Mat rook_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/White_Rook.png");
    Mat queen_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/Black_Queen.png");
    Mat queen_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/White_Queen.png");
    Mat bishop_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/Black_Bishop.png");
    Mat bishop_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_3/White_Bishop.png");

    color_pieces(input, rook_black, rook_white, Scalar(255, 0, 0));
    color_pieces(input, queen_black, queen_white, Scalar(0, 255, 0));
    color_pieces(input, bishop_black, bishop_white, Scalar(0, 0, 255));

    imwrite("p3b_out_3.png", input);
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

void color_pieces(const Mat& input, const Mat& piece_black, const Mat& piece_white, const Scalar& color) {
    Mat input_gray, input_binary;
    cvtColor(input, input_gray, COLOR_BGR2GRAY);
    threshold(input_gray, input_binary, 100, 255, THRESH_BINARY_INV);

    Mat black_out;
    erode(input_binary, black_out, piece_black, Point(piece_black.cols/2, piece_black.rows/2));
    Mat white_out;
    erode(input_binary, white_out, piece_white, Point(piece_white.cols/2, piece_white.rows/2));

    Mat output;
    bitwise_or(black_out, white_out, output);

    Mat color_full(input.size(), input.type(), color);
    Mat mask = Mat::zeros(input.size(), CV_8UC1);

    Mat canny;
    Canny(output, canny, 10, 10);
    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for (const auto& c : contours) {
        Rect r = boundingRect(c);
        int w = piece_white.cols, h = piece_white.rows;
        if (input_gray.at<uchar>(Point(r.x, r.y+2)) > 100) {
            // white
            piece_white.copyTo(mask(cv::Rect(r.x-w/2+1, r.y-(h+1)/2+1, w, h)));
        } else {
            // black
            piece_black.copyTo(mask(cv::Rect(r.x-w/2+1, r.y-(h+1)/2+1, w, h)));
        }
    }
    color_full.copyTo(input, mask);
}
