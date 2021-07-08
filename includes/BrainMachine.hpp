/*      __     BrainMachine.hpp
**  ___( o)>   Creation Date: 11/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/

#include <exception>
#include <string>
#include <sstream>
#include <map>
#include <stack>
#include <list>
#include <cinttypes>
#include "Memory.hpp"
#include "Program.hpp"
#include "Brain.hpp"

class BrainMachine {
private:
    Memory _memory;
    Program _program;
    Brain _brain;

public:
	BrainMachine();
	int load(const char *filename);
	int execute();
};