#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <string>
using namespace std;

class Subject {
  public:
    string name;
    string description;
    bool discovered;
    int lastInteraction;

    Subject ();
};

class NPC : public Subject {
  public:
    bool alive;

    NPC (string nameIn, string descriptionIn);
    NPC (string nameIn, string descriptionIn, bool discoveredIn, bool aliveIn);
};

class Location : public Subject {
  public:
    bool warrant;

    Location (string nameIn, string descriptionIn);
    Location (string nameIn, string descriptionIn, bool discoveredIn, bool warrantIn);
};

#endif