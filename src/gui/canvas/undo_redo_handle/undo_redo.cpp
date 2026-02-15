#include "gui/canvas.h"

void Canvas::undo() {
    if (undoStack.empty()) return;
    Command* commandPtr = undoStack.top().get(); // Get raw pointer to check type
    if (dynamic_cast<Paste*>(commandPtr)) pasteCount = std::max(0, pasteCount-1); // Adjust paste offset
    std::unique_ptr<Command> command = std::move(undoStack.top());
    undoStack.pop(); // Remove from stack
    command->undo(); // do undo logic
    redoStack.push(std::move(command)); // Push to redo stack
}

void Canvas::redo() {
    if (redoStack.empty()) return;
    Command* commandPtr = redoStack.top().get(); // Get raw pointer
    if (dynamic_cast<Paste*>(commandPtr)) pasteCount++; // Adjust paste offset
    std::unique_ptr<Command> command = std::move(redoStack.top()); // Move command out of stack
    redoStack.pop(); // Remove from stack
    command->redo(); // Do redo logic
    undoStack.push(std::move(command)); // Push back to undo stack
}