#include "GameController.h"

#include <thread>

// Simply instantiates the game
GameController::GameController(
    const string &space_grid_file_name,
    const string &celestial_objects_file_name,
    const string &leaderboard_file_name,
    const string &player_file_name,
    const string &player_name

) : leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    game = new AsteroidDash(space_grid_file_name, celestial_objects_file_name, leaderboard_file_name, player_file_name,
                            player_name);
    // TODO: Your code here, if you want to perform extra initializations
}

// Reads commands from the given input file, executes each command in a game tick
void GameController::play(const string &commands_file) {
    // TODO: Your code here


    if (game->space_grid.empty() || game->player->spacecraft_shape.empty()) {
        return;
    }
    fstream file(commands_file);
    string line;


    while (getline(file, line) && !game->game_over) {
        bool worked = true;

        if (line == "NOP") {
        } else if (line == "MOVE_DOWN") {
            game->player->move_down(game->space_grid.size());
        } else if (line == "MOVE_UP") {
            game->player->move_up();
        } else if (line == "MOVE_RIGHT") {
            game->player->move_right(game->space_grid[0].size());
        } else if (line == "MOVE_LEFT") {
            game->player->move_left();
        } else if (line == "SHOOT") {
            game->shoot();
        } else { worked = false; }


        game->update_space_grid();

        if (line == "PRINT_GRID") {
            game->print_player_state();
            game->print_space_grid();
        } else if (!worked) {
            cout << "Unknown command: " << line << endl;
        }


        game->game_time++;
        game->current_score++;
        game->current_score += game->willBeSummed;
        game->willBeSummed = 0;
    }

    game->leaderboard.insert(new LeaderboardEntry(game->current_score, time(nullptr), player_name));
    game->leaderboard.write_to_file(leaderboard_file_name);

    if (!game->game_over) {
        cout << "GAME FINISHED! No more commands!" << endl;
        game->game_time--;
        game->print_player_state();
        cout << "Player: " << game->player->player_name << endl;
        game->print_space_grid();
        game->leaderboard.print_leaderboard();
    } else {
        cout << "GAME OVER!" << endl;
        game->game_time--;
        game->print_player_state();
        cout << "Player: " << game->player->player_name << endl;
        game->print_space_grid();
        game->leaderboard.print_leaderboard();
    }
}


// Destructor to delete dynamically allocated member variables here
GameController::~GameController() {
    // TODO: Your code here
    delete game;
}
