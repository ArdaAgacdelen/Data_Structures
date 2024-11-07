#include "SecretImage.h"


// Constructor: split image into upper and lower triangular arrays
SecretImage::SecretImage(const GrayscaleImage &image) {
    // TODO: Your code goes here.
    width = image.get_width();
    height = image.get_height();


    // 1. Dynamically allocate the memory for the upper and lower triangular matrices.
    int lowerSize = (height - 1) * (height) / 2;
    int upperSize = width * height - lowerSize;
    lower_triangular = new int[lowerSize];
    upper_triangular = new int[upperSize];


    // 2. Fill both matrices with the pixels from the GrayscaleImage.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i <= j) {
                int cellsSkipped = i * (i + 1) / 2;
                upper_triangular[width * i + j - cellsSkipped] = image.get_pixel(i, j);
            } else {
                lower_triangular[(i-1) * (i) / 2 + j] = image.get_pixel(i, j);
            }
        }
    }
}

// Constructor: instantiate based on data read from file
SecretImage::SecretImage(int w, int h, int *upper, int *lower) {
    // TODO: Your code goes here.
    // Since file reading part should dynamically allocate upper and lower matrices.
    // You should simply copy the parameters to instance variables.
    width = w;
    height = h;
    upper_triangular = upper;
    lower_triangular = lower;
}


// Destructor: free the arrays
SecretImage::~SecretImage() {
    // TODO: Your code goes here.
    // Simply free the dynamically allocated memory
    // for the upper and lower triangular matrices.
    delete[] upper_triangular;
    delete[] lower_triangular;
}

// Reconstructs and returns the full image from upper and lower triangular matrices.
GrayscaleImage SecretImage::reconstruct() const {
    GrayscaleImage image(width, height);

    // TODO: Your code goes here.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i <= j) {
                int cellsSkipped = i * (i + 1) / 2;
                image.set_pixel(i, j, upper_triangular[width * i + j - cellsSkipped]);
            } else {
                image.set_pixel(i, j, lower_triangular[(i-1) * (i) / 2 + j]);
            }
        }
    }

    return image;
}

// Save the filtered image back to the triangular arrays
void SecretImage::save_back(const GrayscaleImage &image) {
    // TODO: Your code goes here.
    // Update the lower and upper triangular matrices 
    // based on the GrayscaleImage given as the parameter.
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            if (i <= j) {
                int cellsSkipped = i * (i + 1) / 2;
                upper_triangular[width * i + j - cellsSkipped] = image.get_pixel(i, j);
            } else {
                lower_triangular[i * (i + 1) / 2 - 1] = image.get_pixel(i, j);
            }
        }
    }
}

// Save the upper and lower triangular arrays to a file
void SecretImage::save_to_file(const std::string &filename) {
    // TODO: Your code goes here.
    std::ofstream file(filename);
    // 1. Write width and height on the first line, separated by a single space.
    file << width << " " << height << std::endl;

    // 2. Write the upper_triangular array to the second line.
    // Ensure that the elements are space-separated.
    int lowerSize = (height - 1) * (height) / 2;
    int upperSize = width * height - lowerSize;
    for (int i = 0; i < upperSize; i++) {
        file << upper_triangular[i];
        if (i != (upperSize - 1)) {
            file << " ";
        }
    }
    file << std::endl;

    // If there are 15 elements, write them as: "element1 element2 ... element15"
    // 3. Write the lower_triangular array to the third line in a similar manner
    for (int i = 0; i < lowerSize; i++) {
        file << lower_triangular[i];
        if (i != (lowerSize - 1)) {
            file << " ";
        }
    }

    file.close();
    // as the second line.
}

// Static function to load a SecretImage from a file
SecretImage SecretImage::load_from_file(const std::string &filename) {
    // TODO: Your code goes here.
    // 1. Open the file and read width and height from the first line, separated by a space.
    std::ifstream file(filename);
    int width, height;
    file >> width >> height;


    // 2. Calculate the sizes of the upper and lower triangular arrays.
    int lowerSize = (height - 1) * (height) / 2;
    int upperSize = width * height - lowerSize;

    // 3. Allocate memory for both arrays.
    int* lower_triangular = new int[lowerSize];
    int* upper_triangular = new int[upperSize];

    // 4. Read the upper_triangular array from the second line, space-separated.
    for (int i = 0; i < upperSize; i++) {
        file >> upper_triangular[i];
    }
    // 5. Read the lower_triangular array from the third line, space-separated.
    for (int i = 0; i < lowerSize; i++) {
        file >> lower_triangular[i];
    }
    // 6. Close the file and return a SecretImage object initialized with the
    //    width, height, and triangular arrays.

    SecretImage secret_image(width, height, upper_triangular, lower_triangular);

    file.close();
    return secret_image;
}

// Returns a pointer to the upper triangular part of the secret image.
int *SecretImage::get_upper_triangular() const {
    return upper_triangular;
}

// Returns a pointer to the lower triangular part of the secret image.
int *SecretImage::get_lower_triangular() const {
    return lower_triangular;
}

// Returns the width of the secret image.
int SecretImage::get_width() const {
    return width;
}

// Returns the height of the secret image.
int SecretImage::get_height() const {
    return height;
}
