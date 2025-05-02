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
    this->townsfolk = {};   // known townsfolk - will grow as the game is played
    this->locations = {};   // known locations - will grow as the game is played
    this->accused = "";             // who the player accuses of murder
    this->murderer = getMurderer(); // murderer identity kept in text files to avoid spoilers
}

bool Game::playCutscene(string fileName) {
    bool add_townsfolk_flag = false;    // flag to add the following lines to townsfolk vectors
    bool add_locations_flag = false;    // flag to add the following lines to locations vectors
    vector<string> added_townsfolk;    // vector to hold names of only the new townsfolk that are not repeats
    vector<string> added_locations;    // vector to hold names of only the new locations that are not repeats

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
            NPC addMe(line); 
            townsfolk.push_back(addMe);
            added_townsfolk.push_back(line);
        }
        // add line to locations vectors (if flag is set, not a repeat, not a blank line, and not the flag set line)
        if (add_locations_flag && !alreadyAdded(line, locations) && line != "" && line != "New Locations:") {
            Location addMe(line);
            locations.push_back(addMe);
            added_locations.push_back(line);
        }
        if (!add_townsfolk_flag && !add_locations_flag) {
            cout << line << endl;       // print lines in cutscene
        }
        
    }
    myFile.close();

    // if there are new townsfolk, print only the ones that are not repeats
    if (!added_townsfolk.empty()) {
        cout << "New Townsfolk:\n";
        for (auto i: added_townsfolk) {
            cout << i << endl;
        }
    }
    
    // if there are new locations, print only the ones that are not repeats
    if (!added_locations.empty()) {
        cout << "New Locations:\n";
        for (auto i: added_locations) {
            cout << i << endl;
        }
    }

    return true;
}

bool Game::alreadyAdded(string str, vector<Subject> vect) {
    for (auto i: vect) {
        if (i.name == str) {
            return true;
        }
    }
    return false;
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

string Game::getMurderer() {
    string myText;
    fstream myFile("text_files/murderer.txt");
    getline (myFile, myText);
    myFile.close();
    return myText;
}
