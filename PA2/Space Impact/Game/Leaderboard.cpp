#include "Leaderboard.h"


// Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
// variable will point to the highest all-times score, and all other scores will be reachable from it
// via the "next_leaderboard_entry" member variable pointer.
void Leaderboard::read_from_file(const string &filename) {
    // TODO: Your code here
    ifstream file(filename);
    if (file.is_open()) {
        string line;
        while (getline(file, line)) {
            if (!line.empty()) {
                istringstream iss(line);
                time_t time;
                string name;
                unsigned long score;

                if (iss >> score >> time >> name) {
                    insert(new LeaderboardEntry(score, time, name));
                }
            }
        }
        file.close();
    }
}


// Write the latest leaderboard status to the given file in the format specified in the PA instructions
void Leaderboard::write_to_file(const string &filename) {
    // TODO: Your code here
    std::ofstream file(filename);

    if (file.is_open()) {
        LeaderboardEntry *temp = head_leaderboard_entry;
        while (temp) {
            file << temp->score << " " << temp->last_played << " " << temp->player_name << "\n";
            temp = temp->next;
        }
        file.close();
    }
}

// Print the current leaderboard status to the standard output in the format specified in the PA instructions
void Leaderboard::print_leaderboard() {
    // TODO: Your code here
    cout << "Leaderboard" << endl << "-----------" << endl;
    LeaderboardEntry *temp = head_leaderboard_entry;
    int rank = 1;
    while (temp) {
        tm *time_info = localtime(&temp->last_played);
        char formatString[20];
        strftime(formatString, sizeof(formatString), "%H:%M:%S/%d.%m.%Y", time_info);
        cout << rank << ". " << temp->player_name << " " << temp->score << " " << formatString<< endl;
        temp = temp->next;
        rank++;
    }
}

//  Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
//  is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
//  top 10 all-time high-scores should be kept in descending order by the score).
void Leaderboard::insert(LeaderboardEntry *new_entry) {
    // TODO: Your code here
    LeaderboardEntry *temp = head_leaderboard_entry;
    LeaderboardEntry *beforeTmp = nullptr;
    while (temp != nullptr && new_entry->score <= temp->score) {
        beforeTmp = temp;
        temp = temp->next;
    }
    if (beforeTmp == nullptr) {
        new_entry->next = head_leaderboard_entry;
        head_leaderboard_entry = new_entry;
    } else {
        new_entry->next = temp;
        beforeTmp->next = new_entry;
    }

    currentSize++;

    if (currentSize > MAX_LEADERBOARD_SIZE) {
        beforeTmp = nullptr;
        temp = head_leaderboard_entry;
        while (temp != nullptr && temp->next != nullptr) {
            beforeTmp = temp;
            temp = temp->next;
        }
        if (beforeTmp != nullptr) {
            beforeTmp->next = nullptr;
            delete temp;
        }
    }
}


// Free dynamically allocated memory used for storing leaderboard entries
Leaderboard::~Leaderboard() {
    // TODO: Your code here
    LeaderboardEntry *temp = head_leaderboard_entry;
    while (head_leaderboard_entry != nullptr) {
        temp = temp->next;
        delete head_leaderboard_entry;
        head_leaderboard_entry = temp;
    }
}
