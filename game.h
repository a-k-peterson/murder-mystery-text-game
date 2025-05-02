#ifndef GAME_H
#define GAME_H

#include "subject.h"
#include <vector>
#include <string>
using namespace std;

class Game {
  public:
    const int days = 3;   // total number of days in the game
    const int hours = 8;  // number of hours per day
    int day;          // current day
    int hour;         // current hour
    vector<Subject> townsfolk;  // known townsfolk - will grow as the game is played
    vector<Subject> locations;  // known locations - will grow as the game is played
    string accused;   // who the player accuses of murder
    string murderer;  // murderer identity kept in text files to avoid spoilers

    Game ();

    bool playCutscene(string fileName);
    bool alreadyAdded(string str, vector<Subject> vect);
    int isValidChoice(string input, int min, int max);
    int chooseAction();
    string chooseSubject(vector<Subject>);
    string getMurderer();

};

#endif