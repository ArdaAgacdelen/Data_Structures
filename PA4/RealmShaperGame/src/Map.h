#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <memory>
#include <vector>
#include <fstream>
#include <queue>
#include <cmath>
#include "Isle.h"
#include <stack>

struct MapNode
{
    Isle *isle;
    MapNode *left, *right;
    int height = 1;

    MapNode(Isle *isle) : isle(isle), left(nullptr), right(nullptr), height(1) {}
    ~MapNode()
    {
        // TODO: Free any dynamically allocated memory if necessary
        delete isle;
        isle = nullptr;
    }
};

class Map
{
private:
    MapNode *root; // Root node of the tree
    int reBalanceCount = 0;

    // Height of a node
    int height(MapNode *node);

    int getBalanceFactor(MapNode *node);

    // AVL Rotations
    MapNode *rotateRight(MapNode *current);
    MapNode *rotateLeft(MapNode *current);

    // Helper functions for recursive AVL insertion and deletion
    // USE THEM
    MapNode *insert(MapNode *node, Isle *isle);

    void *insertRecursive(MapNode *node, MapNode *nodeToInsert);

    MapNode *remove(MapNode *node, Isle *isle);

    MapNode *findMax(MapNode *node);

    // Item drop helper functions
    void preOrderItemDrop(MapNode *current, int &count);
    void postOrderItemDrop(MapNode *current, int &count);

public:
    // Constructor declaration
    Map();
    // Destructor
    ~Map();

    // Calculation of minumum depth a player can access at the map
    int calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth);

    // Tree operations
    void insert(Isle *isle);

    MapNode *findMin(MapNode *node);

    void remove(Isle *isle);
    /// Search
    Isle *findIsle(Isle isle);
    Isle *findIsle(std::string name);
    MapNode *findNode(Isle isle);
    MapNode *findNode(std::string name);

    // Initilize tree from a vector
    void initializeMap(std::vector<Isle *> isles);

    static MapNode *findFirstEmptyIsle(MapNode *node);
    

    int getDepth(MapNode *node); // Determines the depth of a node within the tree.
    int getIsleDepth(Isle *isle); // Determines the depth of a isle within the tree.
    int getDepth(); // Total level of the tree

    // Display tree in terminal
    void display(MapNode *current, int depth, int state);

    void writeIsleNameToFile(const std::string &name, std::ofstream &outFile);

    void displayMap();

    // Item drop api
    void populateWithItems();
    void dropItemBFS();


    // Write the AVL tree to a file with levels
    void writeToFile(const std::string &filename);

    void writeInOrderToFile(MapNode *node, std::ofstream &outFile);

    // Write current Isles to file in alphabetical order
    void writeIslesToFile(const std::string &filename);
};

#endif