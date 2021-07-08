/*      __     BrainMachine.cpp
**  ___( o)>   Creation Date: 11/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/

#include "../includes/BrainMachine.hpp"
#include "../includes/print.hpp"

BrainMachine::BrainMachine():
_memory(),
_program(),
_brain(_memory, _program) {
    _memory.check();
    std::cout << "Machine initialization complete." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int BrainMachine::load(const char *filename) {
    try {
        _program.open(filename);
        _program.check();
        _program.compile();
    } catch (const std::exception &e) {
        std::cout << CLEAR_LINE << DISPLAY_CURSOR;
        std::cerr << "Error: " << e.what() << std::endl;
        return (-1);
    }
    std::cout << "Program initialization complete." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return (0);
}

int BrainMachine::execute() {
    try {
        _brain.execute();
    } catch (const std::exception &e) {
        std::cout << CLEAR_LINE << DISPLAY_CURSOR;
        std::cerr << "Error: " << e.what() << std::endl;
        return (-1);
    }
    return (0);
}
