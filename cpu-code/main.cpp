#include <stdio.h>
#include <stdlib.h>
#include <opencv2/opencv.hpp>


using namespace cv;

int main() {
    // Read the image file
    Mat image = imread("brent.jpeg");

    // Check if image was loaded successfully
    if (image.empty()) {
        std::cout << "Error: Could not read image file." << std::endl;
        return -1;
    }

    // Convert the image to a 3D array
    int rows = image.rows;
    int cols = image.cols;
    int channels = image.channels();
    uchar* data = image.data;

    // Create a 3D array to store the RGB values of the image
    uchar*** img = new uchar**[rows];
    for (int i = 0; i < rows; i++) {
        img[i] = new uchar*[cols];
        for (int j = 0; j < cols; j++) {
            img[i][j] = new uchar[channels];
            for (int k = 0; k < channels; k++) {
                img[i][j][k] = data[(i*cols + j)*channels + k];
            }
        }
    }

    int my_c = 0;

    // Use the 3D array to access the RGB values of the image
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            uchar r = img[i][j][2];
            uchar g = img[i][j][1];
            uchar b = img[i][j][0];
            // Do something with the RGB values...

        }
    }

    std::cout << my_c;

    // Free the memory allocated for the 3D array
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            delete[] img[i][j];
        }
        delete[] img[i];
    }
    delete[] img;

    return 0;
}
