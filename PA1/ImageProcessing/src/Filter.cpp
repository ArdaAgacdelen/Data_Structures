#include "Filter.h"
#include <algorithm>
#include <cmath>
#include <vector>
#include <numeric>
#include <math.h>


// Mean Filter
void Filter::apply_mean_filter(GrayscaleImage &image, int kernelSize) {
    // TODO: Your code goes here.
    // 1. Copy the original image for reference.
    GrayscaleImage refImage = GrayscaleImage(image);
    // 2. For each pixel, calculate the mean value of its neighbors using a kernel.

    int kernelDistance = (kernelSize - 1) / 2;

    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            int startingRow = (i - kernelDistance < 0) ? 0 : i - kernelDistance;
            int finishingRow = (i + kernelDistance > image.get_height() - 1)
                                   ? image.get_height() - 1
                                   : i + kernelDistance;
            int startingColumn = (j - kernelDistance < 0) ? 0 : j - kernelDistance;
            int finishingColumn = (j + kernelDistance > image.get_width() - 1)
                                      ? image.get_width() - 1
                                      : j + kernelDistance;

            int sum = 0;
            for (int kernelRowIndex = startingRow; kernelRowIndex <= finishingRow; kernelRowIndex++) {
                for (int kernelColumnIndex = startingColumn; kernelColumnIndex <= finishingColumn; kernelColumnIndex
                     ++) {
                    sum += refImage.get_pixel(kernelRowIndex, kernelColumnIndex);
                }
            }
            int mean = sum / (kernelSize * kernelSize);

            // 3. Update each pixel with the computed mean.
            image.set_pixel(i, j, mean);
        }
    }
}

// Gaussian Smoothing Filter
void Filter::apply_gaussian_smoothing(GrayscaleImage &image, int kernelSize, double sigma) {
    // TODO: Your code goes here.

    GrayscaleImage refImage = GrayscaleImage(image);

    // 1. Create a Gaussian kernel based on the given sigma value.
    double **gaussianKernel = new double *[kernelSize];
    for (int i = 0; i < kernelSize; i++) {
        gaussianKernel[i] = new double[kernelSize];
    }

    struct Cell {
        int row;
        int column;
        Cell(int r, int c) : row(r), column(c) {}
    };

    Cell centerCell = Cell((kernelSize-1)/2, (kernelSize-1)/2);

    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            int x = centerCell.row-i;
            int y = centerCell.column-j;
            gaussianKernel[i][j] = 1/(2*M_PI*sigma*sigma) * exp(-1.0*(x*x + y*y)/(2*sigma*sigma));
        }
    }

    double sum = 0;
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            sum += gaussianKernel[i][j];
        }
    }

    // 2. Normalize the kernel to ensure it sums to 1.
    for (int i = 0; i < kernelSize; i++) {
        for (int j = 0; j < kernelSize; j++) {
            gaussianKernel[i][j] /= sum;
        }
    }

    // 3. For each pixel, compute the weighted sum using the kernel.
    int kernelDistance = (kernelSize - 1) / 2;

    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {

            Cell kernelStartingCell(0,0);
            int startingRow = i - kernelDistance;
            int startingColumn = j - kernelDistance;

            if (startingRow<0) {
                kernelStartingCell.row = startingRow*-1;
                startingRow = 0;
            }

            if (startingColumn < 0) {
                kernelStartingCell.column = startingColumn*-1;
                startingColumn = 0;
            }
            int finishingColumn = (j + kernelDistance > image.get_width() - 1)
                                      ? image.get_width() - 1
                                      : j + kernelDistance;
            int finishingRow = (i + kernelDistance > image.get_height() - 1)
                                   ? image.get_height() - 1
                                   : i + kernelDistance;

            sum = 0;
            int kernelStartingColumn = kernelStartingCell.column;
            for (int index = startingRow; index <= finishingRow; index++) {
                for (int index1 = startingColumn; index1 <= finishingColumn; index1
                     ++) {
                    sum += refImage.get_pixel(index, index1) * gaussianKernel[kernelStartingCell.row][kernelStartingCell.column];
                    kernelStartingCell.column++;
                     }
                kernelStartingCell.row++;
                kernelStartingCell.column = kernelStartingColumn;
            }
            image.set_pixel(i, j, (int)sum);
        }
    }

    // 4. Update the pixel values with the smoothed results.

    for (int i = 0; i < kernelSize; i++) {
        delete[] gaussianKernel[i];
    }
    delete[] gaussianKernel;
}

// Unsharp Masking Filter
void Filter::apply_unsharp_mask(GrayscaleImage &image, int kernelSize, double amount) {
    // TODO: Your code goes here.
    GrayscaleImage refImage = GrayscaleImage(image);

    // 1. Blur the image using Gaussian smoothing, use the default sigma given in the header.
    Filter::apply_gaussian_smoothing(image, kernelSize,  1);

    // 2. For each pixel, apply the unsharp mask formula: original + amount * (original - blurred).
    for (int i = 0; i < image.get_height(); i++) {
        for (int j = 0; j < image.get_width(); j++) {
            int value = refImage.get_pixel(i, j) + amount*(refImage.get_pixel(i,j)-image.get_pixel(i,j));
            value = (value > 255) ? 255 : value;
            value = (value < 0) ? 0 : value;
            image.set_pixel(i, j, value);
        }
    }

    // 3. Clip values to ensure they are within a valid range [0-255].
}
