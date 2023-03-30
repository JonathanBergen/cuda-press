#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <limits>
#include <random>
#include <vector>

struct Point {
  double x{0}, y{0}, z{0};
};

using DataFrame = std::vector<Point>;

double square(double value) {
  return value * value;
}

double squared_l2_distance(Point first, Point second) {
  return square(first.x - second.x) + square(first.y - second.y) + square(first.z - second.z);
}

DataFrame k_means(const DataFrame& data,
                  size_t k,
                  size_t number_of_iterations) {
  static std::random_device seed;
  static std::mt19937 random_number_generator(seed());
  std::uniform_int_distribution<size_t> indices(0, data.size() - 1);

  // Pick centroids as random points from the dataset.
  DataFrame means(k);
  for (auto& cluster : means) {
    cluster = data[indices(random_number_generator)];
  }

  std::vector<size_t> assignments(data.size());
  for (size_t iteration = 0; iteration < number_of_iterations; ++iteration) {
    // Find assignments.
    for (size_t point = 0; point < data.size(); ++point) {
      double best_distance = std::numeric_limits<double>::max();
      size_t best_cluster = 0;
      for (size_t cluster = 0; cluster < k; ++cluster) {
        const double distance =
            squared_l2_distance(data[point], means[cluster]);
        if (distance < best_distance) {
          best_distance = distance;
          best_cluster = cluster;
        }
      }
      assignments[point] = best_cluster;
    }

    // Sum up and count points for each cluster.
    DataFrame new_means(k);
    std::vector<size_t> counts(k, 0);
    for (size_t point = 0; point < data.size(); ++point) {
      const auto cluster = assignments[point];
      new_means[cluster].x += data[point].x;
      new_means[cluster].y += data[point].y;
      new_means[cluster].z += data[point].z;
      counts[cluster] += 1;
    }

    // Divide sums by counts to get new centroids.
    for (size_t cluster = 0; cluster < k; ++cluster) {
      // Turn 0/0 into 0/1 to avoid zero division.
      const auto count = std::max<size_t>(1, counts[cluster]);
      means[cluster].x = new_means[cluster].x / count;
      means[cluster].y = new_means[cluster].y / count;
      means[cluster].z = new_means[cluster].z / count;
    }
  }

  return means;
}

int main() {
  // Load the image
  cv::Mat image = cv::imread("dietrich.jpg");

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
  size_t k = 10;
  size_t num_iterations = 30;
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

    // Save the image
    cv::imwrite("dietrich_compressed.jpg", image);




  return 0;
}
