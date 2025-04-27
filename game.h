#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
using namespace std;

class Game {
  public:
    int days;
    vector<string> locations;
    vector<string> townsfolk;

    Game ();

    bool playCutscene(string fileName);
    void addLocations(string fileName);
    void addTownsfolk(string fileName);
    string selectATownsfolk();
    string selectALocation();

};

#endif