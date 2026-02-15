cmake_minimum_required(VERSION 3.16)

project(VectorEditor VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 1. Setup Qt6 (Core, Gui, Widgets are required for the Editor)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# 2. Enable automatic processing of Qt macros (MOC, UIC, RCC)
# This is crucial for Signals/Slots in your GUI classes to work.
qt_standard_project_setup()

# 3. Define your source files explicitly
# Grouping them makes it easier to read and manage.
set(SOURCES
    src/main.cpp
    
    # Model (Logic)
    src/model/circle.cpp
    src/model/rect.cpp
    src/model/line.cpp
    src/model/graphics_object.cpp
    src/model/freehand.cpp
    src/model/hexagon.cpp
    src/model/text.cpp
    
    # IO (Parsing/Saving)
    src/io/xml_parser.cpp
    src/io/svg_writer.cpp
    
    # GUI (Interface)
    src/gui/main_window/main_window.cpp
    src/gui/canvas/canvas.cpp
    src/gui/canvas/mouse_events/mouse_press_event.cpp
    src/gui/canvas/mouse_events/mouse_move_event.cpp
    src/gui/canvas/mouse_events/mouse_release_event.cpp
    src/gui/canvas/mouse_events/mouse_double_click_event.cpp
    src/gui/canvas/mouse_events/wheel_event.cpp
    src/gui/canvas/undo_redo_handle/undo_redo.cpp
    src/gui/property_panel/property_panel.cpp
    src/gui/property_panel/color_section.cpp
    src/gui/property_panel/set_target.cpp
    src/gui/property_panel/setup.cpp
    src/gui/property_panel/rectangle_connections.cpp
    src/gui/property_panel/circle_connections.cpp
    src/gui/property_panel/text_connections.cpp
    src/gui/property_panel/hex_connections.cpp
    src/gui/color_spectrum_widget/color_spectrum_widget.cpp
    src/gui/color_spectrum_widget/setup.cpp
    src/gui/color_spectrum_widget/mouse_events.cpp
    src/gui/selection_handle.cpp
    src/commands/command.cpp
    src/commands/add_shape.cpp
    src/commands/move.cpp
    src/commands/delete.cpp
    src/commands/paste.cpp
    src/commands/cut.cpp
    src/commands/property_change.cpp
    src/gui/canvas/key_press_event/key_press_event.cpp
    src/gui/canvas/shape_organizer/shape_organizer.cpp
    src/gui/canvas/commands/bring_to_front.cpp
    src/gui/canvas/commands/send_to_back.cpp
    src/gui/canvas/commands/delete_shapes.cpp
    src/gui/canvas/commands/cut.cpp
    src/gui/canvas/commands/copy.cpp
    src/gui/canvas/commands/paste.cpp
    src/gui/canvas/commands/others.cpp
    src/gui/main_window/commands.cpp
    src/gui/main_window/connections.cpp
    src/gui/main_window/other_functions.cpp
    src/model/functions/clone.cpp
    src/model/functions/draw.cpp
    src/model/functions/get_position.cpp
    src/model/functions/move.cpp
    src/model/functions/to_svg.cpp
    src/model/functions/resize/circle.cpp
    src/model/functions/resize/rect.cpp
    src/model/functions/resize/line.cpp
    src/model/functions/resize/hexagon.cpp
    src/model/functions/resize/text.cpp

    # Header Files
    include/gui/main_window.h
    include/gui/canvas.h
    include/gui/property_panel.h
    include/gui/color_spectrum_widget.h
    include/gui/selection_handle.h
)

# 4. Create the executable with the sources listed above
qt_add_executable(VectorEditor ${SOURCES})

# 5. Connect the 'include' directory
# This allows you to write #include "model/circle.h" instead of "../../include/model/circle.h"
target_include_directories(VectorEditor PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 6. Link Qt libraries
target_link_libraries(VectorEditor PRIVATE Qt6::Widgets)

# MacOS Bundle settings (Standard practice)
set_target_properties(VectorEditor PROPERTIES
    MACOSX_BUNDLE ON
)