# Project settings
NAME = FlappyLeo
CXX = g++
DEBUG = -g
OPT = -O8
WARN = -Wall -Wextra -Werror
SFML = -lsfml-graphics -lsfml-window -lsfml-system
CXXFLAGS = $(DEBUG) $(OPT) $(WARN)

# Paths
SRCS_DIR = src
SRCS = $(SRCS_DIR)/main.cpp $(SRCS_DIR)/flappy.cpp
OBJS = $(SRCS:.cpp=.o)

# Default target
all: $(NAME)

# Linking the final executable
$(NAME): $(OBJS)
	$(CXX) $(OBJS) $(SFML) -o $(NAME)
	@echo "\033[1;32m[ ✔ ] $(NAME) ready!\033[0m"
	@echo "\033[0;31m𓄿 𓅓  Made by Alabar 𓄿 𓅓"
	@./$(NAME)

# Compiling object files
$(SRCS_DIR)/%.o: $(SRCS_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Cleaning object files
clean:
	@rm -f $(OBJS) $(NAME)
	@echo "\033[90m[\033[91mDeleting\033[90m]\033[31m Object files deleted\033[0m"

# Full cleanup
fclean: clean
	@rm -f $(NAME)
	@echo "\033[90m[\033[91mDeleting\033[90m]\033[31m Executable deleted.\033[0m"

# Rebuild everything
re: fclean all

.PHONY: all clean fclean re
