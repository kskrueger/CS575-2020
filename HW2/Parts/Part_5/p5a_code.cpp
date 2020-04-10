//
// Created by Karter Krueger on 2/23/20.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

using namespace std;
using namespace cv;

Mat binary_piece(const std::string& filename);
void color_pieces(const Mat& input_binary, const Mat& piece_black, const Mat& piece_white, string arr[8][8], const string& name);

int main() {
    Mat input = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/p5_board_3.jpg");
    imshow("Input", input);

    // Screenshots of the pieces are read in and then we can use as structuring elements
    Mat pawn_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/Black_Pawn.png");
    Mat pawn_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/White_Pawn.png");
    Mat queen_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/Black_Queen.png");
    Mat queen_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/White_Queen.png");
    Mat rook_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/Black_Rook.png");
    Mat rook_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/White_Rook.png");
    Mat bishop_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/Black_Bishop.png");
    Mat bishop_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/White_Bishop.png");
    Mat king_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/Black_King.png");
    Mat king_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/White_King.png");
    Mat knight_black = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/Black_Knight.png");
    Mat knight_white = binary_piece("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_5/White_Knight.png");

    Mat input_gray, input_binary;
    cvtColor(input, input_gray, COLOR_BGR2GRAY);
    threshold(input_gray, input_binary, 120, 255, THRESH_BINARY_INV);
    dilate(input_binary, input_binary, getStructuringElement(MORPH_RECT, Size(3, 3)));

    string pieces[8][8];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char tmp[10];
            sprintf(tmp, "%c%d: Empty", 'A'+i, (8-j));
            pieces[j][i] = tmp;
        }
    }

    color_pieces(input_binary, pawn_black, pawn_white, pieces, "Pawn");
    color_pieces(input_binary, queen_black, queen_white, pieces, "Queen");
    color_pieces(input_binary, rook_black, rook_white, pieces, "Rook");
    color_pieces(input_binary, bishop_black, bishop_white, pieces, "Bishop");
    color_pieces(input_binary, king_black, king_white, pieces, "King");
    color_pieces(input_binary, knight_black, knight_white, pieces, "Knight");

    ofstream outFile;
    outFile.open("p5a_out_3.txt");
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            outFile << pieces[i][j] << endl;
        }
        outFile << endl;
    }
    outFile.close();
    return 0;
}

Mat binary_piece(const std::string& filename) {
    Mat piece = imread(filename);
    cvtColor(piece, piece, COLOR_BGR2GRAY);
    threshold(piece, piece, 80, 255, THRESH_BINARY_INV);
    return piece;
}

void color_pieces(const Mat& input_binary, const Mat& piece_black, const Mat& piece_white, string arr[8][8], const string& name) {
    double midSum = (countNonZero(piece_black) + countNonZero(piece_white))/2;
    cout << name << ": " << midSum << " [" << countNonZero(piece_black) << "," << countNonZero(piece_white) << "]" << endl;
    Mat black_out;
    erode(input_binary, black_out, piece_black, Point(piece_black.cols/2, piece_black.rows/2));
    Mat white_out;
    erode(input_binary, white_out, piece_white, Point(piece_white.cols/2, piece_white.rows/2));

    Mat output;
    bitwise_or(black_out, white_out, output);

    Mat canny;
    Canny(output, canny, 10, 10);
    vector<vector<Point>> contours;
    findContours(canny, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);
    for (const auto& c : contours) {
        Rect r = boundingRect(c);
        int i = (int)round((r.x)/(input_binary.cols/8.0));
        int j = (int)round((r.y)/(input_binary.rows/8.0));
        char tmp[30];
        Mat a = input_binary(Rect(r.x-piece_black.cols/2, r.y-piece_black.rows/2, piece_black.cols, piece_black.rows));
        Mat b;
        a.copyTo(b);
        erode(b, b, getStructuringElement(MORPH_RECT, Size(3, 3)));
        int area = countNonZero(b);
        sprintf(tmp, "%c%d: ", 'A'+i, (8-j));
        arr[j][i] = tmp;

        if (area > midSum+150) {
            arr[j][i] += "Black ";
        } else {
            arr[j][i] += "White ";
        }
        arr[j][i] += name;
    }
}
