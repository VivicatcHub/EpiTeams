##
## EPITECH PROJECT, 2026
## MyTeams
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
RM	=	rm
NAME	=	my_teams
SRCS	=	$(shell find src -type f -name "*.c")
OBJDIR	=	build
OBJS	=	$(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

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
CFLAGS	=	-I./include/
CFLAGS	+=	-Wall
CFLAGS	+=	-Wextra
CFLAGS	+=	-Wpedantic
CFLAGS	+=	-Wunused
CFLAGS	+=	-Wunused-parameter

## for doing debug execute "make ENV=dev"
ifeq ($(ENV), dev)
	CFLAGS	+=	-g3 -DENV_DEV
else
	CFLAGS	+=	-O2 -fno-builtin
endif

TESTS_PATH	=	tests/

all:	$(NAME)

$(NAME):	$(OBJS)
	@$(CC) $(OBJS) -o $(NAME) $(CFLAGS) \
	&& $(call PRINT,COMPILED,$(BOLD_)$(NAME)) \
	|| $(call ERROR,BUILD FAILED,$@)

# Build object files into the build/ directory, creating dirs as needed
$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) -c $< -o $@ $(CFLAGS) \
	&& $(call PRINT,COMPILED,$(BOLD_)$@) \
	|| $(call ERROR,BUILD FAILED,$@)

clean:
	@$(RM) -f $(OBJS) \
	&& $(RM) -rf $(OBJDIR) \
	&& $(call REMOVE,REMOVE,.o files) \
	&& $(call REMOVE,REMOVE,.hi files)

fclean: clean
	@$(RM) -f $(NAME) \
	&& $(call REMOVE,REMOVE,$(NAME))

re:	fclean all

.PHONY: all clean fclean re
