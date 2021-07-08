/*      __     Program.hpp
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
#include <fstream>
#include <iostream>
#include <map>

#define DISK_SIZE 65536

class Program {
private:
    char _program[DISK_SIZE];
    uint16_t _size;
    bool _checked;
    bool _compiled;

    class Loader {
    private:
        std::ifstream _file;

        bool _getFileSize(uint16_t &size);
        void _loadProgram(const uint16_t size, char (&disk)[DISK_SIZE]);

        class OpenError : public std::exception {
        private:
            std::string	_msg;
        public:
            OpenError(const char *filename);
            const char *what() const noexcept;
        };
        class SizeOverflow : public std::exception {
        private:
            std::string	_msg;
        public:
            SizeOverflow(const char *filename);
            const char *what() const noexcept;
        };

    public:
        Loader(const char *filename, uint16_t &size, char (&disk)[DISK_SIZE]);
    };

    class Checker {
    private:
        const char (&_program)[DISK_SIZE];
        const uint16_t _size;

        void _checkSymbols() const;
        class InvalidInstruction : public std::exception {
        private:
            std::string	_msg;
        public:
            InvalidInstruction(const char symbol, const size_t index);
            const char *what() const noexcept;
        };
        class InvalidMemoryLabeling : public std::exception {
        private:
            std::string	_msg;
        public:
            InvalidMemoryLabeling(const char label, const size_t index);
            const char *what() const noexcept;
        };
        class InvalidProcedureLabeling : public std::exception {
        private:
            std::string	_msg;
        public:
            InvalidProcedureLabeling(const char label, const size_t index);
            const char *what() const noexcept;
        };

        void _checkEnclosure() const;
        class ClosureUnopened : public std::exception {
        private:
            std::string	_msg;
        public:
            ClosureUnopened(const char closure, const size_t index);
            const char *what() const noexcept;
        };
        class DontMatchClosure : public std::exception {
        private:
            std::string	_msg;
        public:
            DontMatchClosure(const char closure, const char opener, const size_t i);
            const char *what() const noexcept;
        };
        class OpenerUnclosed : public std::exception {
        private:
            std::string	_msg;
        public:
            OpenerUnclosed(const char opener, const size_t index);
            const char *what() const noexcept;
        };

        void _checkProcedures() const;
        class NestedProcedure : public std::exception {
        private:
            std::string	_msg;
        public:
            NestedProcedure(const size_t index);
            const char *what() const noexcept;
        };

    public:
        Checker(uint16_t size, char (&program)[DISK_SIZE]);
    };

    struct procedureData {
        procedureData(uint16_t pointer): procedureCounter(pointer) {}
        uint16_t procedureCounter;
        std::map<char, uint16_t> labelMap;
    };
    std::map<char, procedureData*> _procedureMap;

    class Compiler {
    private:
        const char (&_program)[DISK_SIZE];
        const uint16_t _size;
        std::map<char, procedureData*> &_procedureMap;

        void _procedureCompilation();
        class UnknownProcedureSymbol : public std::exception {
        private:
            std::string	_msg;
        public:
            UnknownProcedureSymbol(const char symbol, const size_t index);
            const char *what() const noexcept;
        };
        void _memoryLabelPreparation();
        class UnknownLabelSymbol : public std::exception {
        private:
            std::string	_msg;
        public:
            UnknownLabelSymbol(const char symbol, const size_t index);
            const char *what() const noexcept;
        };

    public:
        Compiler(uint16_t size, char (&program)[DISK_SIZE], \
        std::map<char, procedureData*> &compilationMap);
    };

public:
    Program();
    ~Program();

    void open(const char *filename);
    void check();
    class NoProgram : public std::exception {
    public:
        NoProgram() {};
        const char *what() const noexcept;
    };
    void compile();
    class NotChecked : public std::exception {
    public:
        NotChecked() {};
        const char *what() const noexcept;
    };
    bool executable() const;

    const uint16_t &getSize() const;
    const char &read(const uint16_t index) const;
    class ProgramOverflow : public std::exception {
    private:
        std::string	_msg;
    public:
        ProgramOverflow(const size_t index, const size_t size);
        const char *what() const noexcept;
    };

    uint16_t procedureCounter(char symbol) const;
    std::map<char, uint16_t> labelMap(char symbol) const;

};