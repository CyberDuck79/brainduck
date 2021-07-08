/*      __     Compiler.cpp
**  ___( o)>   Creation Date: 21/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/

#include "../../includes/Program.hpp"

void Program::Compiler::_procedureCompilation() {
    _procedureMap.at('0')->labelMap.clear();
    for (size_t	i = 0; i < _size; i++) {
        if (_program[i] == '?') {
            i++;
            _procedureMap[_program[i]] = new procedureData(i + 1);
        } else if (_program[i] == '!') {
            i++;
            if (_procedureMap.find(_program[i]) == _procedureMap.end())
                throw UnknownProcedureSymbol(_program[i], i);
        }
    }
}

Program::Compiler::UnknownProcedureSymbol
::UnknownProcedureSymbol(const char symbol, const size_t index) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Unknown procedure call symbol: " << symbol;
    _msgConstructor << " at index " << index;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Program::Compiler::UnknownProcedureSymbol::what() const noexcept {
    return (_msg.c_str());
}

void Program::Compiler::_memoryLabelPreparation() {
    const char *labelInstructions = "@&=$";
    std::stack<std::map<char, uint16_t>*> labelMapStack;

    labelMapStack.push(&_procedureMap['0']->labelMap);
    for (size_t	i = 0; i < _size; i++) {
        if (_program[i] == '?') {
            i++;
            labelMapStack.push(&_procedureMap[_program[i]]->labelMap);
        } else if (_program[i] == ';') {
            labelMapStack.pop();
        }
        else if (_program[i] == '%') {
            i++;
            labelMapStack.top()->at(_program[i]) = 0;
        } else if (strchr(labelInstructions, _program[i])) {
            i++;
            if (labelMapStack.top()->find(_program[i]) == labelMapStack.top()->end())
                throw UnknownLabelSymbol(_program[i], i);
        }
    }
}

Program::Compiler::UnknownLabelSymbol
::UnknownLabelSymbol(const char symbol, const size_t index) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Unknown label symbol: " << symbol;
    _msgConstructor << " at index " << index;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Program::Compiler::UnknownLabelSymbol::what() const noexcept {
    return (_msg.c_str());
}

Program::Compiler::Compiler(uint16_t size, char (&program)[DISK_SIZE], \
std::map<char, procedureData*> &compilationMap):
_program(program),
_size(size),
_procedureMap(compilationMap){
    _procedureCompilation();
    _memoryLabelPreparation();
}
