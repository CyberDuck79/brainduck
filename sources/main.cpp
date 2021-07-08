/*      __     main.cpp
**  ___( o)>   Creation Date: 11/12/2020
**  \ <_. )    Copyright (C) 2020 Flavien Henrion
**   `---'     Made with care and a lot of quack quack.
**
**  This program is free software; you can redistribute it and/or
**  modify it under the terms of the GNU General Public License
**  as published by the Free Software Foundation; either version 2
**  of the License, or (at your option) any later version.
*/

#include "../includes/BrainMachine.hpp"
#include "../includes/print.hpp"
#include <exception>
#include <iostream>

int main(int argc, const char** argv) {
	if (argc != 2) {
		std::cerr << "USAGE: ./Brainfuck program_filename.duck" << std::endl;
		return (1);
	}
	int error = 0;
    BrainMachine machine;
    error = machine.load(argv[1]);
    if (!error)
        error = machine.execute();
	return (error);
}
