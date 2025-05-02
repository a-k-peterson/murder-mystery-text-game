#include "subject.h"

#include <string>
using namespace std;


Subject::Subject () {
    this->name = "";
    this->description = "";
    this->lastInteraction = 0;
}

NPC::NPC (string nameIn) {
    this->name = nameIn;
    this->description = "";
    this->lastInteraction = 0;
    this->alive = true;
}

Location::Location (string nameIn) {
    this->name = nameIn;
    this->description = "";
    this->lastInteraction = 0;
    this->warrent = false;
}