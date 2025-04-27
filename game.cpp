#include "game.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;


Game::Game () {
    this->days = 3;
    this->locations = {};
    this->townsfolk = {};
}

bool Game::playCutscene(string fileName) {
    string myText;
    fstream myFile("text_files/" + fileName + ".txt");
    if (!myFile.good()) {
        return false;
    }
    while (getline (myFile, myText)) {
        cout << myText << endl;
    }
    myFile.close();
    // add new locations discovered in cutscene
    addLocations("text_files/" + fileName + "_new_locations.txt");
    // add new townsfolk discovered in cutscene
    addTownsfolk("text_files/" + fileName + "_new_townsfolk.txt");
    return true;
}

void Game::addLocations(string fileName) {
    string myText;
    fstream myFile(fileName);
    while (getline (myFile, myText)) {
        locations.push_back(myText);
    }
    myFile.close();
}

void Game::addTownsfolk(string fileName) {
    string myText;
    fstream myFile(fileName);
    while (getline (myFile, myText)) {
        townsfolk.push_back(myText);
    }
    myFile.close();
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
