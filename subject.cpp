#include "subject.h"

#include <string>
using namespace std;


Subject::Subject () {
    this->name = "";
    this->description = "";
    this->discovered = false;
    this->lastInteraction = 0;
}

NPC::NPC (string nameIn, string descriptionIn) {
    this->name = nameIn;
    this->description = descriptionIn;
    this->discovered = false;
    this->lastInteraction = 0;
    this->alive = true;
}

Location::Location (string nameIn, string descriptionIn) {
    this->name = nameIn;
    this->description = descriptionIn;
    this->discovered = false;
    this->lastInteraction = 0;
    this->warrant = false;
}