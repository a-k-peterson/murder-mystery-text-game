# Murder on Command
A text based murder mystery game created by Abby Peterson Ebeling  
**This is a work in progress!**

### What it does
This is a murder mystery game that uses the command line for a user interface. It is meant to work with interchangeable story modules. The player has the ability to interview suspects, review CC TV footage, run background checks, and search locations in order to solve the mystery. There are no dialog options or skill checks, the challenge is that there is a limited amount of time. The player won't have time to interview everyone or search every location. Choose wisely!

### How it works
**main.cpp** contains the loop for each hour and each day. The player can take one action per hour, and has 8 hours per day for 3 days to solve the mystery.

**subject.cpp** defines Subject objects which can be NPCs or Locations. Each has a name and a description, as well as a "discovered" attribute which is set as true when the story introduces the person or place to the player. In order to streamline code, NPCs and Locations are able to be upcasted to Subjects when needed.

**game.cpp** contains all variables related to the game and current state of the game, as well as functions to handle those variables. Notable variables are the townsfolk vector and the locations vector. townsfolk is a list of all the NPCs in the game, and locations is a list of all the Locations in the game. These vectors can be populated by the new game file, or a saved game file.

### How to play
Compile and run instructions will go here.

### How to write a story for use by the game
Explanation of necessary file structure will go here.

