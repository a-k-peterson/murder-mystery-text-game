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

  cout << "\nMURDER ON COMMAND\nBy Abby Peterson Ebeling\nInspired by Midsomer Murders and Death in Paradise\n\n";

  // describe opener
  game.playCutscene("opener");

  // begin the investigation
  cout << "You have recently been promoted to Detective Inspector (DI) for the City of Bromsby and its outlying villages.\n";
  cout << "For your first case as a DI, you will be given " + to_string(game.days); 
  cout << " days to solve the case on your own before a Detective Chief Inspector (DCI) is brought in to assist you. ";
  cout << "Your goal is to prove your abilities by solving the case within " + to_string(game.days) + " days.\n";
  cout << "Let's begin!\n\n";

  // loop for each day
  while (game.day <= game.days) {
    // cutscene for beginning of each day
    cout << "Morning of day " + to_string(game.day) + ":\n";
    game.playCutscene("day_" + to_string(game.day) + "/morning");

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
            string scene = "day_" + to_string(game.day) + "/" + selected;
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
            string scene = "day_" + to_string(game.day) + "/" + selected;
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
          {
            cout << "Are you sure you want to quit the game? (y/n)\n";
            string sure;
            cin >> sure;
            while (sure != "y" && sure != "n") {
              cout << "Invalid entry. Please enter y to quit or n to continue.\n";
              cin >> sure;
            }
            if (sure == "y") {
              cout << "Goodbye";
              return 0;
            }
            break;
          }

        default:
          cout << "Invalid entry. Please enter a number between 1 and 5.\n";
      } // end switch
    } // end loop for each hour
    // break if you are ready to accuse someone
    if (game.accused != "") {
      break;
    }
    // out of time for today
    cout << "Time to call it a day...\n";
    game.hour = 1;
    game.day++;
    
  } // end loop for each day

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