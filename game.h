#ifndef GAME_H
#define GAME_H

#include "subject.h"
#include <vector>
#include <string>
using namespace std;

#include "nlohmann/json.hpp"
using json = nlohmann::json;

class Game {
  public:
    const string newGameFileName = "game_files/game_data.json";
    const string saveGameFileName = "game_files/save_data/save_A.json";
    const int days = 3;   // total number of days in the game
    const int hours = 8;  // number of hours per day
    int day;          // current day
    int hour;         // current hour
    vector<NPC> townsfolk;       // all townsfolk in the game
    vector<Location> locations;  // all locations in the game
    vector<Subject> items;       // all items in the game
    string accused;   // who the player accuses of murder
    string murderer;  // who the actual murderer is

    Game ();
    
    void getGameData();
    void loadGameState(string fileName);
    void saveGame();
    
    bool playCutscene(string fileName);

    void murder(string name);

    void discoverItem(string name);

    bool discoverIfNew(string str, vector<Subject> vect, bool people);
    bool discoverIfNew(string str, vector<NPC> vect);
    bool discoverIfNew(string str, vector<Location> vect);

    int isValidChoice(string input, int min, int max);
    int chooseAction();

    string chooseSubject(vector<Subject>);
    string chooseSubject(vector<NPC>);
    string chooseSubject(vector<Location>);

};

#endif