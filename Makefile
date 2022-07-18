CXX = clang++

SRCS = $(shell find ./srcs -name "*.cpp" ! -name "main.cpp")
OBJS = $(SRCS:.cpp=.o)

SERVER_MAIN = srcs/main.cpp

TEST_SRCS = $(shell find ./tester -name "*.cpp" ! -name "main.cpp")
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

TESTER_MAIN = tester/main.cpp

webserv: $(OBJS) $(SERVER_MAIN)
	$(CXX) $(SRCS) ./srcs/main.cpp -o $@ -I ./srcs -fsanitize=address 

test: $(OBJS) $(TEST_OBJS) $(TESTER_MAIN)
	$(CXX) $(OBJS) $(TEST_OBJS) ./tester/main.cpp -o $@ -I ./srcs -fsanitize=address

all: webserv

clean:
	rm -rf $(OBJS)

fclean: clean

re: fclean all

tclean: fclean
	rm -rf $(TEST_OBJS)

tfclean: tclean
	rm -f test

tre: tfclean test 