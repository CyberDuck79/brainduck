/*      __     Memory.cpp
**  ___( o)>   Creation Date: 21/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/

#include "../includes/Memory.hpp"
#include "../includes/print.hpp"

void Memory::_bootScreen() {
    std::cout << CLEAR_SCREEN;
    std::cout << "Virtual Brainfuck 16bits Machine" << std::endl;
    std::cout << "CPU frequency: 2Mhz" << std::endl;
    std::cout << "RAM size: 16ko" << std::endl;
    std::cout << "Disk size: 64ko" << std::endl;
    std::cout << "Network: no module detected" << std::endl;
    std::cout << "Manufacturer: Cyberduck Computers" << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Memory::check() const {
    std::cout << "Memory check..." << std::endl;
    std::cout << HIDE_CURSOR;
    for (size_t count = 0; count < MEM_SIZE; count++) {
        if (_memory[count])
            throw MemoryError(&_memory[count]);
        printf("Memory check: [%lu/%i]\n", count, MEM_SIZE);
        std::this_thread::sleep_for(std::chrono::microseconds(60));
        std::cout << RESET_LINE;
    }
    std::cout << DISPLAY_CURSOR;
    std::cout << std::endl << "Memory check OK!" << std::endl;
}

Memory::MemoryError::MemoryError(const uint16_t *ptr) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Virtual Memory error at address: 0x";
    _msgConstructor << std::uppercase << std::hex << (long)ptr;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Memory::MemoryError::what() const noexcept {
    return (_msg.c_str());
}

Memory::Memory(): _memory() {
    _bootScreen();
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

uint16_t Memory::read(const uint16_t index) const {
    if (index >= MEM_SIZE)
        throw MemoryOverflow(&_memory[index]);
    return (_memory[index]);
}

void Memory::write(const uint16_t index, const uint16_t value) {
    if (index >= MEM_SIZE)
        throw MemoryOverflow(&_memory[index]);
    _memory[index] = value;
}

Memory::MemoryOverflow::MemoryOverflow(const uint16_t *ptr) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Virtual Memory overflow at address: 0x";
    _msgConstructor << std::uppercase << std::hex << (long)ptr;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Memory::MemoryOverflow::what() const noexcept {
    return (_msg.c_str());
}

void Memory::clear() {
    bzero(_memory, MEM_SIZE);
}
