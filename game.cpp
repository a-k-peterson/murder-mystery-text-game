#include "game.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

Game::Game () {
    //const int days = 3;   // total number of days in the game
    //const int hours = 8;  // number of hours per day
    this->day = 1;      // current day
    this->hour = 1;     // current hour
    this->townsfolk;    // all townsfolk in the game
    this->locations;    // all locations in the game
    this->accused = ""; // who the player accuses of murder
    this->murderer;     // murderer name

    getGameData();
}

void Game::getGameData() {
    ifstream file("game_files/game_data.json");
    json jsonData = json::parse(file);
    file.close();

    // get townsfolk from json file
    for (auto person : jsonData["townsfolk"]) {
        NPC npc(person["name"], person["description"]);
        townsfolk.push_back(npc);
    }

    // get locations from json file
    for (auto place : jsonData["locations"]) {
        Location location(place["name"], place["description"]);
        locations.push_back(location);
    }

    // get murderer from json file
    murderer = jsonData["murderer"];

}

bool Game::playCutscene(string fileName) {
    bool add_townsfolk_flag = false;   // flag to add the following lines to townsfolk vectors
    bool add_locations_flag = false;   // flag to add the following lines to locations vectors
    vector<string> added_townsfolk;    // vector to hold names of only the new townsfolk that are not repeats - for printing
    vector<string> added_locations;    // vector to hold names of only the new locations that are not repeats - for printing

    string line;
    fstream myFile("game_files/" + fileName + ".txt");
    if (!myFile.good()) {
        return false;
    }
    while (getline (myFile, line)) {
        if (line == "New Townsfolk:") { // if we read a line that says "New Townsfolk:"
            add_townsfolk_flag = true;  // set townsfolk flag
            add_locations_flag = false; // unset locations flag
        }
        if (line == "New Locations:") { // if we read a line that says "New Locations:"
            add_locations_flag = true;  // set locations flag
            add_townsfolk_flag = false; // unset townsfolk flag
        }
        // if flag is set, not a blank line, and not the flag set line
        if (add_townsfolk_flag && line != "" && line != "New Townsfolk:") {
            if (discoverIfNew(line, townsfolk)) {   // set discovered = true
                added_townsfolk.push_back(line);    // add to print vector 
            }
        }
        // if flag is set, not a blank line, and not the flag set line
        if (add_locations_flag && line != "" && line != "New Locations:") {
            if (discoverIfNew(line, locations)) {   // set discovered = true
                added_locations.push_back(line);    // add to print vector 
            }
        }
        if (!add_townsfolk_flag && !add_locations_flag) {
            cout << line << endl;       // print lines in cutscene
        }
        
    }
    myFile.close();

    // if there are new townsfolk, print only the ones that are not repeats
    if (!added_townsfolk.empty()) {
        cout << "\nNew Townsfolk:\n";
        for (auto i: added_townsfolk) {
            cout << i << endl;
        }
    }
    
    // if there are new locations, print only the ones that are not repeats
    if (!added_locations.empty()) {
        cout << "\nNew Locations:\n";
        for (auto i: added_locations) {
            cout << i << endl;
        }
    }

    return true;
}

bool Game::discoverIfNew(string str, vector<Subject> vect, bool people) {
    for (int i=0; i<vect.size(); i++) {
        if (vect.at(i).name == str) {
            if (vect.at(i).discovered == true) {
                return false;   // return false if already discovered
            }
            if (people) {
                townsfolk.at(i).discovered = true;
            } else {
                locations.at(i).discovered = true;
            }
            return true;        // return true if newly discovered
        }
    }
    // This name was not found in the vector - error!
    string v;
    if (people) {
        v = "townsfolk";
    } else {
        v = "locations";
    }
    cout << "\nERROR! " + str + " does not match any name in " + v + " list in game_data.json.\n";
    return false;
}

bool Game::discoverIfNew(string str, vector<NPC> vect) {
    // stand up vector
    // upcast NPCs to Subjects
    // Call subject implementation

    vector<Subject> upcasted;
    for (auto i: vect) {
        upcasted.push_back(i);
    }
    return discoverIfNew(str, upcasted, true);
}

bool Game::discoverIfNew(string str, vector<Location> vect) {
    // stand up vector
    // upcast Locations to Subjects
    // Call subject implementation
    
    vector<Subject> upcasted;
    for (auto i: vect) {
        upcasted.push_back(i);
    }
    return discoverIfNew(str, upcasted, false);
}

int Game::isValidChoice(string input, int min, int max) {
    if (input.length() != 1) {
        return -1;
    }
    if (!isdigit(input[0])) {
        return -1;
    }
    int inputInt = stoi(input);
    if (inputInt < min || inputInt > max) {
        return -1;
    }
    return inputInt;
}

int Game::chooseAction() {
    cout << "\n\nDay: " + to_string(day) + "/" + to_string(days);
    cout << "\tHour: " + to_string(hour) + "/" + to_string(hours) + "\n";
    cout << "What would you like to do?\n";
    cout << "1 - Interview someone\n";
    cout << "2 - Review CC TV footage\n";
    cout << "3 - Run a background check\n";
    cout << "4 - Search a location\n";
    cout << "5 - Solve the case!\n";
    cout << "0 - Quit\n";

    string choice;
    getline(cin, choice);
    while (isValidChoice(choice, 0, 5) == -1) {
        cout << "Invalid entry. Please enter a number between 0 and 5.\n";
        getline(cin, choice);
    }
    return isValidChoice(choice, 0, 5);
}

string Game::chooseSubject(vector<Subject> subjects) {
    for (int i=0; i<subjects.size(); i++) {
        cout << to_string(i+1) + " - " + subjects.at(i).name << endl;
    }
    cout << "0 - Nevermind\n";
    string choice;
    getline(cin, choice);
    while (isValidChoice(choice, 0, subjects.size()) == -1) {
        cout << "Invalid entry. Please enter a number between 0 and " + to_string(subjects.size()) + ".\n";
        getline(cin, choice);
    }
    int validChoice = isValidChoice(choice, 0, subjects.size());
    if (validChoice == 0) {
        return "Nevermind";
    }
    return subjects.at(validChoice-1).name;
}

string Game::chooseSubject(vector<NPC> vect) {
    // stand up vector
    // upcast NPCs to Subjects
    // Call subject implementation
    
    vector<Subject> upcasted;
    for (auto i: vect) {
        upcasted.push_back(i);
    }
    return chooseSubject(upcasted);
}

string Game::chooseSubject(vector<Location> vect) {
    // stand up vector
    // upcast Locations to Subjects
    // Call subject implementation
    
    vector<Subject> upcasted;
    for (auto i: vect) {
        upcasted.push_back(i);
    }
    return chooseSubject(upcasted);
}
