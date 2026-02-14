#pragma once

#include <vector>

class Command {
public:
    virtual ~Command();
    virtual void undo() = 0;
    virtual void redo() = 0;
};