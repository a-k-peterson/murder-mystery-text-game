#include "game.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

inline const char * const boolToString(bool b) {
  return b ? "true" : "false";
}

Game::Game () {
    //const int days = 3;   // total number of days in the game
    //const int hours = 8;  // number of hours per day
    this->day;      // current day
    this->hour;     // current hour
    this->townsfolk;    // all townsfolk in the game
    this->locations;    // all locations in the game
    this->accused = ""; // who the player accuses of murder
    this->murderer;     // murderer name

    getGameData();
}

void Game::getGameData() {
    ifstream saveFile(saveGameFileName);
    if (!saveFile.good()) {
        // There is no saved game, load new game state
        loadGameState(newGameFileName);
        return;
    }
    saveFile.close();

    // There is a save file - ask if they want to load it
    cout << "\nLoad saved game or start a new game? (Starting a new game will overwrite the saved game!)\n";
    cout << "1 - Load saved game\n";
    cout << "2 - Start new game\n";
    string choice;
    getline(cin, choice);
    while (isValidChoice(choice, 1, 2) == -1) {
        cout << "Invalid entry. Please enter a number between 1 and 2.\n";
        getline(cin, choice);
    }
    switch (isValidChoice(choice, 1, 2)) {
        case 1: // Load saved game
        {
            loadGameState(saveGameFileName);
            break;
        }

        case 2: // Start new game - will overwrite old save file upon saving
        {
            loadGameState(newGameFileName);
            break;
        }
    }

}

void Game::loadGameState(string fileName) {
    ifstream gameDataFile(fileName);
    json gameData = json::parse(gameDataFile);
    gameDataFile.close();

    // get day
    day = gameData["day"];

    // get hour
    hour = gameData["hour"];

    // get townsfolk from json file
    for (auto person : gameData["townsfolk"]) {
        if (fileName == newGameFileName) {
            NPC npc(person["name"], person["description"]);
            townsfolk.push_back(npc);
        } else {
            NPC npc(person["name"], person["description"], person["discovered"], person["alive"]);
            townsfolk.push_back(npc);
        }
    }

    // get locations from json file
    for (auto place : gameData["locations"]) {
        if (fileName == newGameFileName) {
            Location location(place["name"], place["description"]);
            locations.push_back(location);
        } else {
            Location location(place["name"], place["description"], place["discovered"], place["warrant"]);
            locations.push_back(location);
        }
    }

    // get murderer from json file
    murderer = gameData["murderer"];

}

void Game::saveGame() {
    cout << "Saving...\n";
    json saveData;

    // save day
    saveData["day"] = day;

    // save hour
    saveData["hour"] = hour;

    // save townsfolk vector
    json jTownsfolk;
    for (auto i : townsfolk) {
        string s = "{\"name\": \"" + i.name + "\"," +
                    "\"description\": \"" + i.description + "\"," +
                    "\"discovered\": " + boolToString(i.discovered) + "," +
                    "\"alive\": " + boolToString(i.alive) + "}";
        jTownsfolk.push_back(json::parse(s));
    }
    saveData["townsfolk"] = jTownsfolk;

    // save locations vector
    json jLocations;
    for (auto i : locations) {
        string s = "{\"name\": \"" + i.name + "\"," +
                    "\"description\": \"" + i.description + "\"," +
                    "\"discovered\": " + boolToString(i.discovered) + "," +
                    "\"warrant\": " + boolToString(i.warrant) + "}";
        jLocations.push_back(json::parse(s));
    }
    saveData["locations"] = jLocations;

    // save murderer
    saveData["murderer"] = murderer;

    // write the json data to save file - whatever was in the save file previously will be overwritten
    ofstream saveFile(saveGameFileName);
    saveFile << setw(4) << saveData << endl;
    saveFile.close();

}

bool Game::playCutscene(string fileName) {
    string newTownsfolkLine = "New Townsfolk:";
    string newLocationsLine = "New Locations:";
    string newlyDeceasedLine = "Newly Deceased:";

    string flag = "";
    int flagInt = 0;

    vector<string> newTownsfolk;    // list of new townsfolk for printing
    vector<string> newLocations;    // list of new locations for printing
    vector<string> newlyDeceased;   // list of newly deceased for printing

    string line;
    fstream cutsceneFile("game_files/" + fileName + ".txt");
    if (!cutsceneFile.good()) {
        return false;
    }
    while (getline (cutsceneFile, line)) {
        if (line == newTownsfolkLine) {
            flag = newTownsfolkLine;
            flagInt = 1;
        } else if (line == newLocationsLine) {
            flag = newLocationsLine;
            flagInt = 2;
        } else if (line == newlyDeceasedLine) {
            flag = newlyDeceasedLine;
            flagInt = 3;
        }

        if (flag == "") {                               // it's a regular line before the info section, just print it
            cout << line << endl;
            continue;
        } else if (line == flag || line == "") {        // it's the flag set line, or a blank line, ignore it
            continue;
        } 

        switch (flagInt) {                              // it's a piece of info that needs to be added
            case 1: // new townsfolk
            {
                if (discoverIfNew(line, townsfolk)) {   // set discovered = true
                    newTownsfolk.push_back(line);       // add to print vector 
                }
                break;
            }
    
            case 2: // new locations
            {
                if (discoverIfNew(line, locations)) {   // set discovered = true
                    newLocations.push_back(line);       // add to print vector 
                }
                break;
            }

            case 3: // newly deceased
            {
                murder(line);                           // set alive = false
                newlyDeceased.push_back(line);          // add to print vector 
                break;
            }
        }
    }
    cutsceneFile.close();

    // if there are new townsfolk, print them
    if (!newTownsfolk.empty()) {
        cout << endl << newTownsfolkLine << endl;
        for (auto i: newTownsfolk) {
            cout << i << endl;
        }
    }
    
    // if there are new locations, print them
    if (!newLocations.empty()) {
        cout << endl << newLocationsLine << endl;
        for (auto i: newLocations) {
            cout << i << endl;
        }
    }

    // if there are newly deceased, print them (assume no need to check for repeats)
    if (!newlyDeceased.empty()) {
        cout << endl << newlyDeceasedLine << endl;
        for (auto i: newlyDeceased) {
            cout << i << endl;
        }
    }

    return true;
}

void Game::murder(string name) {
    for (int i=0; i<townsfolk.size(); i++) {
        if (townsfolk[i].name == name) {
            townsfolk[i].alive = false;
        }
    }
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
        cout << to_string(i+1) + " - " + subjects.at(i).name;
        if (subjects.at(i).description == "") {
            cout << endl;
        } else {
            cout << " (" + subjects.at(i).description + ")\n";
        }
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
