#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using namespace std;

class Player {
  public:
    string name;
    string currentLocation;
    vector<string> inventory;
    int energy;
    int fullEnergy;

    Player () {
      //cout << "What is your player's name?\n";
      //cin >> name;
      //currentLocation = "Home";
      energy = 10;
      fullEnergy = 10;
    }
};

class Game {
  public:
    int days;
    vector<string> locations;
    vector<string> townsfolk;

    Game () {
      days = 3;
    }

    bool playCutscene(string fileName) {
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

    void addLocations(string fileName) {
      string myText;
      fstream myFile(fileName);
      while (getline (myFile, myText)) {
        locations.push_back(myText);
      }
      myFile.close();
    }

    void addTownsfolk(string fileName) {
      string myText;
      fstream myFile(fileName);
      while (getline (myFile, myText)) {
        townsfolk.push_back(myText);
      }
      myFile.close();
    }

    string selectATownsfolk() {
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

    string selectALocation() {
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

};

int main() {
  // init
  Game game;
  Player player;

  cout << "\nCOOL TITLE OF GAME\nBy Abby Peterson\nInspired by Midsomer Murders and Death in Paradise\n\n";

  // describe opener
  cout << "A spooky and intriguing opening...\n\n";

  // begin the investigation
  cout << "You are a Detective Chief Inspector (DCI) for the City of Bromsby and its outlying villages.\n";
  cout << "You are expected to solve any murders that occur within 3 days. You have time for 10 actions each day.\n";
  cout << "Let's begin!\n\n";

  string accused = "";
  for (int day=1; day<=game.days; day++) {
    // cutscene for beginning of each day
    cout << "Morning of day " + to_string(day) + ":\n";
    game.playCutscene("morning_" + to_string(day));

    // 10 turns each day to spend as the player wishes
    while (player.energy > 0 && accused == "") {
      cout << "\n\nIt is day " + to_string(day) + " of " + to_string(game.days);
      cout << " and you have " + to_string(player.energy) + "/" + to_string(player.fullEnergy) + " actions left today.\n";
      cout << "What would you like to do?\n";
      cout << "1 - Interview someone\n";
      cout << "2 - Inspect a location\n";
      cout << "3 - Background check someone\n";
      cout << "4 - Solve the case!\n";
      cout << "5 - Quit\n";
      int choice;
      cin >> choice;
      switch (choice) {
        case 1:
          {
            cout << "Who would you like to interview?\n";
            string selected = game.selectATownsfolk();
            if (selected == "Nevermind") {
              break;
            }
            // interview the person selected
            string scene = "day_" + to_string(day) + "_" + selected;
            if (!game.playCutscene(scene)) {
              // they had nothing to say
              cout << selected + " had nothing new to say...\n";
            }
            player.energy--;
            break;
          }

        case 2:
          {
            cout << "Which location would you like to inspect?\n";
            string selected = game.selectALocation();
            if (selected == "Nevermind") {
              break;
            }
            // inspect the location selected
            string scene = "day_" + to_string(day) + "_" + selected;
            if (!game.playCutscene(scene)) {
              // there is nothing new to see
              cout << selected + " looks the same...\n";
            }
            player.energy--;
            break;
          }

        case 3:
          {
            cout << "Which person would you like to run a background check on?\n";
            string selected = game.selectATownsfolk();
            if (selected == "Nevermind") {
              break;
            }
            // run a background check on the person selected
            player.energy--;
            break;
          }

        case 4:
          {
            cout << "Which person will you charge with murder?\n";
            string selected = game.selectATownsfolk();
            if (selected == "Nevermind") {
              break;
            }
            accused = selected;
            break;
          }
        case 5:
          return 0;

        default:
          cout << "Invalid entry. Please enter a number between 1 and 5.\n";
      }
    }
    // break if you are ready to accuse someone
    if (accused != "") {
      break;
    }
    // out of energy for the day
    cout << "Time to call it a day...\n";
    player.energy = player.fullEnergy;
    
  }

  if (accused == "") {
    // you have run out of time to solve the mystery
    cout << "Out of time :(\n";
    return 0;
  }
  // so you think you've solved it
  cout << "So you think you've solved it...";

  return 0;
}