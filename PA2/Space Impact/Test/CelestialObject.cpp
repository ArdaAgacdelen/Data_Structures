#include "CelestialObject.h"

#include <iostream>


// Constructor to initialize CelestialObject with essential properties
CelestialObject::CelestialObject(const vector<vector<bool> > &shape, ObjectType type, int start_row,
                                 int time_of_appearance)
    : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance) {
    // TODO: Your code here
    define_rotations();
}

CelestialObject::CelestialObject(const vector<vector<bool> > &shape, ObjectType type, int start_row,
                                 int time_of_appearance, CelestialObject *next, int occupiedCellCount)
    : shape(shape), object_type(type), starting_row(start_row), time_of_appearance(time_of_appearance),
      next_celestial_object(next), occupiedCellCount(occupiedCellCount) {
}


// Copy constructor for CelestialObject
CelestialObject::CelestialObject(const CelestialObject *other)
    : shape(other->shape), // Copy the 2D vector shape
      object_type(other->object_type), // Copy the object type
      starting_row(other->starting_row), // Copy the starting row
      time_of_appearance(other->time_of_appearance), // Copy the time of appearance
      occupiedCellCount(other->occupiedCellCount) {
    // TODO: Your code here
    this->next_celestial_object = other->next_celestial_object;
}


// Function to delete rotations of a given celestial object. It should free the dynamically allocated
// memory for each rotation.
void CelestialObject::delete_rotations(CelestialObject *target) {
    // TODO: Your code here
    CelestialObject *head = target;
    target = target->right_rotation;
    while (target != nullptr && target != head) {
        CelestialObject *tmp = target->right_rotation;
        delete target;
        target = tmp;
    }
    head->left_rotation = nullptr;
    head->right_rotation = nullptr;
}

bool CelestialObject::is_destroyed() {
    if (shape.empty()) { return false; }

    for (vector<bool> row: shape) {
        for (bool col: row) {
            if (col) return false;
        }
    }
    return true;
}

void CelestialObject::define_rotations() {
    if (shape.empty()) { return; }

    if (shape.empty() || shape.at(0).empty()) {
        return;
    }

    int rowCount = shape.size();
    int colCount = shape.at(0).size();
    bool symmetric = is_symmetric(rowCount, colCount, shape);
    int rotationCount;
    if (symmetric && rowCount == colCount) {
        rotationCount = 1;
    } else if (symmetric) {
        rotationCount = 2;
    } else {
        rotationCount = 4;
    }

    CelestialObject *temp_right = this;
    CelestialObject *temp_left = this;
    for (int i = 0; i < rotationCount - 1; ++i) {
        if (i % 2 == 0) {
            temp_right->right_rotation = rotate_right(rowCount, colCount, temp_right->shape);
            temp_left->left_rotation = rotate_left(rowCount, colCount, temp_left->shape);
        } else {
            temp_right->right_rotation = rotate_right(colCount, rowCount, temp_right->shape);
            temp_left->left_rotation = rotate_left(colCount, rowCount, temp_left->shape);
        }

        if (temp_right->right_rotation) {
            temp_right->right_rotation->left_rotation = temp_right;
            temp_right = temp_right->right_rotation;
        }
        if (temp_left->left_rotation) {
            temp_left->left_rotation->right_rotation = temp_left;
            temp_left = temp_left->left_rotation;
        }
    }

    if (temp_right) {
        temp_right->right_rotation = this;
    }
    if (temp_left) {
        temp_left->left_rotation = this;
    }
}


bool CelestialObject::is_symmetric(int rowCount, int colCount, const vector<vector<bool> > &shape) {
    if (shape.empty()) { return false; }

    for (int i = 0; i < rowCount; ++i) {
        for (int j = 0; j < colCount / 2; ++j) {
            if (shape[i][j] != shape[i][colCount - j - 1]) {
                return false;
            }
        }
    }

    for (int i = 0; i < rowCount / 2; ++i) {
        for (int j = 0; j < colCount; ++j) {
            if (shape[i][j] != shape[rowCount - i - 1][j]) {
                return false;
            }
        }
    }

    return true;
}


CelestialObject *CelestialObject::rotate_right(int rowCount, int colCount, const vector<vector<bool> > &shape) {
    if (shape.empty()) { return nullptr; }

    vector<vector<bool> > rotated_shape;

    for (int i = 0; i < colCount; ++i) {
        vector<bool> temp;
        for (int j = rowCount - 1; j >= 0; --j) {
            if (j < shape.size() && i < shape[j].size()) {
                temp.push_back(shape[j][i]);
            }
        }
        rotated_shape.push_back(temp);
    }
    CelestialObject *retCel = new CelestialObject(rotated_shape, object_type, starting_row, time_of_appearance,
                                                  next_celestial_object, occupiedCellCount);
    return retCel;
}


CelestialObject *CelestialObject::rotate_left(int rowCount, int colCount, const vector<vector<bool> > &shape) {
    if (shape.empty()) { return nullptr; }

    vector<vector<bool> > rotated_shape;

    for (int i = colCount - 1; i >= 0; --i) {
        vector<bool> temp;
        for (int j = 0; j < rowCount; ++j) {
            if (j < shape.size() && i < shape[j].size()) {
                temp.push_back(shape[j][i]);
            }
        }
        rotated_shape.push_back(temp);
    }

    CelestialObject *retCel = new CelestialObject(rotated_shape, object_type, starting_row, time_of_appearance,
                                                  next_celestial_object, occupiedCellCount);
    return retCel;
}
