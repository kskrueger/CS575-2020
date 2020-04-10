//
// Created by Karter Krueger on 2/21/20.
//

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <cstdio>
#include <cstdlib>

using namespace std;
using namespace cv;

int main() {
    Mat input = imread("/Users/kskrueger/CLionProjects/CV/src/cs575/HW2-2020/input/Part_1/p1_board.jpg");
    imshow("Input", input);

    int width = input.cols/8;
    int height = input.rows/8;

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            char* tmp = (char*)malloc(sizeof(char) *2);
            sprintf(tmp, "%c%d", i+'a', (8-j));
            putText(input,
                    tmp,
                    Point(i*width + width/2, j*height + height/2),
                    FONT_HERSHEY_SIMPLEX,
                    .5,
                    Scalar(0, 0, 255),
                    1);
        }
    }

    imshow("Output", input);
    imwrite("p1a_out.png", input);

    waitKey(0); // wait for keyboard

    return 0;
}
