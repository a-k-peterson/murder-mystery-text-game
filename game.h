#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
using namespace std;

class Game {
  public:
    const int days = 3;   // total number of days in the game
    const int hours = 8;  // number of hours per day
    int day;          // current day
    int hour;         // current hour
    vector<string> locations; // known locations - will grow as the game is played
    vector<string> townsfolk; // known townsfolk - will grow as the game is played
    string accused;   // who the player accuses of murder
    string murderer;  // murderer identity kept in text files to avoid spoilers

    Game ();

    bool playCutscene(string fileName);
    bool alreadyAdded(string str, vector<string> vect);
    int isValidChoice(string input, int min, int max);
    int chooseAction();
    string selectATownsfolk();
    string selectALocation();
    string getMurderer();

};

#endif