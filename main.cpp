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

  cout << "\nMURDER ON COMMAND\nBy Abby Peterson Ebeling\nInspired by Midsomer Murders, Vera, and Death in Paradise\n\n";

  // describe opener
  game.playCutscene("opener");

  // begin the investigation
  cout << "You have recently been promoted to Detective Inspector (DI) for the City of <city> and its outlying villages.\n";
  cout << "For your first case as a DI, you will be given " + to_string(game.days); 
  cout << " days to solve the case on your own before a Detective Chief Inspector (DCI) is brought in to assist you. ";
  cout << "Your goal is to prove your abilities by solving the case within " + to_string(game.days) + " days.\n";
  cout << "Let's begin!\n\n";

  // play "night before" cutscene (optional depending on story)
  game.playCutscene("night_before");

  // loop for each day
  while (game.day <= game.days) {
    // cutscene for beginning of each day
    cout << "\nMorning of day " + to_string(game.day) + ":\n";
    game.playCutscene("day_" + to_string(game.day) + "/morning");

    // loop for each hour in day
    while (game.hour <= game.hours && game.accused == "") {
      // choose an action
      int choice = game.chooseAction();
      switch (choice) {
        case 1: // interview someone
        {
            cout << "Who would you like to interview?\n";
            vector<Subject> canInterview;
            for (auto i : game.townsfolk) {
              if (i.alive && i.discovered) {
                canInterview.push_back(i);
              }
            }
            string selected = game.chooseSubject(canInterview);

            if (selected == "Nevermind") {
              break;
            }
            // interview the person selected
            string scene = "day_" + to_string(game.day) + "/" + selected;
            if (!game.playCutscene(scene)) {
              // if no file found -> they had nothing new to say
              cout << selected + " had nothing new to say...\n";
            }
            game.hour++;
            break;
        }

        case 2: // review CC TV footage
        {
          cout << "Which location's CC TV footage would you like to review?\n";
          vector<Subject> canViewCCTV;
          for (auto i : game.locations) {
            if (i.discovered) {
              canViewCCTV.push_back(i);
            }
          }
          string selected = game.chooseSubject(canViewCCTV);
          if (selected == "Nevermind") {
            break;
          }
          // review footage
          string scene = "day_" + to_string(game.day) + "/cctv_footage/" + selected + "_cctv";
          if (!game.playCutscene(scene)) {
            // if no file found -> there is no footage
            cout << selected + " has no CC TV cameras installed...\n";
          }
          game.hour++;
          break;
        }

        case 3: // run a background check
        {
            cout << "Who would you like to run a background check on?\n";
            vector<Subject> canBackgroundCheck;
            for (auto i : game.townsfolk) {
              if (i.discovered) {
                canBackgroundCheck.push_back(i);
              }
            }
            string selected = game.chooseSubject(canBackgroundCheck);
            if (selected == "Nevermind") {
              break;
            }
            // run a background check on the person selected
            string scene = "background_checks/" + selected + "_background";
            if (!game.playCutscene(scene)) {
              // if no file found -> not in the system
              cout << "There are no records of a \"" + selected + "\" in the system...\n";
            }
            game.hour++;
            break;
        }

        case 4: // search a location
        {
            cout << "Which location would you like to search?\n";
            vector<Subject> canSearch;
            for (auto i : game.locations) {
              if (i.warrant && i.discovered) {
                canSearch.push_back(i);
              }
            }
            string selected = game.chooseSubject(canSearch);
            if (selected == "Nevermind") {
              break;
            }
            // search the location selected
            string scene = "day_" + to_string(game.day) + "/search_warrents/" + selected + "_search";
            if (!game.playCutscene(scene)) {
              // if no file found -> you don't have a search warrent
              cout << "You don't have enough evidence to get a search warrent for " + selected + "...\n";
            }
            game.hour++;
            break;
        }

        case 5: // solve the case!
        {
            cout << "Who will you charge with murder?\n";
            vector<Subject> canCharge;
            for (auto i : game.townsfolk) {
              if (i.discovered) {
                canCharge.push_back(i);
              }
            }
            string selected = game.chooseSubject(canCharge);
            if (selected == "Nevermind") {
              break;
            }
            game.accused = selected;
            break;
        }

        case 6: // view inventory
        {
            cout << "Which item would you like to inspect?\n";
            vector<Subject> canInspect;
            for (auto i : game.items) {
              if (i.discovered) {
                canInspect.push_back(i);
              }
            }
            string selected = game.chooseSubject(canInspect);
            if (selected == "Nevermind") {
              break;
            }
            // inspect the item selected
            string scene = "items/" + selected;
            if (!game.playCutscene(scene)) {
              // if no file found -> error
              cout << "Error! No file " + selected + " found in items folder.\n";
            }
            //game.hour++;
            break;
        }

        case 0: // quit
        {
            cout << "Your progress will be saved. Are you sure you want to quit the game? (y/n)\n";
            string sure;
            getline(cin, sure);
            while (sure != "y" && sure != "n" && sure != "Y" && sure != "N") {
              cout << "Invalid entry. Please enter y to save and quit or n to continue playing.\n";
              getline(cin, sure);
            }
            if (sure == "y" || sure == "Y") {
              game.saveGame();
              cout << "Goodbye";
              return 0;
            }
            break;
        }
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