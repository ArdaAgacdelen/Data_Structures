#ifndef ASTEROIDDASH_H
#define ASTEROIDDASH_H

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "CelestialObject.h"
#include "LeaderboardEntry.h"
#include "Leaderboard.h"
#include "Player.h"

#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

class Projectile;
using namespace std;

// Class that encapsulates the game play internals
class AsteroidDash {
public:
    // Constructor to initialize the game
    AsteroidDash(const string &space_grid_file_name, const string &celestial_objects_file_name,
                 const string &leaderboard_file_name, const string &player_file_name, const string &player_name);

    // Destructor. Remove dynamically allocated member variables here
    virtual ~AsteroidDash();

    void clear_space_grid();

    void add_player_to_grid();

    void delete_space_craft();


    void add_projectiles_to_grid();


    void add_celestials_to_grid();


    void find_and_delete_colliding_celestial(int collidingRow, int collidingCol);

    bool shoot_celestial(int collidingRow, int collidingCol, vector<vector<int>> *spaceGridCopy);

    bool is_upper_part_collision(CelestialObject &asteroid, int collidingRow) const;

    bool is_lower_part_collision(CelestialObject &asteroid, int collidingRow) const;

    // 2D space_grid
    vector<vector<int> > space_grid;

    // Pointer to track the player instance
    Player *player = nullptr;

    // A reference to the head of the celestial objects linked list
    CelestialObject *celestial_objects_list_head = nullptr;

    Projectile *projectile_list_head = nullptr;

    // Current score of the game
    unsigned long current_score = 0;

    // Current game tick
    unsigned long game_time = 0;

    // Leaderboard file name to store and read the leaderboard from
    string leaderboard_file_name;

    // Leaderboard member variable
    Leaderboard leaderboard;

    // True if the game is over
    bool game_over = false;

    int willBeSummed = 0;


    void readField(fstream &file, int &startRow, int &timeOfAppearance, ObjectType &type, string line);

    // Function to print the space_grid
    void print_space_grid() const;

    void print_player_state() const;

    // Function to read the space grid from a file
    void read_space_grid(const string &input_file);

    // Function to read the player from a file
    void read_player(const string &player_file_name, const string &player_name);

    // Function to read celestial objects from a file
    // Reads the input file and calls the read_celestial_object() function for each celestial_object;
    void read_celestial_objects(const string &input_file);

    bool isValidShapeLine(const string &line, char opening_char, char closing_char);

    string readShape(const string &line, char closing_char, fstream &file, vector<vector<bool>> &shape, int &occupied);

    // Updates the grid based on player and celestial object states
    void update_space_grid();

    // Corresponds to the SHOOT command.
    void shoot();

    void update_projectiles();
};


struct Position {
    int x;
    int y;
};

class Projectile {
public:
    Projectile(int x, int y);

    Position getPosition() ;

    Projectile *next_projectile()  {
        return nextProjectile;
    }

    void set_next_projectile(Projectile *next_projectile) {
        nextProjectile = next_projectile;
    }

    bool move_right(int grid_width) ;


private:
    Position position{};
    Projectile *nextProjectile = nullptr;
};


#endif // ASTEROIDDASH_H
