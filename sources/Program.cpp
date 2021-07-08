/*      __     Program.cpp
**  ___( o)>   Creation Date: 21/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/

#include "../includes/Program.hpp"
#include "../includes/print.hpp"

Program::Program(): _program(), _size(0), _checked(), _compiled(0) {
    _procedureMap['0'] = new procedureData(0);
}

Program::~Program() {
    for (auto &it : _procedureMap) {
        delete it.second;
    }
}

void Program::open(const char *filename) {
    std::cout << "Program loading..." << std::endl;
    Loader(filename, _size, _program);
    _checked = false;
    _compiled = false;
    std::cout << std::endl << "Program loading done." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Program::check() {
    if (!_size) {
        throw NoProgram();
    } else if (_checked) {
        return;
    }
    std::cout << "Program checking..." << std::endl;
    Checker(_size, _program);
    _checked = true;
    std::cout << "Program checking passed." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

void Program::compile() {
    if (!_size) {
        throw NoProgram();
    } else if (!_checked){
        throw NotChecked();
    } else if (_compiled) {
        return;
    }
    std::cout << "Program compilation..." << std::endl;
    Compiler(_size, _program, _procedureMap);
    _compiled = true;
    std::cout << "Program compilation succeeded." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

const char *Program::NoProgram::what() const noexcept {
    return ("No program.");
}
const char *Program::NotChecked::what() const noexcept {
    return ("Program not checked.");
}

bool Program::executable() const {
    return (_size && _checked && _compiled);
}

const uint16_t &Program::getSize() const {
    return (_size);
}

const char &Program::read(const uint16_t index) const {
    if (index >= _size)
        throw ProgramOverflow(index, _size);
    return (_program[index]);
}

Program::ProgramOverflow::ProgramOverflow(const size_t index, const size_t size) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Attempt to read program at index " << index;
    _msgConstructor << " while size is " << size;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Program::ProgramOverflow::what() const noexcept {
    return (_msg.c_str());
}

uint16_t  Program::procedureCounter(char symbol) const {
    return _procedureMap.at(symbol)->procedureCounter;
}

std::map<char, uint16_t>  Program::labelMap(char symbol) const {
    return _procedureMap.at(symbol)->labelMap;
}
