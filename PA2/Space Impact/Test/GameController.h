#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include "AsteroidDash.h"
#include "Player.h"
#include "CelestialObject.h"
#include "Leaderboard.h"
#include <vector>
#include <string>


class GameController {
public:
    // Game instance
    AsteroidDash *game;

    // Constructor
    GameController(
        const string &space_grid_file_name,
        const string &celestial_objects_file_name,
        const string &leaderboard_file_name,
        const string &player_file_name,
        const string &player_name
    );

    // Reads commands from the given input file, executes each command in a tick
    void play(const string &commands_file);

    // Destructor to delete dynamically allocated member variables here
    virtual ~GameController();


    void add_player_to_grid();

    void delete_player_from_grid();

    const string &leaderboard_file_name;
    const string &player_name;

private:
    void add_projectiles_to_grid();

    void delete_projectiles_from_grid();

    void add_celestials_to_grid();

    void delete_celestials_from_grid();

    void find_and_delete_colliding_celestial(int collidingRow, int collidingCol, ObjectType type);

    bool is_upper_part_collision(CelestialObject &asteroid, int collidingRow);

    bool is_lower_part_collision(CelestialObject &asteroid, int collidingRow);

    bool is_middle_part_collision(CelestialObject &asteroid, int collidingRow);

    bool shoot_celestial(int collidingRow, int collidingCol, Projectile *projectile);
};

#endif // GAMECONTROLLER_H
