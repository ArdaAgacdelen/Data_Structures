#include "Map.h"
#include <fstream>
#include <iostream>
#include <sstream>

Map::Map() {
    // TODO: Your code here
    // Initialize all distances to a value representing no direct connection
    for (int i = 0; i < MAX_SIZE; ++i) {
        for (int j = 0; j < MAX_SIZE; ++j) {
            distanceMatrix[i][j] = -1;
        }
    }
    // Initialize all provinces as unvisited
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }

    for (int i = 0; i < MAX_SIZE; ++i) {
        enqueued[i] = false;
    }
}

// Loads distance data from a file and fills the distanceMatrix
void Map::loadDistanceData(const std::string &filename) {
    // TODO: Your code here
    // Read each line in the CSV file
    // Read each cell separated by a comma
    // Convert cell to an integer and store in distanceMatrix

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }


    int i = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) { continue; }


        int j = 0;
        std::stringstream ss(line);
        std::string cell;
        while (std::getline(ss, cell, ',')) {
            if (cell.empty()) {
                j++;
                continue;
            }

            if (i < MAX_SIZE && j < MAX_SIZE) { distanceMatrix[i][j] = std::stoi(cell); }
            j++;
        }

        i++;
    }

    file.close();
}

// Checks if the distance between two provinces is within the allowed maxDistance
bool Map::isWithinRange(int provinceA, int provinceB, int maxDistance) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB] <= maxDistance;
}

// Marks a province as visited
void Map::markAsVisited(int province) {
    // TODO: Your code here
    visited[province] = true;
}

// Checks if a province has already been visited
bool Map::isVisited(int province) const {
    // TODO: Your code here
    return visited[province];
}

// Resets all provinces to unvisited
void Map::resetVisited() {
    // TODO: Your code here
    for (int i = 0; i < MAX_SIZE; ++i) {
        visited[i] = false;
    }
}

// Function to count the number of visited provinces
int Map::countVisitedProvinces() const {
    // TODO: Your code here
    int count = 0;
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (visited[i]) {count++; }
    }
    return count;
}

// Function to get the distance between two provinces
int Map::getDistance(int provinceA, int provinceB) const {
    // TODO: Your code here
    return distanceMatrix[provinceA][provinceB];
}
