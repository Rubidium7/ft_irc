
#colours
GREEN = \033[0;32m
RED = \033[0;31m
BLUE = \033[0;34m
MAGENTA = \033[0;35m
END = \033[0m

#binary name
NAME = ircserv

#compiler
C = c++

#compilation flags
FLAGS = -std=c++98 -Wall -Wextra -Werror -I includes -g

OBJS_DIR = obj/
SRCS_DIR = srcs/
SRCS = main.cpp errors.cpp setup.cpp Server.cpp Client.cpp Channel.cpp Parser.cpp Join.cpp Nick.cpp Invite.cpp User.cpp Pass.cpp Part.cpp ToolFunctions.cpp Mode.cpp Topic.cpp Kick.cpp Privmsg.cpp WhoIs.cpp

OBJ_FILES = $(SRCS:.cpp=.o)

OBJS = $(patsubst %, $(OBJS_DIR)%, $(SRCS:.cpp=.o))

all: $(NAME)

$(NAME): $(OBJS_DIR) $(OBJS)
	@$(C) $(FLAGS) $(OBJS) -o $@
	@echo "$(GREEN)$@ created$(END)"

$(OBJS_DIR):
	@mkdir -p $(OBJS_DIR)
	@echo "$(BLUE)object directory created$(END)"

$(OBJS_DIR)%.o: $(SRCS_DIR)%.cpp
	@$(C) $(FLAGS) -c $< -o $@
	@echo "$(BLUE)$@ created$(END)"

clean:
	@rm -f $(OBJS)
	@echo "$(MAGENTA)objs cleaned$(END)"
	@rm -rf $(OBJS_DIR)
	@echo "$(MAGENTA)obj directory removed$(END)"

fclean: clean
	@rm -f $(NAME)
	@echo "$(RED)$(NAME) removed$(END)"

re: fclean all
