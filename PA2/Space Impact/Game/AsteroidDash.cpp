#include "AsteroidDash.h"


// Constructor to initialize AsteroidDash with the given parameters
AsteroidDash::AsteroidDash(const string &space_grid_file_name,
                           const string &celestial_objects_file_name,
                           const string &leaderboard_file_name,
                           const string &player_file_name,
                           const string &player_name)

    : leaderboard_file_name(leaderboard_file_name), leaderboard(Leaderboard()) {
    read_player(player_file_name, player_name); // Initialize player using the player.dat file
    read_space_grid(space_grid_file_name); // Initialize the grid after the player is loaded
    read_celestial_objects(celestial_objects_file_name); // Load celestial objects
    leaderboard.read_from_file(leaderboard_file_name);

    CelestialObject *firstTemp = celestial_objects_list_head;

    while (firstTemp != nullptr) {
        CelestialObject *rotTemp = firstTemp->right_rotation;

        while (rotTemp != nullptr && rotTemp != firstTemp) {
            rotTemp->next_celestial_object = firstTemp->next_celestial_object;
            rotTemp = rotTemp->right_rotation;
        }

        firstTemp = firstTemp->next_celestial_object;
    }
}

// Function to read the space grid from a file
void AsteroidDash::read_space_grid(const string &input_file) {
    ifstream file(input_file);
    if (!file.is_open()) {
        return;
    }
    int cell;
    vector<int> row;

    while (file >> cell) {
        row.push_back(cell);

        if (file.peek() == '\n' || file.eof()) {
            space_grid.push_back(row);
            row.clear();
        }
    }
    file.close();
}


// Function to read the player from a file
void AsteroidDash::read_player(const string &player_file_name, const string &player_name) {
    ifstream file(player_file_name);
    if (!file.is_open()) {
        return;
    }

    int start_row, start_col;
    file >> start_row >> start_col;

    vector<vector<bool> > shape;
    string line;

    while (getline(file, line)) {
        vector<bool> lineVector;
        for (char c: line) {
            if (c == '1') lineVector.push_back(true);
            else if (c == '0') lineVector.push_back(false);
        }
        if (!lineVector.empty()) {
            shape.push_back(lineVector);
        }
    }

    player = new Player(shape, start_row, start_col, player_name);
    file.close();
}

void AsteroidDash::read_celestial_objects(const string &input_file) {
    ifstream file(input_file);
    if (!file.is_open()) {
        return;
    }

    bool paranthesisFound = false;
    string line;

    while (getline(file, line)) {
        if (!line.empty() && (line[0] == '[' || line[0] == '{')) {
            paranthesisFound = true;
            break;
        }
    }
    if (!paranthesisFound) {
        file.close();
        return;
    }

    file.clear();
    file.seekg(0, ios::beg);

    while (getline(file, line)) {
        vector<vector<bool> > shape;
        int startingRow = 0;
        int time = 0;
        int occupied = 0;
        ObjectType type = ASTEROID;

        if (line[0] == '[' || line[0] == '{') {
            do {
                vector<bool> lineVector;
                for (char c: line) {
                    if (c == '1') {
                        lineVector.push_back(true);
                        occupied++;
                    } else if (c == '0') {
                        lineVector.push_back(false);
                    }
                }
                if (!lineVector.empty()) shape.push_back(lineVector);

                if (!getline(file, line)) break;
            } while (!line.empty() && line[0] != 's' && line[0] != 't' && line[0] != 'e');
        }

        if (shape.empty()) {
            continue;
        }

        while (!line.empty() && (line[0] == 's' || line[0] == 't' || line[0] == 'e')) {
            if (line.at(0) == 's') {
                startingRow = stoi(line.substr(2));
            } else if (line.at(0) == 't') {
                time = stoi(line.substr(2));
            } else if (line.at(0) == 'e') {
                string powerUP = line.substr(2);
                if (powerUP == "life") type = LIFE_UP;
                else if (powerUP == "ammo") type = AMMO;
            }
            if (!getline(file, line)) break;
        }

        CelestialObject *temp = celestial_objects_list_head;
        if (temp == nullptr) {
            temp = new CelestialObject(shape, type, startingRow, time);
            temp->occupiedCellCount = occupied;
            celestial_objects_list_head = temp;
        } else {
            while (temp->next_celestial_object != nullptr) {
                temp = temp->next_celestial_object;
            }
            temp->next_celestial_object = new CelestialObject(shape, type, startingRow, time);
            temp->next_celestial_object->occupiedCellCount = occupied;
        }
    }

    CelestialObject *firstTemp = celestial_objects_list_head;

    while (firstTemp != nullptr) {
        CelestialObject *rotTemp = firstTemp->right_rotation;

        while (rotTemp != nullptr && rotTemp != firstTemp) {
            rotTemp->next_celestial_object = firstTemp->next_celestial_object;
            rotTemp = rotTemp->right_rotation;
        }

        firstTemp = firstTemp->next_celestial_object;
    }


    file.close();
}


// Print the entire space grid
void AsteroidDash::print_space_grid() const {
    // TODO: Your code here

    if (space_grid.empty()) {
        return;
    }
    for (const vector<int> &row: space_grid) {
        for (const int &col: row) {
            if (col == 0) {
                cout << unoccupiedCellChar;
            } else {
                cout << occupiedCellChar;
            }
        }
        cout << endl;
    }
    cout << endl;
}

void AsteroidDash::print_player_state() const {
    unsigned long highScore = (leaderboard.head_leaderboard_entry) ? leaderboard.head_leaderboard_entry->score : 0;
    cout << "Tick: " << game_time << endl;
    cout << "Lives: " << ((player->lives > 0) ? player->lives : 0) << endl;
    cout << "Ammo: " << player->current_ammo << endl;
    cout << "Score: " << current_score << endl;
    cout << "High Score: " << ((highScore > current_score) ? highScore : current_score) << endl;
}


// Function to update the space grid with player, celestial objects, and any other changes
// It is called in every game tick before moving on to the next tick.
void AsteroidDash::update_space_grid() {
    if (space_grid.empty() || player->spacecraft_shape.empty()) {
        return;
    }

    clear_space_grid();

    add_celestials_to_grid();
    add_projectiles_to_grid();
    add_player_to_grid();
    if (game_over) {
        delete_space_craft();
    }

    update_projectiles();
}


// Corresponds to the SHOOT command.
// It should shoot if the player has enough ammo.
// It should decrease the player's ammo
void AsteroidDash::shoot() {
    // TODO: Your code here
    if (space_grid.empty() || player->spacecraft_shape.empty()) {
        return;
    }
    if (player->current_ammo <= 0) {
        return;
    }
    player->current_ammo--;


    int row = player->position_row + (player->spacecraft_shape.size() / 2);
    int col = player->position_col;
    for (int i = player->spacecraft_shape[0].size() - 1; i >= 0; --i) {
        if (player->spacecraft_shape.at((player->spacecraft_shape.size()) / 2).at(i)) {
            col += i + 1;
            break;
        }
    }

    Projectile *temp = projectile_list_head;
    if (temp != nullptr) {
        while (temp->next_projectile() != nullptr) {
            temp = temp->next_projectile();
        }
        temp->set_next_projectile(new Projectile(row, col));
    } else {
        projectile_list_head = new Projectile(row, col);
    }
}

void AsteroidDash::update_projectiles() {
    Projectile *temp = projectile_list_head;
    Projectile *previous = nullptr;
    while (temp != nullptr) {
        if (!temp->move_right(space_grid[0].size())) {
            if (temp == projectile_list_head) {
                projectile_list_head = temp->next_projectile();
                delete temp;
                temp = projectile_list_head;
            } else {
                previous->set_next_projectile(temp->next_projectile());
                delete temp;
                temp = previous->next_projectile();
            }
        } else {
            temp = temp->next_projectile();
        }
        previous = temp;
    }
}

// Destructor. Remove dynamically allocated member variables here.
AsteroidDash::~AsteroidDash() {
    // TODO: Your code here
    CelestialObject *temp_cel = nullptr;
    while (celestial_objects_list_head != nullptr) {
        temp_cel = celestial_objects_list_head->next_celestial_object;
        CelestialObject::delete_rotations(celestial_objects_list_head);
        delete celestial_objects_list_head;
        celestial_objects_list_head = temp_cel;
    }

    delete player;

    Projectile *temp_pro = nullptr;
    while (projectile_list_head != nullptr) {
        temp_pro = projectile_list_head->next_projectile();
        delete projectile_list_head;
        projectile_list_head = temp_pro;
    }
}

Position Projectile::getPosition() {
    return position;
}

bool Projectile::move_right(int grid_width) {
    if (grid_width <= position.y) {
        return false;
    }
    position.y++;
    return true;
}

Projectile::Projectile(int x, int y) {
    position.x = x;
    position.y = y;
}

void AsteroidDash::clear_space_grid() {
    for (int i = 0; i < space_grid.size(); ++i) {
        for (int j = 0; j < space_grid.at(0).size(); ++j) {
            space_grid.at(i).at(j) = 0;
        }
    }
}

void AsteroidDash::add_player_to_grid() {
    if (space_grid.empty() || player->spacecraft_shape.empty()) {
        return;
    }
    vector<vector<int> > &grid = space_grid;
    vector<vector<bool> > &playerShape = player->spacecraft_shape;
    int startRow = player->position_row;
    int startCol = player->position_col;

    for (int i = 0; i < playerShape.size(); ++i) {
        for (int j = 0; j < playerShape.at(i).size(); ++j) {
            if (playerShape.at(i).at(j)) {
                if (j + startCol < grid.at(0).size() && grid.at(i + startRow).at(j + startCol) != 0) {
                    find_and_delete_colliding_celestial(startRow + i, startCol + j);
                }
                grid.at(i + startRow).at(j + startCol) = 1;
            }
        }
    }
}

void AsteroidDash::delete_space_craft() {
    for (int i = 0; i < player->spacecraft_shape.size(); ++i) {
        for (int j = 0; j < player->spacecraft_shape.at(i).size(); ++j) {
            if (player->spacecraft_shape.at(i).at(j)) {
                space_grid.at(i + player->position_row).at(j + player->position_col) = 0;
            }
        }
    }
}

void AsteroidDash::add_projectiles_to_grid() {
    if (space_grid.empty() || player->spacecraft_shape.empty()) {
        return;
    }

    Projectile *beforeTemp = nullptr;
    Projectile *temp = projectile_list_head;
    vector<vector<int> > &grid = space_grid;
    vector<vector<int> > spaceGridCopy = space_grid;


    while (temp != nullptr) {
        int row = temp->getPosition().x;
        int col = temp->getPosition().y;
        bool collision = false;

        if (row >= 0 && row < grid.size() && col >= 0 && col < grid[0].size()) {
            if (col + 1 < grid[0].size() && spaceGridCopy.at(row).at(col+1) != 0) {
                collision = shoot_celestial(row, col + 1,&spaceGridCopy);
            } else if (col + 2 < grid[0].size() && spaceGridCopy.at(row).at(col+2) != 0) {
                collision = shoot_celestial(row, col + 2, &spaceGridCopy);
            }
            if (col < space_grid.at(0).size()) { grid[row][col] = 1; }
        }

        if (collision) {
            if (beforeTemp == nullptr) {
                projectile_list_head = temp->next_projectile();
            } else {
                beforeTemp->set_next_projectile(temp->next_projectile());
            }

            Projectile *toDelete = temp;
            temp = temp->next_projectile();
            delete toDelete;
        } else {
            if (col < space_grid.at(0).size()) { grid[row][col] = 1; }
            beforeTemp = temp;
            temp = temp->next_projectile();
        }
    }
}


void AsteroidDash::add_celestials_to_grid() {
    if (space_grid.empty() || player->spacecraft_shape.empty()) {
        return;
    }
    CelestialObject *temp = celestial_objects_list_head;
    vector<vector<int> > &grid = space_grid;
    int gridWidth = grid.at(0).size();

    while (temp != nullptr) {
        int startRow = temp->starting_row;

        if (temp->time_of_appearance > game_time) {
            break;
        }

        for (int i = 0; i < temp->shape.size(); ++i) {
            int progress = game_time - temp->time_of_appearance + 1;
            int maxVisibleWidth = std::min(progress, static_cast<int>(temp->shape.at(0).size()));


            for (int j = 0; j < maxVisibleWidth; ++j) {
                if (gridWidth - progress + j >= 0 && (i + startRow) < grid.size()) {
                    if (temp->shape[i][j]) {
                        grid[i + startRow][gridWidth - progress + j] = 1;
                    }
                }
            }
        }

        temp = temp->next_celestial_object;
    }
}


void AsteroidDash::find_and_delete_colliding_celestial(int collidingRow, int collidingCol) {
    if (space_grid.empty() || player->spacecraft_shape.empty()) {
        return;
    }

    CelestialObject *temp = celestial_objects_list_head;
    CelestialObject *beforeTemp = nullptr;

    while (temp != nullptr) {
        int progress = game_time - temp->time_of_appearance + 1;
        int maxVisibleWidth = std::min(progress, static_cast<int>(temp->shape[0].size()));
        int tempRow = collidingRow - temp->starting_row;
        int tempCol = collidingCol - space_grid[0].size() + progress;
        int size = space_grid[0].size();

        if (collidingRow < temp->starting_row ||
            collidingRow > temp->starting_row + temp->shape.size() ||
            static_cast<int>(game_time) - progress + maxVisibleWidth < 0 ||
            collidingCol < size - progress ||
            collidingCol > size - progress + temp->shape[0].size() ||
            tempRow < 0 || tempCol < 0 || tempCol > space_grid[0].size() || tempRow > space_grid.size()) {
            beforeTemp = temp;
            temp = temp->next_celestial_object;
            continue;
        }
        if (temp->shape[tempRow][tempCol]) {
            if (temp->object_type == LIFE_UP) {
                player->lives++;
            } else if (temp->object_type == AMMO) {
                player->current_ammo = player->max_ammo;
            } else {
                player->lives--;
                current_score--;
                if (player->lives <= 0) {
                    game_over = true;
                }
            }

            for (int i = 0; i < temp->shape.size(); ++i) {
                for (int j = 0; j < maxVisibleWidth; ++j) {
                    if (temp->shape[i][j] == 1) {
                        space_grid[temp->starting_row + i][space_grid[0].size() - progress + j] = 0;
                    }
                }
            }

            if (beforeTemp == nullptr) {
                celestial_objects_list_head = temp->next_celestial_object;
            } else {
                beforeTemp->next_celestial_object = temp->next_celestial_object;
            }
            CelestialObject::delete_rotations(temp);
            delete temp;

            if (beforeTemp != nullptr) {
                temp = beforeTemp->next_celestial_object;
            } else {
                temp = celestial_objects_list_head;
            }
        } else {
            beforeTemp = temp;
            temp = temp->next_celestial_object;
        }
    }
}


bool AsteroidDash::shoot_celestial(int collidingRow, int collidingCol, vector<vector<int>> *spaceGridCopy) {
    if (spaceGridCopy->empty() || player->spacecraft_shape.empty()) {
        return false;
    }

    bool collision = false;
    CelestialObject *temp = celestial_objects_list_head;
    CelestialObject *beforeTemp = nullptr;

    while (temp != nullptr) {
        int progress = game_time - temp->time_of_appearance + 1;
        int maxVisibleWidth = std::min(progress, static_cast<int>(temp->shape[0].size()));
        int tempRow = collidingRow - temp->starting_row;
        int size = spaceGridCopy->at(0).size();
        int tempCol = collidingCol - size + progress;

        if (temp->object_type != ASTEROID ||
            collidingRow < temp->starting_row ||
            collidingRow > temp->starting_row + temp->shape.size() ||
            static_cast<int>(game_time) - progress + maxVisibleWidth < 0 ||
            collidingCol < size - progress ||
            collidingCol > size - progress + temp->shape[0].size() ||
            tempRow < 0 || tempCol < 0 || tempCol > spaceGridCopy->at(0).size() || tempRow > spaceGridCopy->size()) {
            beforeTemp = temp;
            temp = temp->next_celestial_object;
            continue;
            }


        if (temp->shape[tempRow][tempCol]) {
            collision = true;

            temp->shape[tempRow][tempCol] = false;
            will_be_summed += 10;

            if (temp->is_destroyed()) {
                will_be_summed += 100 * temp->occupiedCellCount;
                if (beforeTemp == nullptr) {
                    celestial_objects_list_head = temp->next_celestial_object;
                } else {
                    beforeTemp->next_celestial_object = temp->next_celestial_object;
                }
                CelestialObject::delete_rotations(temp);
                delete temp;
                temp = (beforeTemp == nullptr)
                           ? celestial_objects_list_head
                           : beforeTemp->next_celestial_object;
            } else {
                CelestialObject::delete_rotations(temp);
                CelestialObject *rotated = temp;

                if (is_lower_part_collision(*temp, collidingRow)) {
                    rotated = temp->rotate_left(temp->shape.size(), temp->shape[0].size(), temp->shape);
                } else if (is_upper_part_collision(*temp, collidingRow)) {
                    rotated = temp->rotate_right(temp->shape.size(), temp->shape[0].size(), temp->shape);
                }

                rotated->object_type = temp->object_type;
                rotated->starting_row = temp->starting_row;
                rotated->time_of_appearance = temp->time_of_appearance;
                rotated->next_celestial_object = temp->next_celestial_object;

                rotated->define_rotations();

                if (beforeTemp == nullptr) {
                    celestial_objects_list_head = rotated;
                } else {
                    beforeTemp->next_celestial_object = rotated;
                }

                if (rotated != temp) {
                    CelestialObject::delete_rotations(temp);
                    delete temp;
                }
                temp = rotated->next_celestial_object;


                for (int i = 0; i < spaceGridCopy->size(); ++i) {
                    for (int j = 0; j < spaceGridCopy->at(0).size(); ++j) {
                        spaceGridCopy->at(i).at(j) = 0;
                    }
                }

                CelestialObject *celestial = celestial_objects_list_head;
                int gridWidth = spaceGridCopy->at(0).size();

                while (celestial != nullptr) {
                    int startRow = celestial->starting_row;

                    if (celestial->time_of_appearance > game_time) {
                        break;
                    }

                    for (int i = 0; i < celestial->shape.size(); ++i) {
                        int progress = game_time - celestial->time_of_appearance + 1;
                        int maxVisibleWidth = std::min(progress, static_cast<int>(celestial->shape.at(0).size()));


                        for (int j = 0; j < maxVisibleWidth; ++j) {
                            if (gridWidth - progress + j >= 0 && (i + startRow) < spaceGridCopy->size()) {
                                if (celestial->shape[i][j]) {
                                    spaceGridCopy->at(i + startRow).at(gridWidth - progress + j )= 1;
                                }
                            }
                        }
                    }

                    celestial = celestial->next_celestial_object;
                }


            }
        } else {
            beforeTemp = temp;
            temp = temp->next_celestial_object;
        }
    }
    return collision;
}


bool AsteroidDash::is_upper_part_collision(CelestialObject &asteroid, int collidingRow) const {
    if (space_grid.empty() || player->spacecraft_shape.empty()) {
        return false;
    }
    return (asteroid.shape.size() - 1) / 2.0f > collidingRow - asteroid.starting_row;
}

bool AsteroidDash::is_lower_part_collision(CelestialObject &asteroid, int collidingRow) const {
    if (space_grid.empty() || player->spacecraft_shape.empty()) {
        return false;
    }
    return (asteroid.shape.size() - 1) / 2.0f < collidingRow - asteroid.starting_row;
}
