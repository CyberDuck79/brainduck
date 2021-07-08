/*      __     print.hpp
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
#include <iostream>
#include <chrono>
#include <thread>

#define HIDE_CURSOR "\e[?25l"
#define DISPLAY_CURSOR "\e[?25h"
#define RESET_SCREEN "\e[H"
#define CLEAR_SCREEN "\e[2J\e[H"
#define RESET_LINE "\e[A\r"
#define CLEAR_LINE "\e[2K\r"
