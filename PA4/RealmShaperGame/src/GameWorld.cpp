#include "GameWorld.h"

GameWorld::GameWorld() : mapTree(), shaperTree() {
}

void GameWorld::initializeGame(std::vector<Isle *> places, std::vector<RealmShaper *> players) {
    shaperTree.initializeTree(players);
    mapTree.initializeMap(places);
    mapTree.populateWithItems();
}

Map &GameWorld::getMapTree() {
    return mapTree;
}

ShaperTree &GameWorld::getShaperTree() {
    return shaperTree;
}

bool GameWorld::hasAccess(RealmShaper *realmShaper, Isle *isle) {
    bool hasAccess = false;
    int shaperDepth = shaperTree.getDepth(realmShaper);
    if (shaperDepth < 0) {
        std::cout << "[Access Control] " << "RealmShaper not found!" << std::endl;
        return false;
    }
    if (!isle || !realmShaper) return false;
    int minDepth = mapTree.calculateMinMapDepthAccess(shaperDepth, shaperTree.getDepth(), mapTree.getDepth());
    hasAccess = minDepth <= mapTree.getDepth(mapTree.findNode(*isle));
    // TODO: Check if the realmShaper has access to explore the isle
    // Get necessary depth values
    // Use mapTree.calculateMinMapDepthAccess

    return hasAccess;
}

void GameWorld::exploreArea(RealmShaper *realmShaper, Isle *isle) {
    // TODO:
    // Check if realmShaper has access
    if (realmShaper->getName() == "Player8" ) {

    }
    if (!isle || !realmShaper) return;
    if (!hasAccess(realmShaper, isle)) {
        std::cout << "[Explore Area] " << realmShaper->getName() << " does not have access to explore area " << *isle <<
                std::endl;
        return;
    }

    std::string previousIsleName = shaperTree.currentIsles[realmShaper->getName()];
    if (previousIsleName != "NOWHERE") {
        Isle *previousIsle = mapTree.findIsle(previousIsleName);
        if (previousIsle) previousIsle->decreaseShaperCount();
    }

    realmShaper->collectItem(isle->getItem());
    shaperTree.currentIsles[realmShaper->getName()] = isle->getName();
    std::cout << "[Explore Area] " << realmShaper->getName() << " visited " << isle->getName() << std::endl;
    std::cout << "[Energy] " << realmShaper->getName() << "'s new energy level is " << realmShaper->getEnergyLevel() <<
            std::endl;

    if (isle->increaseShaperCount()) {
        std::cout << "[Owercrowding] " << isle->getName() << " self-destructed, it will be removed from the map" <<
                std::endl;
        mapTree.remove(isle);
    }


    // If realmShaper has access
    // Visit isle, 
    // collect item, 
    // check overcrowding for Isle, 
    // delete Isle if necessary


    // You will need to implement a mechanism to keep track of how many realm shapers are at an Isle at the same time
    // There are more than one ways to do this, so it has been left completely to you
    // Use shaperCount, but that alone will not be enough,
    // you will likely need to add attributes that are not currently defined
    // to RealmShaper or Isle or other classes depending on your implementation
}

void GameWorld::craft(RealmShaper *shaper, const std::string &isleName) {
    // TODO: Check energy and craft new isle if possible

    if (shaper->hasEnoughEnergy()) {
        std::cout << "[Energy] " << shaper->getName() << " has enough energy points: " << shaper->getEnergyLevel() <<
                std::endl;
        mapTree.insert(new Isle(isleName));
        shaper->loseEnergy();
        std::cout << "[Craft] " << shaper->getName() << " crafted new Isle " << isleName << std::endl;

    } else {
        std::cout << "[Energy] " << shaper->getName() << " does not have enough energy points: " << shaper->
                getEnergyLevel() << std::endl;
    }
}

void GameWorld::displayGameState() {
    // mapTree.displayMap();
    // shaperTree.displayTree();
}

// TODO: Implement functions to read and parse Access and Duel logs

void GameWorld::readDuelsFromFile(std::string fileName) {
    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints
    std::ifstream file(fileName);
    if (!file.is_open()) throw std::runtime_error("GameWorld::readDuelsFromFile() - Could not open file " + fileName);

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string playerName;
        int result;

        ss >> playerName >> result;
        duels.push(new Duel(playerName, result));
    }

    file.close();
}

void GameWorld::readAccessesFromFile(std::string fileName) {
    // TODO: Read RealmShapers from the file,
    // add them to vector
    // return the vector
    // Input format: playerName[tab]honourPoints
    std::ifstream file(fileName);
    if (!file.is_open())
        throw
                std::runtime_error("GameWorld::readAccessesFromFile() - Could not open file " + fileName);

    std::string line;
    std::getline(file, line);
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::istringstream ss(line);
        std::string playerName;
        std::string isleName;

        ss >> playerName >> isleName;
        accesses.push(new Access(playerName, isleName));
    }

    file.close();
}

void GameWorld::processGameEvents(const std::string &accessLogs, const std::string &duelLogs) {
    // TODO:
    // Read logs
    // For every 5 access, 1 duel happens
    // If there are still duel logs left after every access happens duels happens one after other
    readAccessesFromFile(accessLogs);
    readDuelsFromFile(duelLogs);

    int count = 0;
    while (!accesses.empty()) {
        count++;
        Access *access = accesses.front();
        std::string player = access->player;
        std::string isleName = access->isleName;
        Isle *isle = mapTree.findIsle(isleName);
        RealmShaper *realmShaper = shaperTree.findPlayer(player);

        if (!isle) craft(realmShaper, isleName);
        else { exploreArea(realmShaper, isle); }

        delete access;
        accesses.pop();
        displayGameState();
        if (count % 5 == 0) {
            Duel *duel = duels.front();
            player = duel->player;
            bool result = duel->result == 1;
            realmShaper = shaperTree.findPlayer(player);
            shaperTree.duel(realmShaper, result);
            if (shaperTree.playerDeleted) {
                shaperTree.playerDeleted = false;
                std::string playerIsleName = shaperTree.deletedPlayersIsle;
                if (playerIsleName != "NOWHERE") mapTree.findIsle(playerIsleName)->decreaseShaperCount();
            }
            delete duel;
            duels.pop();
            displayGameState();
        }
    }

    while (!duels.empty()) {
        Duel *duel = duels.front();
        std::string player = duel->player;
        bool result = duel->result == 1;
        RealmShaper *realmShaper = shaperTree.findPlayer(player);
        shaperTree.duel(realmShaper, result);

        delete duel;
        duels.pop();
        displayGameState();
    }
    // This function should call exploreArea and craft functions

    // Use displayGameState();
}

void GameWorld::saveGameState(const std::string &currentIsles, const std::string &currentWorld,
                              const std::string &currentShapers, const std::string &currentPlayerTree) {
    mapTree.writeIslesToFile(currentIsles);
    mapTree.writeToFile(currentWorld);
    shaperTree.writeToFile(currentPlayerTree);
    shaperTree.writeShapersToFile(currentShapers);
}
