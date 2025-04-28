#include "game.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;


Game::Game () {
    //const int days = 3;   // total number of days in the game
    //const int hours = 8;  // number of hours per day
    this->day = 1;      // current day
    this->hour = 1;     // current hour
    this->locations = {};   // known locations - will grow as the game is played
    this->townsfolk = {};   // known townsfolk - will grow as the game is played
    this->accused = "";             // who the player accuses of murder
    this->murderer = getMurderer(); // murderer identity kept in text files to avoid spoilers
}

bool Game::playCutscene(string fileName) {
    bool add_townsfolk_flag = false;    // flag to add the following lines to townsfolk vectors
    bool add_locations_flag = false;    // flag to add the following lines to locations vectors
    vector<string> added_townsfolk;     // vector to hold only the new townsfolk that are not repeats
    vector<string> added_locations;     // vector to hold only the new locations that are not repeats

    string line;
    fstream myFile("text_files/" + fileName + ".txt");
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
        // add line to townsfolk vectors (if flag is set, not a repeat, not a blank line, and not the flag set line)
        if (add_townsfolk_flag && !alreadyAdded(line, townsfolk) && line != "" && line != "New Townsfolk:") {
            townsfolk.push_back(line);
            added_townsfolk.push_back(line);
        }
        // add line to locations vectors (if flag is set, not a repeat, not a blank line, and not the flag set line)
        if (add_locations_flag && !alreadyAdded(line, locations) && line != "" && line != "New Locations:") {
            locations.push_back(line);
            added_locations.push_back(line);
        }
        if (!add_townsfolk_flag && !add_locations_flag) {
            cout << line << endl;       // print lines in cutscene
        }
        
    }
    myFile.close();

    // print only the new townfolk that are not repeats
    cout << "New Townsfolk:\n";
    for (auto i: added_townsfolk) {
        cout << i << endl;
    }
    // print only the new locations that are not repeats
    cout << "New Locations:\n";
    for (auto i: added_locations) {
        cout << i << endl;
    }
    return true;
}

bool Game::alreadyAdded(string str, vector<string> vect) {
    for (auto i: vect) {
        if (i == str) {
            return true;
        }
    }
    return false;
}

string Game::selectATownsfolk() {
    for (int i=0; i<townsfolk.size(); i++) {
        cout << to_string(i+1) + " - " + townsfolk.at(i) << endl;
    }
    cout << to_string(townsfolk.size()+1) + " - Nevermind\n";
    int selectNum;
    cin >> selectNum;
    while (selectNum < 1 || selectNum > townsfolk.size()+1) {
        cout << "Invalid entry. Please enter a number between 1 and " + to_string(townsfolk.size()+1) + ".\n";
        cin >> selectNum;
    }
    if (selectNum == townsfolk.size()+1) {
        return "Nevermind";
    }
    return townsfolk.at(selectNum-1);
}

string Game::selectALocation() {
    for (int i=0; i<locations.size(); i++) {
        cout << to_string(i+1) + " - " + locations.at(i) << endl;
    }
    cout << to_string(locations.size()+1) + " - Nevermind\n";
    int selectNum;
    cin >> selectNum;
    while (selectNum < 1 || selectNum > locations.size()+1) {
        cout << "Invalid entry. Please enter a number between 1 and " + to_string(locations.size()+1) + ".\n";
        cin >> selectNum;
    }
    if (selectNum == locations.size()+1) {
        return "Nevermind";
    }
    return locations.at(selectNum-1);
}

string Game::getMurderer() {
    string myText;
    fstream myFile("text_files/murderer.txt");
    getline (myFile, myText);
    myFile.close();
    return myText;
}
