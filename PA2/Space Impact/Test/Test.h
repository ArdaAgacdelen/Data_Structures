#include <iostream>
#include <iomanip>
#include <stdexcept>
#include <string>
#include <vector>
#include <map>

#include "AsteroidDash.h"
#include "Leaderboard.h"
#include "LeaderboardEntry.h"
#include "GameController.h"

using namespace std;

#define SAMPLE_GRID_FILENAME "sample_io/sample_grid.dat"
#define SAMPLE_PLAYER_FILENAME "sample_io/sample_player.dat"
#define SAMPLE_CELESTIAL_OBJECTS_FILENAME "sample_io/sample_celestial_objects.dat"
#define SAMPLE_COMMANDS_FILENAME "sample_io/sample_commands.dat"
#define SAMPLE_DEFAULT_PLAYER_NAME "BBM203"
#define SAMPLE_LEADERBOARD_FILENAME "leaderboard.txt"

#define SAMPLE_EMPTY_COMMANDS_FILENAME "sample_io/sample_empty_commands.dat"
#define SAMPLE_EMPTY_CELESTIAL_OBJECTS_FILENAME "sample_io/sample_empty_celestial_objects.dat"

#define SAMPLE_SHOOTING_CELESTIAL_OBJECTS_FILENAME "sample_io/sample_shooting_celestial_objects.dat"

#define SAMPLE_GAME_FINISHED_CELESTIAL_OBJECTS_FILENAME "sample_io/sample_game_finished_celestial_objects.dat"

#define SAMPLE_GAME_OVER_CELESTIAL_OBJECTS_FILENAME "sample_io/sample_game_over_celestial_objects.dat"

#define SAMPLE_POWERUP_CELESTIAL_OBJECTS_FILENAME "sample_io/sample_powerup_celestial_objects.dat"

#define SAMPLE_MOVEMENTS_GRID "sample_io/sample_movements_grid.dat"
#define SAMPLE_MOVEMENTS_CELESTIAL_OBJECTS "sample_io/sample_movements_celestial_objects.dat"
#define SAMPLE_MOVEMENTS_PLAYER "sample_io/sample_movements_player.dat"

#define SAMPLE_SHOOTING_UPDATE_GRID "sample_io/sample_shooting_update_space_grid.dat"
#define SAMPLE_SHOOTING_UPDATE_CELESTIAL_OBJECTS "sample_io/sample_shooting_update_celestial_objects.dat"
#define SAMPLE_SHOOTING_UPDATE_PLAYER "sample_io/sample_shooting_update_player.dat"
#define SAMPLE_SHOOTING_UPDATE_COMMANDS "sample_io/sample_shooting_update_commands.dat"

#define EXPECTED_OUTPUT_FILENAME "sample_io/expected_output.txt"

#define SAMPLE_INPUT_CELESTIAL_OBJECTS_FILENAME "sample_io/sample_input_celestial_objects.dat"
#define SAMPLE_INPUT_COMMANDS_FILENAME "sample_io/sample_input_commands.dat"
#define SAMPLE_INPUT_PLAYER_FILENAME "sample_io/sample_input_player.dat"
#define SAMPLE_INPUT_SPACE_GRID_FILENAME "sample_io/sample_input_space_grid.dat"


double check_leaderboard(const vector<std::pair<std::string, int>> &expected, LeaderboardEntry *head) {

    int i = 0;
    double score = 0;

    LeaderboardEntry *current = head;

    while (current != nullptr) {
        if (current->player_name == expected[i].first && current->score == expected[i].second) {
            score += 1;
        }
        current = current->next;
        i++;
    }
    return score;
}

bool celestial_object_equality_helper(CelestialObject *first, CelestialObject *second) {
    if (first->shape != second->shape) return false;
    if (first->object_type != second->object_type) return false;
    if (first->starting_row != second->starting_row) return false;
    if (first->time_of_appearance != second->time_of_appearance) return false;
    return true;
}

double check_rotations(const vector<CelestialObject *> expected_rotations, CelestialObject *current) {
    double score = 0;
    double left_score = 0;
    int index = 0;

    CelestialObject *current_rotation = current;
    CelestialObject *previous_rotation = nullptr;

    do {
        // Check if current node matches the expected rotation
        if (celestial_object_equality_helper(current_rotation, expected_rotations[index])) {
            score += 1;
        }

        // Verify if left link of the current node points to the previous node
        if (previous_rotation != nullptr && (current_rotation->left_rotation->shape == previous_rotation->shape))  {
            left_score += 1;
        }

        // Move to the next node in the right rotation
        previous_rotation = current_rotation;
        current_rotation = current_rotation->right_rotation;
        index++;
    } while (current_rotation != current);

    score /= 4;
    left_score /= 3;
    return score * 0.5 + left_score * 0.5;
}


// Function to trim leading and trailing whitespaces
std::string trim(const std::string &str) {
    // Find the first non-whitespace character from the left
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) {
        // If the string is all whitespace, return an empty string
        return "";
    }

    // Find the last non-whitespace character from the right
    size_t end = str.find_last_not_of(" \t\n\r\f\v");

    // Return the trimmed substring
    return str.substr(start, end - start + 1);
}

// Function to check if a string contains a substring
bool contains(const std::string &str, const std::string &substr) {
    return str.find(substr) != std::string::npos;
}

