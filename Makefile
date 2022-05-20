CXX = clang++

SRCS = $(shell find ./srcs -name "*.cpp")
OBJS = $(SRCS:.cpp=.o)

TEST_SRCS = $(shell find ./tester -name "*.cpp")
TEST_OBJS = $(TEST_SRCS:.cpp=.o)

server: $(OBJS)
	$(CXX) $(SRCS) -o $@ -fsanitize=address

test: $(OBJS) $(TEST_OBJS)
	$(CXX) $(OBJS) $(TEST_OBJS) -o $@ -fsanitize=address

all: server

clean:
	rm -rf $(OBJS)

fclean: clean

re: fclean all

tclean: fclean
	rm -rf $(TEST_OBJS)

tfclean: tclean
	rm -f test

tre: tfclean test 