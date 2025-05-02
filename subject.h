#ifndef SUBJECT_H
#define SUBJECT_H

#include <vector>
#include <string>
using namespace std;

class Subject {
  public:
    string name;
    string description;
    int lastInteraction;

    Subject ();
};

class NPC : public Subject {
  public:
    bool alive;

    NPC (string nameIn);
};

class Location : public Subject {
  public:
    bool warrent;

    Location (string nameIn);
};

#endif