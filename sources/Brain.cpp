/*      __     Brain.cpp
**  ___( o)>   Creation Date: 21/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/

#include "../includes/Brain.hpp"
#include <iostream>
#include "../includes/print.hpp"

void Brain::_printState(std::list<std::string*> &history) const {
    char buffer[75];

    sprintf(buffer, "Program Counter: 0x%04X |", _state.counter);
    sprintf(&buffer[25], " Pointer: 0x%04X |", _state.pointer);
    sprintf(&buffer[43], " Value: 0x%04X |", _memory.read(_state.pointer));
    sprintf(&buffer[59], " Instruction: %c", _program.read(_state.counter));
    history.push_back(new std::string(buffer));
    std::cout << RESET_SCREEN;
    for (auto & it : history) {
        std::cout << *it << std::endl;
    }
    std::cout << _state.output << std::endl;
    if (history.size() > 7) {
        delete *history.begin();
        history.pop_front();
    }
}

void Brain::_next() {
    _state.pointer++;
    _state.counter++;
}

void Brain::_previous() {
    _state.pointer--;
    _state.counter++;
}

void Brain::_increment() {
    const char data = _memory.read(_state.pointer);
    _memory.write(_state.pointer, data + 1);
    _state.counter++;
}

void Brain::_decrement() {
    const char data = _memory.read(_state.pointer);
    _memory.write(_state.pointer, data - 1);
    _state.counter++;
}

void Brain::_read() {
    _state.output += static_cast<char>(_memory.read(_state.pointer));
    _state.counter++;
}

void Brain::_write() {
    char input;
    std::cin >> input;
    _memory.write(_state.pointer, input);
    _state.counter++;
}

void Brain::_loopStart() {
    if (!_memory.read(_state.pointer)) {
        while (_program.read(_state.counter) != ']')
            _state.counter++;
    }
    _state.counter++;
}

void Brain::_loopEnd() {
    if (_memory.read(_state.pointer)) {
        while (_program.read(_state.counter) != '[')
            _state.counter--;
    }
    _state.counter++;
}

void Brain::_conditionStart() {
    if (!_memory.read(_state.pointer)) {
        while (_program.read(_state.counter) != '}')
            _state.counter++;
    }
    _state.counter++;
}

void Brain::_conditionEnd() {
    _state.counter++;
}

void Brain::_declarationStart() {
    while (_program.read(_state.counter) != ';')
        _state.counter++;
    _state.counter++;
}

void Brain::_declarationEnd() {
    _state.counter = _state.returnStack.top();
    _state.returnStack.pop();
}

void Brain::_call() {
    const char symbol = _program.read(_state.counter + 1);
    _state.returnStack.push(_state.counter + 2);
    _state.counter = _program.procedureCounter(symbol);
    _state.labelScopeStack.push(_state.labelMap);
    _state.labelMap = _program.labelMap(symbol);
}

void Brain::_return() {
    if (_state.returnStack.empty()) {
        _state.counter = _program.getSize();
    } else {
        _state.counter = _state.returnStack.top();
        _state.returnStack.pop();
        _state.labelMap = _state.labelScopeStack.top();
        _state.labelScopeStack.pop();
    }
}

void Brain::_definition() {
    _state.counter++;
    _state.labelMap.at(_program.read(_state.counter)) = _state.pointer;
    _state.counter++;
}

void Brain::_go() {
    _state.counter++;
    const char symbol = _program.read(_state.counter);
    if (symbol == '*') {
        _state.pointer = _memory.read(_state.pointer);
    } else {
        _state.pointer = _state.labelMap.at(symbol);
    }
    _state.counter++;
}

void Brain::_address() {
    _state.counter++;
    const char symbol = _program.read(_state.counter);
    if (symbol == '*') {
        _memory.write(_state.pointer, _state.pointer);
    } else {
        _memory.write(_state.pointer, _state.labelMap.at(symbol));
    }
    _state.counter++;
}

void Brain::_load() {
    _state.counter++;
    const char symbol = _program.read(_state.counter);
    const uint16_t value = _memory.read(_state.labelMap.at(symbol));
    _memory.write(_state.pointer, value);
    _state.counter++;
}

void Brain::_move() {
    _state.counter++;
    const char symbol = _program.read(_state.counter);
    const uint16_t value = _memory.read(_state.pointer);
    _memory.write(_state.labelMap.at(symbol), value);
    _state.counter++;
}

void Brain::execute() {
    if (!_program.executable()) {
        throw NotExecutable();
    }
    const size_t size = _program.getSize();
    std::list<std::string*> history;

    std::cout << CLEAR_SCREEN << HIDE_CURSOR;
    while (_state.counter < size) {
        _printState(history);
        std::this_thread::sleep_for(std::chrono::microseconds(500)); // 2Mhz
        (this->*_instructionsMap.at(_program.read(_state.counter)))();
    }
    std::cout << DISPLAY_CURSOR;
    _memory.clear();
    for (auto &it : history) {
        delete it;
    }
    _state = State(_program.labelMap('0'));
}

const char *Brain::NotExecutable::what() const noexcept {
    return ("Program not executable.");
}

Brain::Brain(Memory &memory, Program &program):
_memory(memory),
_program(program),
_state(program.labelMap('0')),
_instructionsMap({
     {'>', &Brain::_next},
     {'<', &Brain::_previous},
     {'+', &Brain::_increment},
     {'-', &Brain::_decrement},
     {'.', &Brain::_read},
     {',', &Brain::_write},
     {'[', &Brain::_loopStart},
     {']', &Brain::_loopEnd},
     {'{', &Brain::_conditionStart},
     {'}', &Brain::_conditionEnd},
     {'?', &Brain::_declarationStart},
     {';', &Brain::_declarationEnd},
     {'!', &Brain::_call},
     {':', &Brain::_return},
     {'%', &Brain::_definition},
     {'@', &Brain::_go},
     {'&', &Brain::_address},
     {'=', &Brain::_load},
     {'$', &Brain::_move}
}) {}
