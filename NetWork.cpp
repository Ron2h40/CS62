#include "NetWork.h"
#include <fstream>
#include <iostream>

Network::Network() {
    // Constructor implementation (if needed)
}

Network::~Network() {
    // Destructor implementation
    // Ensure proper cleanup of dynamically allocated User objects
    for (User* user : users) {
        delete user;
    }
}
void Network::addUser(User* user) {
    // Implementation to add a User to the Network database
    users.push_back(user);
}




int Network::addConnection(const std::string& usr1, const std::string& usr2) {
    // Implementation to add a friend connection
    // Return 0 on success, -1 if either of the users are invalid
    bool checkU1 = false;
    bool checkU2 = false;
    for (auto checkUsr : users) {
        if (checkUsr->getName() == usr1) {
            checkU1 = true; //user1 is invalid
        }
        if (checkUsr->getName() == usr2) {
            checkU2 = true; //user2 is invalid
        }
    }
    if (checkU1 == false || checkU2 == false) {
        return -1; //either of the users are invalid
    }
    for (auto tempU : users) {
        if (tempU->getName() == usr1) {
            for (auto tempU2 : users) {
                if (tempU2->getName() == usr2) {
                    tempU->addFriend(tempU2->getId());
                    tempU2->addFriend(tempU->getId());
                }
            }
        }
    }



    return 0; // Return 0 on success
}

int Network::removeConnection(const std::string& usr1, const std::string& usr2) {
    // Implementation to remove a friend connection
    // Return 0 on success, -1 if either of the users are invalid
    bool checkU1 = false;
    bool checkU2 = false;
    for (auto checkUsr : users) {
        if (checkUsr->getName() == usr1) {
            checkU1 = true; //user1 is invalid
        }
        if (checkUsr->getName() == usr2) {
            checkU2 = true; //user2 is invalid
        }
    }
    if (checkU1 == false || checkU2 == false) {
        return -1; //either of the users are invalid
    }

    // Your logic to remove a friend connection should go here
    for (auto tempU : users) {
        if (tempU->getName() == usr1) {
            for (auto tempU2 : users) {
                if (tempU2->getName() == usr2) {
                    tempU->deleteFriend(tempU2->getId());
                    tempU2->deleteFriend(tempU->getId());
                }
            }
        }
    }

    return 0; // Return 0 on success
}

int Network::getId(const std::string& userName) const {
    for (auto usr : users) {
        if (usr->getName() == userName) {
            return usr->getId();
        }
    }

    return -1; // Return -1 if the user is not found
}

std::size_t Network::numUsers() const {
    return users.size();
}

User* Network::getUser(std::size_t id) const {
    for (auto usr : users) {
        if (usr->getId() == id) {
            return usr;
        }
    }
    return nullptr; // Return nullptr if the ID is out of bounds
}




int Network::writeUsers(const char* filename) {
    // Read the data from the input file first
    if (readUsers(filename) == -1) {
        // If reading fails, return -1 to indicate failure
        return -1;
    }

    // Open the output file for writing
    std::ofstream outputFile(filename);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open the output file." << std::endl;
        return -1; // Return -1 to indicate failure
    }

    // Write the number of users to the file
    outputFile << users.size() << std::endl;

    // Loop through each User and write their data to the file
    for (const User* user : users) {
        // Write user ID
        outputFile << user->getId() << std::endl;

        // Write user name
        outputFile << user->getName() << std::endl;

        // Write birth year
        outputFile << user->getBirthYear() << std::endl;

        // Write zip code
        outputFile << user->getZip() << std::endl;

        // Write friend IDs
        const std::vector<std::size_t>& friends = user->getFriends();
        for (std::size_t friendID : friends) {
            outputFile << friendID << " ";
        }
        outputFile << std::endl;
    }

    outputFile.close();
    return 0; // Return 0 on success
}

int Network::readUsers(const char* filename) {



    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open the input file." << std::endl;
        return -1; // Return -1 to indicate failure
    }

    std::size_t numUsers;
    std::string usrStr;
    std::getline(inputFile, usrStr);
    numUsers = std::stoull(usrStr);


    for (std::size_t i = 0; i < numUsers; ++i) {
        std::size_t id;
        std::string strId;
        std::string name;
        std::string strBirth;
        std::size_t birthYear;
        std::string strZip;
        std::size_t zip;
        std::vector<std::size_t> friends;

        // Read user ID
        std::getline(inputFile, strId);
        id = std::stoull(strId);
        // Read user name
        std::getline(inputFile, name);

        // Read birth year 
        std::getline(inputFile, strBirth);
        birthYear = std::stoull(strBirth);

        //Read zip
        std::getline(inputFile, strZip);
        zip = std::stoull(strZip);

        // Read friend IDs
        std::string friendsLine;
        std::getline(inputFile, friendsLine);
        std::istringstream ss(friendsLine);
        std::size_t friendID;
        while (ss >> friendID) {
            friends.push_back(friendID);
        }
        User* tempUsr = new User(id, name, birthYear, zip);
        users.push_back(tempUsr);
    }
    inputFile.close();
    return 0; // Return 0 on success
}



std::vector<std::size_t> Network::shortestPath(std::size_t from, std::size_t to) {
    // Use a graph traversal algorithm (e.g., Breadth-First Search) to find the shortest path
    std::vector<std::size_t> shortestPath;
    std::unordered_map<std::size_t, std::size_t> parentMap;
    std::queue<std::size_t> queue;
    std::unordered_set<std::size_t> visited;

    // Initialize the BFS traversal from the 'from' user
    queue.push(from);
    visited.insert(from);

    while (!queue.empty()) {
        std::size_t current = queue.front();
        queue.pop();

        if (current == to) {
            // We found the target user, reconstruct the shortest path
            std::size_t node = to;
            while (node != from) {
                shortestPath.push_back(node);
                node = parentMap[node];
            }
            shortestPath.push_back(from);
            std::reverse(shortestPath.begin(), shortestPath.end());
            return shortestPath;
        }

        // Traverse friends of the current user
        User* currentUser = getUser(current);
        const std::vector<std::size_t>& friends = currentUser->getFriends();
        for (std::size_t friendID : friends) {
            if (visited.find(friendID) == visited.end()) {
                visited.insert(friendID);
                parentMap[friendID] = current;
                queue.push(friendID);
            }
        }
    }

    // No path found
    return shortestPath;
}

std::vector<std::size_t> Network::distanceUser(std::size_t from, std::size_t& to, std::size_t distance) {
    std::vector<std::size_t> path;
    std::unordered_map<std::size_t, std::size_t> parentMap;
    std::queue<std::size_t> queue;
    std::unordered_set<std::size_t> visited;

    // Initialize the BFS traversal from the 'from' user
    queue.push(from);
    visited.insert(from);

    while (!queue.empty()) {
        if (distance == 0) {
            // We have reached the desired distance
            to = queue.front();
            std::size_t node = to;
            while (node != from) {
                path.push_back(node);
                node = parentMap[node];
            }
            path.push_back(from);
            std::reverse(path.begin(), path.end());
            return path;
        }

        // Continue BFS traversal
        std::size_t levelSize = queue.size();
        for (std::size_t i = 0; i < levelSize; ++i) {
            std::size_t current = queue.front();
            queue.pop();

            User* currentUser = getUser(current);
            const std::vector<std::size_t>& friends = currentUser->getFriends();
            for (std::size_t friendID : friends) {
                if (visited.find(friendID) == visited.end()) {
                    visited.insert(friendID);
                    parentMap[friendID] = current;
                    queue.push(friendID);
                }
            }
        }

        distance--;
    }

    // No user found at the specified distance
    to = -1;
    return path; // Empty vector
}