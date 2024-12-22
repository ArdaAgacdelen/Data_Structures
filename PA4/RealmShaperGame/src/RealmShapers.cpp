#include "RealmShapers.h"
#include <cmath>
#include <algorithm>

ShaperTree::ShaperTree() {
}

ShaperTree::~ShaperTree() {
    // TODO: Free any dynamically allocated memory if necessary
    for (auto &realmShaper: realmShapers) {
        delete realmShaper;
    }
    realmShapers.clear();
}

void ShaperTree::initializeTree(std::vector<RealmShaper *> shapers) {
    // TODO: Insert innitial shapers to the tree
    realmShapers = shapers;

    for (auto &realmShaper: realmShapers) {
        currentIsles[realmShaper->getName()] = "NOWHERE";
    }
}

int ShaperTree::getSize() {
    // TODO: Return number of shapers in the tree
    return realmShapers.size();
}

std::vector<RealmShaper *> ShaperTree::getTree() {
    return realmShapers;
}

bool ShaperTree::isValidIndex(int index) {
    bool isValid = false;

    // TODO: Check if the index is valin in the tree
    if (index >= 0 && index < realmShapers.size()) isValid = true;

    return isValid;
}

void ShaperTree::insert(RealmShaper *shaper) {
    // TODO: Insert shaper to the tree
    realmShapers.push_back(shaper);
}

int ShaperTree::remove(RealmShaper *shaper) {
    // TODO: Remove the player from tree if it exists
    // Make sure tree protects its form (complate binary tree) after deletion of a node
    // return index if found and removed
    if (!shaper) return -1;

    for (int i = 0; i < realmShapers.size(); ++i) {
        if (*realmShapers[i] == *shaper) {
            delete realmShapers[i];
            realmShapers.erase(realmShapers.begin() + i);
            return i;
        }
    }
    // else
    return -1;
}

int ShaperTree::findIndex(RealmShaper *shaper) {
    // return index in the tree if found
    if (!shaper) return -1;
    for (int i = 0; i < realmShapers.size(); ++i) {
        if (*realmShapers[i] == *shaper) {
            return i;
        }
    }
    // else
    return -1;
}

int ShaperTree::getDepth(RealmShaper *shaper) {
    // return depth of the node in the tree if found
    int index = findIndex(shaper);
    if (index != -1) {
        return ceil(log2(index + 2)) - 1;
    }
    // else
    return -1;
}

int ShaperTree::getDepth() {
    // return total|max depth|height of the tree
    if (!realmShapers.empty()) {
        return getDepth(realmShapers.back());
    }
    return 0;
}

RealmShaper ShaperTree::duel(RealmShaper *challenger, bool result) {
    // TODO: Implement duel logic, return the victor
    if (!challenger) return *realmShapers.front();
    playerDeleted = false;
    RealmShaper *parent = getParent(challenger);
    if (!parent) return *challenger;
    RealmShaper &victor = result ? *challenger : *parent;
    RealmShaper &loser = !result ? *challenger : *parent;
    victor.gainHonour();
    loser.loseHonour();

    if (result) {
        std::cout << "[Duel] " << *challenger << " won the duel" << std::endl;
        replace(challenger, parent);
    }
    else {
        std::cout << "[Duel] " << *challenger << " lost the duel" << std::endl;
    }

    std::cout << "[Honour] " << "New honour points: ";
    std::cout << *challenger << "-" << challenger->getHonour() << " ";
    std::cout << *parent << "-" << parent->getHonour() << std::endl;

    if (loser.getHonour() < 0) {
        std::cout << "[Duel] " << loser.getName() << " lost all honour, delete" << std::endl;
        deletedPlayersIsle = currentIsles[loser.getName()];
        playerDeleted = true;
        remove(&loser);
    }

    return victor;
}

RealmShaper *ShaperTree::getParent(RealmShaper *shaper) {
    RealmShaper *parent = nullptr;

    // TODO: return parent of the shaper
    int index = findIndex(shaper);
    if (index != -1) {
        int parentIndex = ((index + 1) / 2)-1;
        if (parentIndex<0) return nullptr;
        parent = realmShapers.at(parentIndex);
    }

    return parent;
}

void ShaperTree::replace(RealmShaper *player_low, RealmShaper *player_high) {
    // TODO: Change player_low and player_high's positions on the tree
    int low_index = findIndex(player_low);
    int high_index = findIndex(player_high);
    if (!isValidIndex(low_index) || !isValidIndex(high_index)) return;
    realmShapers[low_index] = player_high;
    realmShapers[high_index] = player_low;
}

RealmShaper *ShaperTree::findPlayer(RealmShaper shaper) {
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by object
    // Return the shaper if found
    // Return nullptr if shaper not found
    for (auto &realmShaper: realmShapers) {
        if (*realmShaper == shaper) {
            return realmShaper;
        }
    }

    return foundShaper;
}

// Find shaper by name
RealmShaper *ShaperTree::findPlayer(std::string name) {
    RealmShaper *foundShaper = nullptr;

    // TODO: Search shaper by name
    // Return the shaper if found
    // Return nullptr if shaper not found
    for (auto &realmShaper: realmShapers) {
        if (realmShaper->getName() == name) {
            return realmShaper;
        }
    }

    return foundShaper;
}

int ShaperTree::getRightChildIndex(int parentIndex) {
    int index = 2 * parentIndex + 2;
    if (isValidIndex(index)) {
        return index;
    }
    return -1;
}

int ShaperTree::getLeftChildIndex(int parentIndex) {
    int index = 2 * parentIndex + 1;
    if (isValidIndex(index)) {
        return index;
    }
    return -1;
}

void ShaperTree::transverseInOrder(int index, std::vector<std::string> &result) {
    if (!isValidIndex(index)) return;
    transverseInOrder(getLeftChildIndex(index), result);
    result.push_back(realmShapers.at(index)->getName());
    transverseInOrder(getRightChildIndex(index), result);
}

std::vector<std::string> ShaperTree::inOrderTraversal(int index) {
    std::vector<std::string> result = {};
    // TODO: Implement inOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation

    // Note: Since SheperTree is not an binary search tree,
    // in-order traversal will not give rankings in correct order
    // for correct order you need to implement level-order traversal
    // still you are to implement this function as well
    transverseInOrder(index, result);

    return result;
}


void ShaperTree::transversePreOrder(int index, std::vector<std::string> &result) {
    if (!isValidIndex(index)) return;
    result.push_back(realmShapers.at(index)->getName());
    transversePreOrder(getLeftChildIndex(index), result);
    transversePreOrder(getRightChildIndex(index), result);
}

std::vector<std::string> ShaperTree::preOrderTraversal(int index) {
    std::vector<std::string> result = {};
    // TODO: Implement preOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    transversePreOrder(index, result);
    return result;
}


void ShaperTree::transversePostOrder(int index, std::vector<std::string> &result) {
    if (!isValidIndex(index)) return;
    transversePostOrder(getLeftChildIndex(index), result);
    transversePostOrder(getRightChildIndex(index), result);
    result.push_back(realmShapers.at(index)->getName());
}

std::vector<std::string> ShaperTree::postOrderTraversal(int index) {
    std::vector<std::string> result = {};
    // TODO: Implement postOrderTraversal in tree
    // Add all to a string vector
    // Return the vector

    // Define and implement as many helper functions as necessary for recursive implementation
    transversePostOrder(index, result);
    return result;
}


void ShaperTree::writePlayerToFile(const std::string &name, std::ofstream &outFile) {
    outFile << name << std::endl;
}

void ShaperTree::preOrderTraversal(int index, std::ofstream &outFile) {
    // TODO: Implement preOrderTraversal in tree
    // write nodes to output file
    if (!isValidIndex(index)) return;
    writePlayerToFile(realmShapers.at(index)->getName(), outFile);
    preOrderTraversal(getLeftChildIndex(index), outFile);
    preOrderTraversal(getRightChildIndex(index), outFile);
}

void ShaperTree::breadthFirstTraversal(std::ofstream &outFile) {
    // TODO: Implement level-order traversal
    // write nodes to output file

    for (auto &realmShaper: realmShapers) {
        writePlayerToFile(realmShaper->getName(), outFile);
    }
    // Define and implement as many helper functions as necessary
}

void ShaperTree::displayTree() {
    std::cout << "[Shaper Tree]" << std::endl;
    printTree(0, 0, "");
}

// Helper function to print tree with indentation
void ShaperTree::printTree(int index, int level, const std::string &prefix) {
    if (!isValidIndex(index))
        return;

    std::cout << prefix << (level > 0 ? "   └---- " : "") << *realmShapers[index] << std::endl;
    int left = getLeftChildIndex(index); // TODO: Calculate left index
    int right = getRightChildIndex(index); // TODO: Calculate right index

    if (isValidIndex(left) || isValidIndex(right)) {
        printTree(left, level + 1, prefix + (level > 0 ? "   │   " : "")); // ╎
        printTree(right, level + 1, prefix + (level > 0 ? "   │   " : ""));
    }
}

void ShaperTree::writeShapersToFile(const std::string &filename) {
    // TODO: Write the shapers to filename output level by level
    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("ShaperTree::writeShapersToFile() - Could not open file " + filename);
    breadthFirstTraversal(file);
    std::cout << "[Output] " << "Shapers have been written to " << filename << " according to rankings." << std::endl;
}

void ShaperTree::writeToFile(const std::string &filename) {
    // TODO: Write the tree to filename output pre-order
    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("ShaperTree::writeToFile() - Could not open file " + filename);
    preOrderTraversal(0, file);
    std::cout << "[Output] " << "Tree have been written to " << filename << " in pre-order." << std::endl;

}
