#ifndef ENTITY_H
#define ENTITY_H

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

class Entity {
public:
    virtual ~Entity() {}
    virtual string getId() const = 0;
    virtual string getDisplayName() const = 0;
    virtual void display() const = 0;
    virtual void displayTableHeader() const = 0;
    virtual void displayTableRow(int stt) const = 0;
    virtual string toFileString() const = 0;
    virtual void fromFileString(const string& line) = 0;
    virtual void inputData() = 0;
    virtual void updateData() = 0;
};

#endif
