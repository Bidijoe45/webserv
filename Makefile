PROJDIR		:= $(realpath $(CURDIR))
BUILDDIR	:= $(PROJDIR)/build
VERBOSE		:= FALSE

NAME		:= webserv
SOURCEDIR	:= $(PROJDIR)/srcs
VPATH		:= $(SOURCEDIR)
SOURCEDIRS	:= $(shell find $(SOURCEDIR) -type d)
TARGETDIRS	:= $(subst $(SOURCEDIR),$(BUILDDIR),$(SOURCEDIRS))
SRCS		:= $(foreach dir,$(SOURCEDIRS),$(wildcard $(dir)/*.cpp))
OBJS		:= $(subst $(SOURCEDIR),$(BUILDDIR),$(SRCS:.cpp=.o))

INCLUDES	:= $(foreach dir,$(SOURCEDIRS),$(addprefix -I,$(dir)))

DEPS		= $(OBJS:.o=.d)

CXX			= clang++

RM			= rm -rf
MKDIR		= mkdir -p
ERRIGNORE	= 2>/dev/null

COMMON		=
CXXFLAGS	?= $(INCLUDES) -std=c++98 $(COMMON)
LDFLAGS		?= $(COMMON)
SANITIZE	= -g3 -fsanitize=address

ifeq ($(VERBOSE),TRUE)
HIDE =  
else
HIDE = @
endif

all: directories $(NAME)

debug: COMMON += $(SANITIZE)
debug: re

$(NAME): $(OBJS)
	@echo Linking $@
	$(HIDE)$(CXX) $(LDFLAGS) $^ -o $@

ifeq (,$(filter clean fclean,$(MAKECMDGOALS)))
-include $(DEPS)
endif

define generateRules
$(1)/%.o: %.cpp
	@echo Building $$@
	$(HIDE)$(CXX) -c $$(CXXFLAGS) -o $$@ $$< -MMD
endef

$(foreach targetdir, $(TARGETDIRS), $(eval $(call generateRules, $(targetdir))))

directories:
	$(HIDE)$(MKDIR) $(TARGETDIRS) $(ERRIGNORE)

clean:
	$(HIDE)$(RM) $(BUILDDIR) $(ERRIGNORE)
	@echo cleaning done!

fclean: clean
	$(HIDE)$(RM) $(NAME) $(ERRIGNORE)
	@echo fcleaning done!

re: fclean all

.PHONY: all clean fclean re directories

