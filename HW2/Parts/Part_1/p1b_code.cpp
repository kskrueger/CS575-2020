//
// Created by Karter Krueger on 2/21/20.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

int main() {
    Mat input = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_1/p1_board.jpg");
    imshow("Input", input);
    Mat output(input.size(), input.type());

    Mat light_squares;
    inRange(input, Scalar(220, 220, 220), Scalar(255, 255, 255), light_squares);
    Mat sub1(light_squares.size(), light_squares.type(), Scalar((158), (158), (158)));
    bitwise_and(sub1, light_squares, light_squares);

    Mat dark_squares;
    inRange(input, Scalar(120, 120, 120), Scalar(180, 180, 180), dark_squares);
    Mat sub2(light_squares.size(), light_squares.type(), Scalar((242), (242), (242)));
    bitwise_and(sub2, dark_squares, dark_squares);

    bitwise_or(light_squares, dark_squares, output);
    imwrite("p1b_out.png", output);
    imshow("Output", output);
    waitKey(0); // wait for keyboard
    return 0;
}
