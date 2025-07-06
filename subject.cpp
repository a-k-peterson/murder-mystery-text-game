#include "subject.h"

#include <string>
using namespace std;


Subject::Subject () {
    this->name = "";
    this->description = "";
    this->discovered = false;
    this->lastInteraction = 0;
}

Subject::Subject (string nameIn, string descriptionIn) {
    this->name = nameIn;
    this->description = descriptionIn;
    this->discovered = false;
    this->lastInteraction = 0;
}

Subject::Subject (string nameIn, string descriptionIn, bool discoveredIn) {
    this->name = nameIn;
    this->description = descriptionIn;
    this->discovered = discoveredIn;
    this->lastInteraction = 0;
}

NPC::NPC (string nameIn, string descriptionIn) {
    this->name = nameIn;
    this->description = descriptionIn;
    this->discovered = false;
    this->lastInteraction = 0;
    this->alive = true;
}

NPC::NPC (string nameIn, string descriptionIn, bool discoveredIn, bool aliveIn) {
    this->name = nameIn;
    this->description = descriptionIn;
    this->discovered = discoveredIn;
    this->lastInteraction = 0;
    this->alive = aliveIn;
}

Location::Location (string nameIn, string descriptionIn) {
    this->name = nameIn;
    this->description = descriptionIn;
    this->discovered = false;
    this->lastInteraction = 0;
    this->warrant = false;
}

Location::Location (string nameIn, string descriptionIn, bool discoveredIn, bool warrantIn) {
    this->name = nameIn;
    this->description = descriptionIn;
    this->discovered = discoveredIn;
    this->lastInteraction = 0;
    this->warrant = warrantIn;
}