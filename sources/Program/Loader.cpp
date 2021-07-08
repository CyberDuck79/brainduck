/*      __     Loader.cpp
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
#include "../../includes/print.hpp"

bool Program::Loader::_getFileSize(uint16_t &size) {
    _file.seekg(0, _file.end);
    std::ifstream::pos_type fileSize = _file.tellg();
    _file.seekg(0, _file.beg);
    if (fileSize > DISK_SIZE)
        return (false);
    size = static_cast<uint16_t>(fileSize);
    return (true);
}

void Program::Loader::_loadProgram(const uint16_t size, char (&disk)[DISK_SIZE]) {
    std::cout << HIDE_CURSOR;
    for (uint16_t count = 0; count < size; count++) {
        _file.read(&disk[count], 1);
        printf("Disk writing: [%i/%hu]\n", count + 1, size);
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        std::cout << RESET_LINE;
    }
    std::cout << DISPLAY_CURSOR;
}

Program::Loader::OpenError::OpenError(const char *filename) {
    _msg = "Impossible to open \"";
    _msg += filename;
    _msg += "\" file does not exist.";
}

const char *Program::Loader::OpenError::what() const noexcept {
    return (_msg.c_str());
}

Program::Loader::SizeOverflow::SizeOverflow(const char *filename) {
    _msg = "Impossible to open \"";
    _msg += filename;
    _msg += "\" program too large to fit into the Machine Virtual Disk.";
}

const char *Program::Loader::SizeOverflow::what() const noexcept {
    return (_msg.c_str());
}

Program::Loader::Loader(const char *filename, uint16_t &size, char (&disk)[DISK_SIZE]):
_file(filename) {
    if (!_file.is_open())
        throw OpenError(filename);
    if (!_getFileSize(size)) {
        _file.close();
        throw SizeOverflow(filename);
    }
    _loadProgram(size,disk);
    _file.close();
}
