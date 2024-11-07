#include "Crypto.h"
#include "GrayscaleImage.h"


// Extract the least significant bits (LSBs) from SecretImage, calculating x, y based on message length
std::vector<int> Crypto::extract_LSBits(SecretImage &secret_image, int message_length) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Reconstruct the SecretImage to a GrayscaleImage.
    GrayscaleImage img = secret_image.reconstruct();

    // 2. Calculate the image dimensions.
    int width = img.get_width();
    int height = img.get_height();

    // 3. Determine the total bits required based on message length.
    int total_bits = 7 * message_length;

    // 4. Ensure the image has enough pixels; if not, throw an error.
    if (width * height < total_bits) {
        std::cerr << "Error: Image does not have enough pixels!" << std::endl;
    }

    // 5. Calculate the starting pixel from the message_length knowing that
    //    the last LSB to extract is in the last pixel of the image.
    int startingIndex = width * height - total_bits;

    // // 6. Extract LSBs from the image pixels and return the result.

    for (int i = startingIndex; i < width * height; i++) {
        int row = i / width;
        int col = i % width;
        LSB_array.push_back(img.get_pixel(row, col) & 1);
    }
    return LSB_array;
}


// Decrypt message by converting LSB array into ASCII characters
std::string Crypto::decrypt_message(const std::vector<int> &LSB_array) {
    std::string message;
    // TODO: Your code goes here.

    // 1. Verify that the LSB array size is a multiple of 7, else throw an error.
    if (LSB_array.size() % 7 != 0) {
        std::cerr << "Error: LSB array cannot be decrypted!" << std::endl;
    }
    // 2. Convert each group of 7 bits into an ASCII character.
    for (int i = 0; i < LSB_array.size(); i += 7) {
        int intRepresentation = 0;
        for (int j = 0; j < 7; j++) {
            intRepresentation += LSB_array[i + j] * (1 << (6 - j));
        }
        message.push_back(static_cast<char>(intRepresentation));
    }
    // 3. Collect the characters to form the decrypted message.
    // 4. Return the resulting message.

    return message;
}

// Encrypt message by converting ASCII characters into LSBs
std::vector<int> Crypto::encrypt_message(const std::string &message) {
    std::vector<int> LSB_array;
    // TODO: Your code goes here.

    // 1. Convert each character of the message into a 7-bit binary representation.
    //    You can use std::bitset.
    // 2. Collect the bits into the LSB array.

    for (char letter: message) {
        std::bitset<7> bits(letter);
        for (int i = 6; i >=0 ; i--) {
            LSB_array.push_back(bits[i]);
        }
    }
    // 3. Return the array of bits.

    return LSB_array;
}

// Embed LSB array into GrayscaleImage starting from the last bit of the image
SecretImage Crypto::embed_LSBits(GrayscaleImage &image, const std::vector<int> &LSB_array) {

    // TODO: Your code goes here.

    int width = image.get_width();
    int height = image.get_height();
    int total_bits = LSB_array.size();

    // 1. Ensure the image has enough pixels to store the LSB array, else throw an error.
    if (total_bits > width * height) {
        std::cerr << "Error: LSB array cannot be encrypted!" << std::endl;
    }
    // 2. Find the starting pixel based on the message length knowing that
    //    the last LSB to embed should end up in the last pixel of the image.
    int startingIndex = width * height - total_bits;

    // 3. Iterate over the image pixels, embedding LSBs from the array.
    for (int i = 0; i < LSB_array.size(); i++) {
        int row = (i + startingIndex) / width;
        int col = (i + startingIndex) % width;

        if (image.get_pixel(row, col)%2 == 0 && LSB_array[i] == 1) {
            image.set_pixel(row, col, image.get_pixel(row, col) + 1);
        } else if (image.get_pixel(row, col)%2 == 1 && LSB_array[i] == 0) {
            image.set_pixel(row, col, image.get_pixel(row, col) - 1);
        }


    }
    // 4. Return a SecretImage object constructed from the given GrayscaleImage 
    //    with the embedded message.
    SecretImage secret_image(image);

    return secret_image;
}
