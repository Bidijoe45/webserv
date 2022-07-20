NAME		:= webserv

#OBJS_DIR	:= ./objs

SRCS		:= $(shell find ./srcs -name "*.cpp" ! -name "main.cpp")
SRCS_TEST	:= $(shell find ./tester -name "*.cpp" ! -name "main.cpp")

SERVER_MAIN := srcs/main.cpp
TESTER_MAIN := tester/main.cpp

#OBJS	:= $(shell echo $(SRCS:.cpp=.o) | rev | cut -d "/" -f 1 | rev)
OBJS	:= $(SRCS:.cpp=.o)
OBJS_TEST := $(SRCS_TEST:.cpp=.o)

RM			= rm -rf

CXX			= clang++

COMMON		=
CXXFLAGS	?= -std=c++98 $(COMMON)
LDFLAGS		?= $(COMMON)
SANITIZE	= -g3 -fsanitize=address

$(NAME): $(OBJS) $(SERVER_MAIN)
	$(CXX) $(LDFLAGS) $^ -o $@

test: $(OBJS) $(OBJS_TEST) $(TESTER_MAIN)
	$(CXX) $(LDFLAGS) $^ -o $@

debug:	COMMON += $(SANITIZE)
debug:	re

tdebug: COMMON += $(SANITIZE)
tdebug: fclean test

all: $(NAME)

clean:
	$(RM) $(OBJS) $(TEST_OBJS)

fclean: clean
	$(RM) $(NAME)
	$(RM) test

re: fclean all

.PHONY: all clean fclean re debug tdebug