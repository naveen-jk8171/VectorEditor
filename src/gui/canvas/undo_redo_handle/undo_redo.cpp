#include "gui/canvas.h"

void Canvas::undo() {
    if (undoStack.empty()) return;
    Command* commandPtr = undoStack.top().get();
    if (dynamic_cast<Paste*>(commandPtr)) pasteCount = std::max(0, pasteCount-1);
    std::unique_ptr<Command> command = std::move(undoStack.top());
    undoStack.pop();
    command->undo();
    redoStack.push(std::move(command));
}

void Canvas::redo() {
    if (redoStack.empty()) return;
    Command* commandPtr = redoStack.top().get();
    if (dynamic_cast<Paste*>(commandPtr)) pasteCount++;
    std::unique_ptr<Command> command = std::move(redoStack.top());
    redoStack.pop();
    command->redo();
    undoStack.push(std::move(command));
}