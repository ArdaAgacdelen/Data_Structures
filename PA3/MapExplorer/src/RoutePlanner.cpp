#include "RoutePlanner.h"
#include <iostream>
#include <fstream>

// Array to help you out with name of the cities in order
const std::string cities[81] = {
    "Adana", "Adiyaman", "Afyon", "Agri", "Amasya", "Ankara", "Antalya", "Artvin", "Aydin", "Balikesir", "Bilecik",
    "Bingol", "Bitlis", "Bolu", "Burdur", "Bursa", "Canakkale", "Cankiri", "Corum", "Denizli", "Diyarbakir", "Edirne",
    "Elazig", "Erzincan", "Erzurum", "Eskisehir", "Gaziantep", "Giresun", "Gumushane", "Hakkari", "Hatay", "Isparta",
    "Mersin", "Istanbul", "Izmir", "Kars", "Kastamonu", "Kayseri", "Kirklareli", "Kirsehir", "Kocaeli", "Konya",
    "Kutahya",
    "Malatya", "Manisa", "Kaharamanmaras", "Mardin", "Mugla", "Mus", "Nevsehir", "Nigde", "Ordu", "Rize", "Sakarya",
    "Samsun", "Siirt", "Sinop", "Sivas", "Tekirdag", "Tokat", "Trabzon", "Tunceli", "Urfa", "Usak", "Van", "Yozgat",
    "Zonguldak", "Aksaray", "Bayburt", "Karaman", "Kirikkale", "Batman", "Sirnak", "Bartin", "Ardahan", "Igdir",
    "Yalova", "Karabuk", "Kilis", "Osmaniye", "Duzce"
};

// Constructor to initialize and load constraints
RoutePlanner::RoutePlanner(const std::string &distance_data, const std::string &priority_data,
                           const std::string &restricted_data, int maxDistance)
    : maxDistance(maxDistance), totalDistanceCovered(0), numPriorityProvinces(0), numWeatherRestrictedProvinces(0) {
    // TO DO:
    // Load map data from file
    // Mark all provinces as unvisited initially
    map.loadDistanceData(distance_data);
    map.resetVisited();

    // Load priority provinces
    // Load restricted provinces
    loadPriorityProvinces(priority_data);
    loadWeatherRestrictedProvinces(restricted_data);
}

// Load priority provinces from txt file to an array of indices
void RoutePlanner::loadPriorityProvinces(const std::string &filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }

    int i = 0;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) { continue; }

        unsigned int startPos = line.find('(');
        unsigned int endPos = line.find(')');

        if (startPos != std::string::npos && endPos != std::string::npos) {
            priorityProvinces[i] = std::stoi(line.substr(startPos + 1, endPos - startPos - 1));
            i++;
            if (i == MAX_PRIORITY_PROVINCES) { return; }
            numPriorityProvinces++;
        }
    }

    file.close();
}

// Load weather-restricted provinces from txt file to an array of indices
void RoutePlanner::loadWeatherRestrictedProvinces(const std::string &filename) {
    // TODO: Your code here
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << std::endl;
        return;
    }
    std::string line;
    int i = 0;
    while (std::getline(file, line)) {
        if (line.empty()) { continue; }

        unsigned int startPos = line.find('(');
        unsigned int endPos = line.find(')');
        if (startPos != std::string::npos && endPos != std::string::npos) {
            weatherRestrictedProvinces[i] = std::stoi(line.substr(startPos + 1, endPos - startPos - 1));
            i++;
            if (i == MAX_WEATHER_RESTRICTED_PROVINCES) { return; }
            numWeatherRestrictedProvinces++;
        }
    }
    file.close();
}

// Checks if a province is a priority province
bool RoutePlanner::isPriorityProvince(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numPriorityProvinces; ++i) {
        if (priorityProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Checks if a province is weather-restricted
bool RoutePlanner::isWeatherRestricted(int province) const {
    // TODO: Your code here
    for (int i = 0; i < numWeatherRestrictedProvinces; ++i) {
        if (weatherRestrictedProvinces[i] == province) {
            return true;
        }
    }
    return false;
}

// Begins the route exploration from the starting point
void RoutePlanner::exploreRoute(int startingCity) {
    // TODO: Your code here
    stack.push(startingCity);
    route.push_back(startingCity);
    map.markAsVisited(startingCity);

    exploreFromProvince(startingCity);

    displayResults();
}

// Helper function to explore from a specific province
void RoutePlanner::exploreFromProvince(int province) {
    // TODO: Your code here
    enqueueNeighbors(province);

    while (!queue.isEmpty()) {
        int city = queue.peek();
        if (isWeatherRestricted(city)) {
            map.enqueued[queue.peek()] = false;
            queue.dequeue();
            std::cout << "Province " << cities[city] << " is weather-restricted. Skipping." << std::endl;
            continue;
        }
        if (map.getDistance(province, city) <=250){
            stack.push(city);
            route.push_back(city);
            map.markAsVisited(city);
            totalDistanceCovered += map.getDistance(province, city);
            province = city;
            map.enqueued[queue.peek()] = false;
            queue.dequeue();
            enqueueNeighbors(city);
            continue;
        }
        map.enqueued[queue.peek()] = false;
        queue.dequeue();
    }

    if (isExplorationComplete()) {
        return;
    }

    backtrack();
    while (!stack.isEmpty()) {
        popped = stack.pop();
    }
    exploreFromProvince(popped);
}

void RoutePlanner::enqueueNeighbors(int province) {
    // TO DO: Enqueue priority & non-priority neighbors to the queue according to given constraints
    for (int i = 0; i < MAX_SIZE; ++i) {
        if (map.isVisited(i) || map.enqueued[i]) { continue; }
        if (map.isWithinRange(province, i, maxDistance)) {
            if (isPriorityProvince(i)) {
                queue.enqueuePriority(i);
                map.enqueued[i] = true;
            } else {
                queue.enqueue(i);
                map.enqueued[i] = true;
            }
        }
    }
}

void RoutePlanner::backtrack() {
    // If you reach a dead-end province
    // TODO: Your code here
    popped = stack.pop();
}

bool RoutePlanner::isExplorationComplete() const {
    // TODO: Your code here
    return queue.isEmpty() && stack.isEmpty();
}

void RoutePlanner::displayResults() const {
    // TODO: Your code here
    // Display "Journey Completed!" message
    // Display the total number of provinces visited
    // Display the total distance covered
    // Display the route in the order visited
    // Priority Province Summary

    std::cout << "----------------------------" << std::endl;
    std::cout << "Journey Completed!" << std::endl;
    std::cout << "----------------------------" << std::endl;
    std::cout << "Total Number of Provinces Visited: " << route.size() << std::endl;
    std::cout << "Total Distance Covered: " << totalDistanceCovered << " km" << std::endl;
    std::cout << "Route Taken:" << std::endl;
    for (int i = 0; i < route.size(); i++) {
        std::cout << cities[route[i]];
        std::cout << " -> ";
    }
    std::cout << "End" << std::endl;
    displayPriorityProvinces();
}

void RoutePlanner::displayPriorityProvinces() const {
    int count = 0;
    std::cout << "Priority Provinces Status:" << std::endl;
    for (int i = 0; i < numPriorityProvinces; ++i) {
        std::cout << "- " << cities[priorityProvinces[i]] << " (" << (map.isVisited(priorityProvinces[i])
                                                                          ? "Visited"
                                                                          : "Not Visited") << ")" << std::endl;
        count += map.isVisited(priorityProvinces[i]);
    }
    std::cout << "Total Priority Provinces Visited: " << count << " out of " << numPriorityProvinces << "" << std::endl;
    if (count == numPriorityProvinces) {
        std::cout << "Success: All priority provinces were visited." << std::endl;
    } else {
        std::cout << "Warning: Not all priority provinces were visited." << std::endl;
    }
}
