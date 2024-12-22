#include "Map.h"

Map::Map() {
    this->root = nullptr;
}

Map::~Map() {
    // TODO: Free any dynamically allocated memory if necessary
    if (!root) return;

    std::stack<MapNode *> stack;
    stack.push(root);

    while (!stack.empty()) {
        MapNode *node = stack.top();
        stack.pop();

        if (node->left) stack.push(node->left);
        if (node->right) stack.push(node->right);

        delete node;
    }
}

void Map::initializeMap(std::vector<Isle *> isles) {
    // TODO: Insert innitial isles to the tree
    // Then populate with Goldium and Einstainium items
    for (auto &isle: isles) {
        insert(isle);
        reBalanceCount = 0;
    }
}

MapNode *Map::rotateRight(MapNode *current) {
    // TODO: Perform right rotation according to AVL
    // return necessary new root

    if (current == nullptr || !current->left) {
        std::cerr << "[Right Rotation] " << "Called on invalid node!" << std::endl;
        return nullptr;
    }

    MapNode *newHead = current->left;
    current->left = newHead->right;
    newHead->right = current;
    current->height = std::max(height(current->right), height(current->left)) + 1;
    newHead->height = std::max(height(newHead->right), height(newHead->left)) + 1;
    return newHead;
}

MapNode *Map::rotateLeft(MapNode *current) {
    // TODO: Perform left rotation according to AVL
    // return necessary new root

    if (current == nullptr || !current->right) {
        std::cerr << "[Left Rotation] " << "Called on invalid node!" << std::endl;
        return nullptr;
    }

    MapNode *newHead = current->right;
    current->right = newHead->left;
    newHead->left = current;
    current->height = std::max(height(current->right), height(current->left)) + 1;
    newHead->height = std::max(height(newHead->right), height(newHead->left)) + 1;
    return newHead;
}

int Map::calculateMinMapDepthAccess(int playerDepth, int totalShaperTreeHeight, int totalMapDepth) {
    return (int) totalMapDepth * ((double) playerDepth / totalShaperTreeHeight);
}

int Map::height(MapNode *node) {
    // TODO: Return height of the node
    return node ? node->height : 0;
}

int Map::getBalanceFactor(MapNode *node) {
    return node ? height(node->left) - height(node->right) : 0;
}

MapNode *Map::insert(MapNode *node, Isle *isle) {
    // MapNode *newNode = new MapNode(isle);
    //
    // // TODO: Recursively insert isle to the tree
    // // returns inserted node
    // insertRecursive(node, newNode);
    //
    // return newNode;

    if (!node) {
        return new MapNode(isle); // Create and return a new node
    }

    if (*isle < *(node->isle)) {
        node->left = insert(node->left, isle);
    } else if (*isle > *(node->isle)) {
        node->right = insert(node->right, isle);
    } else {
        return node;
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balanceFactor = getBalanceFactor(node);

    if (balanceFactor > 1 && *isle < *(node->left->isle)) {
        reBalanceCount++;
        return rotateRight(node);
    }
    if (balanceFactor < -1 && *isle > *(node->right->isle)) {
        reBalanceCount++;
        return rotateLeft(node);
    }
    if (balanceFactor > 1 && *isle > *(node->left->isle)) {
        reBalanceCount++;
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balanceFactor < -1 && *isle < *(node->right->isle)) {
        reBalanceCount++;
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

void Map::insert(Isle *isle) {
    if (root == nullptr) {
        root = new MapNode(isle);
        return;
    }
    root = insert((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
    if (reBalanceCount != 0 && reBalanceCount%3==0) {
        populateWithItems();
        dropItemBFS();
    }
}

MapNode *Map::findMax(MapNode *node) {
    while (node->right) {
        node = node->right;
    }
    return node;
}


void Map::remove(Isle *isle) {
    root = remove((root), isle);
    // you might need to insert some checks / functions here depending on your implementation
    if (reBalanceCount != 0 && reBalanceCount%3==0) {
        populateWithItems();
        dropItemBFS();
    }
}

MapNode *Map::remove(MapNode *node, Isle *isle) {
    if (!node) {
        std::cout << "[Remove] Tree is Empty or Node Not Found" << std::endl;
        return nullptr;
    }

    if (*isle < *(node->isle)) {
        node->left = remove(node->left, isle);
    } else if (*isle > *(node->isle)) {
        node->right = remove(node->right, isle);
    } else {
        if (!node->left || !node->right) {
            MapNode *temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        } else {
            MapNode *predecessor = findMax(node->left);
            if (predecessor) {
                Isle* toDel =  node->isle;
                node->isle = predecessor->isle;
                predecessor->isle = toDel;
                node->left = remove(node->left, predecessor->isle);
            }
        }
    }

    node->height = 1 + std::max(height(node->left), height(node->right));

    int balanceFactor = height(node->left) - height(node->right);

    if (balanceFactor > 1) {
        if (height(node->left) >= height(node->left->right)) {
            reBalanceCount++;
            return rotateRight(node);
        }
        if (height(node->left) < height(node->left->right)) {
            reBalanceCount++;
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }
    }
    if (balanceFactor < -1) {
        if (height(node->right) >= height(node->right->left)) {
            reBalanceCount++;
            return rotateLeft(node);
        }
        if (height(node->right) < height(node->right->left)) {
            reBalanceCount++;
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }
    }

    return node;
}


void Map::preOrderItemDrop(MapNode *current, int &count) {
    // TODO: Drop EINSTEINIUM according to rules

    if (!current) return;
    if (count % 5 == 0) {
        current->isle->setItem(EINSTEINIUM);
        std::cout << "[Item Drop] " << "EINSTEINIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
    count++;
    preOrderItemDrop(current->left, count);
    preOrderItemDrop(current->right, count);
}

// to Display the values by Post Order Method .. left - right - node
void Map::postOrderItemDrop(MapNode *current, int &count) {
    // TODO: Drop GOLDIUM according to rules

    if (!current) return;
    postOrderItemDrop(current->left, count);
    postOrderItemDrop(current->right, count);
    if (count % 3 == 0) {
        current->isle->setItem(GOLDIUM);
        std::cout << "[Item Drop] " << "GOLDIUM dropped on Isle: " << current->isle->getName() << std::endl;
    }
    count++;
}

MapNode *Map::findFirstEmptyIsle(MapNode *node) {
    // TODO: Find first Isle with no item
    // if (!node) return nullptr;
    //
    // MapNode* retNode = findFirstEmptyIsle(node->left);
    // if (retNode) return retNode;
    // if (node->isle->getItem() == EMPTY) return node;
    // return findFirstEmptyIsle(node->right);

    if (!node) return nullptr;
    std::queue<MapNode *> queue;
    queue.push(node);
    while (!queue.empty()) {
        MapNode *current = queue.front();
        if (current->isle->getItem() == EMPTY) {
            return current;
        }
        if (current->left) { queue.push(current->left); }
        if (current->right) { queue.push(current->right); }
        queue.pop();
    }
    return nullptr;
}

void Map::dropItemBFS() {
    // TODO: Drop AMAZONITE according to rules
    MapNode *firstIsle = findFirstEmptyIsle(root);
    if (firstIsle) {
        std::cout << "[BFS Drop] " << "AMAZONITE dropped on Isle: " << firstIsle->isle->getName() << std::endl;
        firstIsle->isle->setItem(AMAZONITE);
    } else { std::cout << "[BFS Drop] " << "No eligible Isle found for AMAZONITE drop." << std::endl; }
}

void Map::displayMap() {
    std::cout << "[World Map]" << std::endl;
    display(root, 0, 0);
}

int Map::getDepth(MapNode *node) {
    // TODO: Return node depth if found, else
    // if (node) { return height(root) - height(node); }
    // return -1;
    int depth = 0;
    MapNode *tmp = root;
    std::string name = node->isle->getName();
    while (tmp && tmp->isle->getName() != name) {
        if (name < tmp->isle->getName()) tmp = tmp->left;
        else tmp = tmp->right;
        depth++;
    }
    return tmp ? depth : -1;
}

// Function to calculate the depth of a specific node in the AVL tree
int Map::getIsleDepth(Isle *isle) {
    // TODO: Return node depth by isle if found, else
    MapNode *node = findNode(*isle);
    if (node) { return height(root) - height(node); }
    return -1;
}

int Map::getDepth() {
    // TODO: Return max|total depth of tree
    return height(root) - 1;
}

void Map::populateWithItems() {
    // TODO: Distribute fist GOLDIUM than EINSTEINIUM
    int count = 1;
    postOrderItemDrop(root, count);
    count = 1;
    preOrderItemDrop(root, count);
}

Isle *Map::findIsle(Isle isle) {
    // TODO: Find isle by value
    MapNode *tmp = root;
    while (tmp && tmp->isle->getName() != isle.getName()) {
        if (isle.getName() < tmp->isle->getName()) tmp = tmp->left;
        else tmp = tmp->right;
    }
    return tmp ? tmp->isle : nullptr;
}

Isle *Map::findIsle(std::string name) {
    // TODO: Find isle by name
    MapNode *tmp = root;
    while (tmp && tmp->isle->getName() != name) {
        if (name < tmp->isle->getName()) tmp = tmp->left;
        else tmp = tmp->right;
    }
    return tmp ? tmp->isle : nullptr;
}

MapNode *Map::findNode(Isle isle) {
    // TODO: Find node by value
    MapNode *tmp = root;
    while (tmp && tmp->isle->getName() != isle.getName()) {
        if (isle.getName() < tmp->isle->getName()) tmp = tmp->left;
        else tmp = tmp->right;
    }
    return tmp;
}

MapNode *Map::findNode(std::string name) {
    // TODO: Find node by name
    MapNode *tmp = root;
    while (tmp && tmp->isle->getName() != name) {
        if (name < tmp->isle->getName()) tmp = tmp->left;
        else tmp = tmp->right;
    }
    return tmp;
}

void Map::display(MapNode *current, int depth, int state) {
    // SOURCE:

    if (current->left)
        display(current->left, depth + 1, 1);

    for (int i = 0; i < depth; i++)
        printf("     ");

    if (state == 1) // left
        printf("   ┌───");
    else if (state == 2) // right
        printf("   └───");

    std::cout << "[" << *current->isle << "] - (" << current->height << ")\n"
            << std::endl;

    if (current->right)
        display(current->right, depth + 1, 2);
}

void Map::writeIsleNameToFile(const std::string &name, std::ofstream &outFile) {
    outFile << name;
}

void Map::writeToFile(const std::string &filename) {
    // TODO: Write the tree to filename output level by level
    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Map::writeToFile() - Could not open file " + filename);

    std::queue<MapNode *> queue;
    queue.push(root);

    int nodesInCurrentLevel = 1; // Nodes in the current level
    int nodesInNextLevel = 0; // Nodes in the next level
    bool hasNonNullNodeInCurrentLevel = false; // Flag to check if there's any non-null node in the current level
    std::vector<std::string> toWrite;

    while (!queue.empty()) {
        MapNode *current = queue.front();
        queue.pop();

        // Write the current node's name or '-' for missing nodes
        if (current) {
            toWrite.push_back(current->isle->getName());
            queue.push(current->left);
            queue.push(current->right);
            nodesInNextLevel += 2;
            hasNonNullNodeInCurrentLevel = true; // Found a non-null node
        } else {
            toWrite.push_back("NULL");
        }

        nodesInCurrentLevel--;

        // Insert a newline when the current level is fully processed
        if (nodesInCurrentLevel == 0) {
            if (hasNonNullNodeInCurrentLevel) {
                for (auto &name: toWrite) {
                    writeIsleNameToFile(name, file);
                    file << " ";
                }
                toWrite.clear();
                file << std::endl;
            }
            // Reset the flag and update counters for the next level
            hasNonNullNodeInCurrentLevel = false;
            nodesInCurrentLevel = nodesInNextLevel;
            nodesInNextLevel = 0;
        }
    }
}

void Map::writeInOrderToFile(MapNode *node, std::ofstream &outFile) {
    if (!node) return;
    writeInOrderToFile(node->left, outFile);
    writeIsleNameToFile(node->isle->getName(), outFile);
    outFile << std::endl;
    writeInOrderToFile(node->right, outFile);
}

void Map::writeIslesToFile(const std::string &filename) {
    // TODO: Write Isles to output file in alphabetical order

    std::ofstream file(filename);
    if (!file.is_open()) throw std::runtime_error("Map::writeIslesToFile() - Could not open file " + filename);
    writeInOrderToFile(root, file);

    std::cout << "[Output] " << "Isles have been written to " << filename << " in in alphabetical order." << std::endl;
}
