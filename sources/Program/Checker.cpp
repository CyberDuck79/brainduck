/*      __     Checker.cpp
**  ___( o)>   Creation Date: 26/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/

#include "../../includes/Program.hpp"

void Program::Checker::_checkSymbols() const {
    const char instructionSymbols[] = "><+-.,[]{}?;!:%@&=$";
    const char memorySymbols[] = "%@&=$";
    const char procedureSymbols[] = "?!";
    const char memoryLabels[] = "abcdefghijklmnopqrstuvwxyz";
    //const char registersLabels[] = "0123456789";
    const char procedureLabels[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    for (size_t	i = 0; i < _size; i++) {
        if (!strchr(instructionSymbols, _program[i]))
            throw InvalidInstruction(_program[i], i);
        if (strchr(memorySymbols, _program[i])) {
            if (!strchr(memoryLabels, _program[i + 1]))
                throw InvalidMemoryLabeling(_program[i + 1], i);
            i++;
        } else if (strchr(procedureSymbols, _program[i])) {
            if (!strchr(procedureLabels, _program[i + 1]))
                throw InvalidProcedureLabeling(_program[i + 1], i);
            i++;
        }
    }
}

Program::Checker::InvalidInstruction
::InvalidInstruction(const char symbol, const size_t index) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Invalid instruction symbol " << symbol;
    _msgConstructor << " at index " << index;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Program::Checker::InvalidInstruction::what() const noexcept {
    return (_msg.c_str());
}

Program::Checker::InvalidMemoryLabeling
::InvalidMemoryLabeling(const char label, const size_t index) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Invalid memory label " << label;
    _msgConstructor << " at index " << index;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Program::Checker::InvalidMemoryLabeling::what() const noexcept {
    return (_msg.c_str());
}

Program::Checker::InvalidProcedureLabeling
::InvalidProcedureLabeling(const char label, const size_t index) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Invalid procedure label " << label;
    _msgConstructor << " at index " << index;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Program::Checker::InvalidProcedureLabeling::what() const noexcept {
    return (_msg.c_str());
}

void Program::Checker::_checkEnclosure() const {
    const char openSymbols[] = "{[?";
    const char closeSymbols[] = "}];";
    const char *ptr;
    std::stack<long> IndexStack;

    for (size_t	i = 0; i < _size; i++) {
        if ((ptr = strchr(openSymbols, _program[i]))) {
            IndexStack.push(ptr - openSymbols);
        } else if ((ptr = strchr(closeSymbols, _program[i]))) {
            if (IndexStack.empty())
                throw ClosureUnopened(_program[i], i);
            if (IndexStack.top() != ptr - closeSymbols)
                throw DontMatchClosure(_program[i], openSymbols[IndexStack.top()], i);
            IndexStack.pop();
        }
    }
    if (!IndexStack.empty())
        throw OpenerUnclosed(openSymbols[IndexStack.top()], _size);
}

Program::Checker::ClosureUnopened
::ClosureUnopened(const char closure, const size_t index) {
    std::stringstream _msgConstructor;
    _msgConstructor << closure;
    _msgConstructor << " at index " << index;
    _msgConstructor << " is unopened.";
    _msg = _msgConstructor.str();
}

const char *Program::Checker::ClosureUnopened::what() const noexcept {
    return (_msg.c_str());
}

Program::Checker::DontMatchClosure
::DontMatchClosure(const char closure, const char opener, const size_t i) {
    std::stringstream _msgConstructor;
    _msgConstructor << closure;
    _msgConstructor << " at index " << i;
    _msgConstructor << " don't match " << opener;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Program::Checker::DontMatchClosure::what() const noexcept {
    return (_msg.c_str());
}

Program::Checker::OpenerUnclosed
::OpenerUnclosed(const char opener, const size_t index) {
    std::stringstream _msgConstructor;
    _msgConstructor << opener;
    _msgConstructor << " at index " << index;
    _msgConstructor << " is unclosed.";
    _msg = _msgConstructor.str();
}

const char *Program::Checker::OpenerUnclosed::what() const noexcept {
    return (_msg.c_str());
}

void Program::Checker::_checkProcedures() const {
    bool procedure = false;

    for (size_t i = 0; i < _size; i++) {
        if (_program[i] == '?') {
            if (procedure)
                throw NestedProcedure(i);
            procedure = true;
        } else if (_program[i] == ';') {
            procedure = false;
        }
    }
}

Program::Checker::NestedProcedure::NestedProcedure(const size_t index) {
    std::stringstream _msgConstructor;
    _msgConstructor << "Nested procedure declaration at index " << index;
    _msgConstructor << ".";
    _msg = _msgConstructor.str();
}

const char *Program::Checker::NestedProcedure::what() const noexcept {
    return (_msg.c_str());
}

Program::Checker::Checker(uint16_t size, char (&program)[DISK_SIZE]):
_program(program),
_size(size) {
    _checkSymbols();
    _checkEnclosure();
    _checkProcedures();
}
