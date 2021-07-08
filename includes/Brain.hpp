/*      __     Brain.hpp
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
#include "Memory.hpp"
#include "Program.hpp"
#include <stack>
#include <list>

class Brain {
private:
    Memory &_memory;
    Program &_program;

    struct State {
        State(std::map<char, uint16_t> rootLabelMap):
        pointer(0),
        counter(0),
        output("Output:\n"),
        labelMap(rootLabelMap) {}

        uint16_t pointer;
        uint16_t counter;
        std::string	output;
        std::map<char, uint16_t> registers;
        std::map<char, uint16_t> labelMap;
        std::stack<std::map<char, uint16_t>> labelScopeStack;
        std::stack<size_t> returnStack;
    };
    State _state;
    void _printState(std::list<std::string*> &) const;

    const std::map<char, void (Brain::*)()> _instructionsMap;
    void _next();
    void _previous();
    void _increment();
    void _decrement();
    void _read();
    void _write();
    void _loopStart();
    void _loopEnd();
    void _conditionStart();
    void _conditionEnd();
    void _declarationStart();
    void _declarationEnd();
    void _call();
    void _return();
    void _definition();
    void _go();
    void _address();
    void _load();
    void _move();

public:
    Brain(Memory &memory, Program &program);
    void execute();
    class NotExecutable : public std::exception {
    public:
        NotExecutable() {};
        const char *what() const noexcept;
    };
};
