#pragma once

#include <vector>

class Command {
public:
    virtual ~Command();
    virtual void undo() = 0; // virtual function to undo an action
    virtual void redo() = 0; // virtual function to redo an action
};