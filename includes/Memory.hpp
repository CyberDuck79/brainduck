/*      __     Memory.hpp
**  ___( o)>   Creation Date: 21/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/
#pragma once

#include <cinttypes>
#include <exception>
#include <string>
#include <sstream>

#define MEM_SIZE 16384

class Memory {
private:
    uint16_t _memory[MEM_SIZE];

    static void _bootScreen();

public:
    Memory();

    void check() const;
    class MemoryError : public std::exception {
    private:
        std::string	_msg;
    public:
        MemoryError(const uint16_t *ptr);
        const char *what() const noexcept;
    };

    uint16_t read(const uint16_t index) const;
    void write(const uint16_t index, const uint16_t value);
    class MemoryOverflow : public std::exception {
    private:
        std::string	_msg;
    public:
        MemoryOverflow(const uint16_t *ptr);
        const char *what() const noexcept;
    };
    void clear();

};