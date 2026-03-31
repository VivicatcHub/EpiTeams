##
## EPITECH PROJECT, 2026
## my_teams
## File description:
## Makefile
##

## STYLE ##
ECHO	:=	/bin/echo
DEF_	:=	\e[0m
BOLD_	:=	\e[1m
RED_	:=	\e[31m
GREEN_	:=	\e[32m
CYAN_	:=	\e[36m

## KEYWORDS ##
CC	=	epiclang
CXX	=	g++
RM	=	rm
NAME_SERVER	=	myteams_server
NAME_CLI	=	myteams_cli
SRCS_SERVER	=	$(shell find src/server -type f -name "*.c")
SRCS_CLI	=	$(shell find src/cli -type f -name "*.cpp")
OBJDIR_SERVER	=	build/server
OBJDIR_CLI	=	build/cli
OBJS_SERVER	=	$(patsubst src/server/%.c,$(OBJDIR_SERVER)/%.o,$(SRCS_SERVER))
OBJS_CLI	=	$(patsubst src/cli/%.cpp,$(OBJDIR_CLI)/%.o,$(SRCS_CLI))

## MESSAGES ##
define PRINT
    $(ECHO) -e "[$(GREEN_)✔$(DEF_)] $(1): $(GREEN_)$(2)$(DEF_)"
endef
define ERROR
    $(ECHO) -e "[$(RED_)✘$(DEF_)]$(RED_) $(1) ($(2))$(DEF_)"
endef
define REMOVE
    $(ECHO) -e "[$(CYAN_)✔$(DEF_)] $(1): $(CYAN_)$(2)$(DEF_)"
endef

## FLAGS ##
CFLAGS	=	-Wall
CFLAGS	+=	-Wextra
CFLAGS	+=	-Wpedantic
CFLAGS	+=	-Wunused
CFLAGS	+=	-Wunused-parameter

## for doing debug execute "make ENV=dev"
## NOTE: dev environment will compile using gcc for faster compilation
ifeq ($(ENV), dev)
	CC = gcc
	CFLAGS	+=	-g3 -DENV_DEV
else
	CFLAGS	+=	-O2 -fno-builtin
endif

TESTS_PATH	=	tests/

all:	$(NAME_SERVER) $(NAME_CLI)

$(NAME_SERVER):	$(OBJS_SERVER)
	@$(CC) -I./include/server/ -I./libs/myteams/ $(CFLAGS) $(OBJS_SERVER) -o \
	$(NAME_SERVER) \
	&& $(call PRINT,COMPILED,$(BOLD_)$(NAME_SERVER)) \
	|| $(call ERROR,BUILD FAILED,$@)

$(NAME_CLI):	$(OBJS_CLI)
	@$(CXX) -I./include/cli/ $(CFLAGS) $(OBJS_CLI) -o $(NAME_CLI) -lstdc++ \
	&& $(call PRINT,COMPILED,$(BOLD_)$(NAME_CLI)) \
	|| $(call ERROR,BUILD FAILED,$@)

# Build object files into the build/ directory, creating dirs as needed
$(OBJDIR_SERVER)/%.o: src/server/%.c
	@mkdir -p $(dir $@)

	@$(CC) -c $< -o $@ -I./include/server/ -I./libs/myteams/ $(CFLAGS) \
	&& $(call PRINT,COMPILED,$(BOLD_)$@) \
	|| $(call ERROR,BUILD FAILED,$@)

$(OBJDIR_CLI)/%.o: src/cli/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) -c $< -o $@ -I./include/cli/ -I./libs/myteams/ $(CFLAGS) \
	&& $(call PRINT,COMPILED,$(BOLD_)$@) \
	|| $(call ERROR,BUILD FAILED,$@)

clean:
	@$(RM) -f $(OBJS_SERVER) $(OBJS_CLI) \
	&& $(RM) -rf $(OBJDIR_SERVER) $(OBJDIR_CLI) \
	&& $(call REMOVE,REMOVE,.o files) \
	&& $(call REMOVE,REMOVE,.hi files)

fclean: clean
	@$(RM) -f $(NAME_SERVER) $(NAME_CLI) \
	&& $(call REMOVE,REMOVE,$(NAME_SERVER)) \
	&& $(call REMOVE,REMOVE,$(NAME_CLI))

re:	fclean all

mango:
	mango -Ef libs/*

.PHONY: all clean fclean re
