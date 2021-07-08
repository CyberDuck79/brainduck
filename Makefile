##      __     Makefile
##  ___( o)>   Creation Date: 11/12/2020
##  \ <_. )    Copyright (C) 2020 Flavien Henrion
##   `---'     Made with care and a lot of quack quack.
##
##  This program is free software; you can redistribute it and/or
##  modify it under the terms of the GNU General Public License
##  as published by the Free Software Foundation; either version 2
##  of the License, or (at your option) any later version.

NAME = Brainduck
SRC_NAME = main BrainMachine Memory Program Program/Loader Program/Checker Program/Compiler Brain
HDR_NAME = BrainMachine Memory Program Brain print

SRC = $(addsuffix .cpp,$(SRC_NAME))
SRC_FOLDER = sources/
OBJ = $(addsuffix .o,$(SRC_NAME))
OBJ_FOLDER = objects/
HDR = $(addsuffix .hpp,$(HDR_NAME))
HDR_FOLDER = includes/

CC = clang++
CFLAGS = -Werror -Wall -Wextra -std=c++11 -g

.PHONY: clean fclean all re bonus

all: $(NAME)

$(NAME): $(OBJ_FOLDER) $(addprefix $(OBJ_FOLDER), $(OBJ)) $(addprefix $(HDR_FOLDER), $(HDR))
	$(CC) $(CFLAGS) $(addprefix $(OBJ_FOLDER), $(OBJ)) -I $(HDR_FOLDER) -o $@
	@echo "\033[0;32mCompilation OK\033[0m"

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.cpp $(HDR_FOLDER)%.hpp
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_FOLDER)%.o : $(SRC_FOLDER)%.cpp
	$(CC) -c $(CFLAGS) $< -o $@

# Link Program class subclasses to Program header
$(OBJ_FOLDER)program/%.o : $(SRC_FOLDER)program/%.cpp $(HDR_FOLDER)program.hpp
	$(CC) -c $(CFLAGS) $< -o $@

$(OBJ_FOLDER):
	mkdir $(OBJ_FOLDER)
	mkdir $(OBJ_FOLDER)program

commit:
	git add $(SRC_FOLDER) $(HDR_FOLDER) Makefile
	git commit -m 'makefile_commit'
	git push

clean:
	/bin/rm -rf $(OBJ_FOLDER)
	@echo "\033[0;32mObjects files cleaning OK\033[0m"

fclean: clean
	/bin/rm -f $(NAME)
	@echo "\033[0;32mLibrary cleaning OK\033[0m"

re: fclean $(NAME)