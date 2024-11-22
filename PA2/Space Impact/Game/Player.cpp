#include "Player.h"

// Constructor to initialize the player's spacecraft, position, and ammo
Player::Player(const vector<vector<bool> > &shape, int row, int col, const string &player_name, int max_ammo, int lives)
    : spacecraft_shape(shape), position_row(row), position_col(col), player_name(player_name), max_ammo(max_ammo),
      current_ammo(max_ammo), lives(lives) {
    // TODO: Your code here, if you want to do further initializations
}

// Move player left within the grid boundaries
void Player::move_left() {
    // TODO: Your code here
    position_col = max(position_col - 1, 0);
}

// Move player right within the grid boundaries
void Player::move_right(int grid_width) {
    // TODO: Your code here
        position_col = std::min(static_cast<int>(position_col + 1), static_cast<int>(grid_width - spacecraft_shape[0].size()));
}

// Move player up within the grid boundaries
void Player::move_up() {
    // TODO: Your code here
        position_row = std::max(position_row - 1, 0);
}

// Move player down within the grid boundaries
void Player::move_down(int grid_height) {
    // TODO: Your code here
        position_row = std::min((position_row + 1), static_cast<int>(grid_height - spacecraft_shape.size()));
}
