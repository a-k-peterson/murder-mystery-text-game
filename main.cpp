#include "game.h"

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

    Player () {
      //cout << "What is your player's name?\n";
      //cin >> name;
      //currentLocation = "Home";
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

  // loop for each day
  while (game.day <= game.days) {
    // cutscene for beginning of each day
    cout << "Morning of day " + to_string(game.day) + ":\n";
    game.playCutscene("morning_" + to_string(game.day));

    // loop for each hour in day
    while (game.hour <= game.hours && game.accused == "") {
      cout << "\n\nDay: " + to_string(game.day) + "/" + to_string(game.days);
      cout << "\tHour: " + to_string(game.hour) + "/" + to_string(game.hours) + "\n";
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
            string scene = "day_" + to_string(game.day) + "_" + selected;
            if (!game.playCutscene(scene)) {
              // they had nothing to say
              cout << selected + " had nothing new to say...\n";
            }
            game.hour++;
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
            string scene = "day_" + to_string(game.day) + "_" + selected;
            if (!game.playCutscene(scene)) {
              // there is nothing new to see
              cout << selected + " looks the same...\n";
            }
            game.hour++;
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
            game.hour++;
            break;
          }

        case 4:
          {
            cout << "Which person will you charge with murder?\n";
            string selected = game.selectATownsfolk();
            if (selected == "Nevermind") {
              break;
            }
            game.accused = selected;
            break;
          }
        case 5:
          return 0;

        default:
          cout << "Invalid entry. Please enter a number between 1 and 5.\n";
      }
    }
    // break if you are ready to accuse someone
    if (game.accused != "") {
      break;
    }
    // out of time for today
    cout << "Time to call it a day...\n";
    game.hour = 1;
    game.day++;
  }

  // Ending 1 - player ran out of time
  if (game.accused == "") {
    game.playCutscene("end_out_of_time");
    return 0;
  }

  // Ending 2 - player correctly solved the case!
  if (game.accused == game.murderer) {
    game.playCutscene("end_murderer_accused");
    return 0;
  }

  // Ending 3 - player accused an innocent townsfolk
  game.playCutscene("end_innocent_accused");
  return 0;

}