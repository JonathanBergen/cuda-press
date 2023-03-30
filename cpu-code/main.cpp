#include <iostream>
#include <string>
#include <filesystem>
#include <opencv2/opencv.hpp>
#include "k_means.h"

using namespace std;
namespace fs = std::filesystem;

int main(int argc, char* argv[]) {
  // Parse command-line arguments
  if (argc != 4) {
    cerr << "Usage: " << argv[0] << " k num_iterations input_image" << endl;
    return 1;
  }
  const int k = stoi(argv[1]);
  const int num_iterations = stoi(argv[2]);
  const string input_filename = argv[3];

  // Load the input image
  cv::Mat image = cv::imread(input_filename, cv::IMREAD_COLOR);
  if (image.empty()) {
    cerr << "Error: failed to load input image: " << input_filename << endl;
    return 1;
  }

  // Convert the image to a DataFrame
  DataFrame data(image.rows * image.cols);
  for (int y = 0; y < image.rows; ++y) {
    for (int x = 0; x < image.cols; ++x) {
      cv::Vec3b pixel = image.at<cv::Vec3b>(y, x);
      Point p;
      p.x = static_cast<double>(pixel[2]);
      p.y = static_cast<double>(pixel[1]);
      p.z = static_cast<double>(pixel[0]);
      data[y * image.cols + x] = p;
    }
  }

  // Call k-means algorithm with the DataFrame
  DataFrame centroids = k_means(data, k, num_iterations);

  // Convert the DataFrame back to an image
  for (int y = 0; y < image.rows; ++y) {
    for (int x = 0; x < image.cols; ++x) {
      Point p = data[y * image.cols + x];
      int best_cluster = 0;
      double best_distance = std::numeric_limits<double>::max();
      for (int i = 0; i < k; ++i) {
        double distance = squared_l2_distance(p, centroids[i]);
        if (distance < best_distance) {
          best_distance = distance;
          best_cluster = i;
        }
      }
      cv::Vec3b pixel;
      pixel[2] = static_cast<uchar>(centroids[best_cluster].x);
      pixel[1] = static_cast<uchar>(centroids[best_cluster].y);
      pixel[0] = static_cast<uchar>(centroids[best_cluster].z);
      image.at<cv::Vec3b>(y, x) = pixel;
    }
  }

  // Generate the output image filename
  const fs::path input_path(input_filename);
  const string output_filename = (input_path.parent_path() / (input_path.stem().string() + "_compressed" + input_path.extension().string())).string();

  // Save the output image
  if (!cv::imwrite(output_filename, image)) {
    cerr << "Error: failed to write output image: " << output_filename << endl;
    return 1;
  }

  cout << "Output image saved to: " << output_filename << endl;
  return 0;
}
