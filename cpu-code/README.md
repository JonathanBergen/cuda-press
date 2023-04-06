## CS420 Final Project CPU Code

## Jonathan Bergen

### cuda-press

- Build the file: clang++ -std=c++17 main.cpp -o press `pkg-config --cflags --libs opencv4`
- Run it: ./press 10 20 tower.jpg

This program takes three parameters in this order: k(int) num_iterations(int) image_name(string).

K represents the number of centroids used in the K-means process, and therefore the number of discrete colors used in the output image.

Increasing the number of iterations causes the output image's color space to resemble the input image's color space more closely. In my GPU code, I'll make this parameter optional, since the k-means algorithm can approach convergence, which I'll calculate automatically.

The output image will be saved with \_compressed attached to the input image's name.
