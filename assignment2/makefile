# Compiler settings
CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LDFLAGS =

# Directories
SRC_DIR = .
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.c)

# Object files
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Executable program
EXEC_NAME = my_prog
EXEC = $(BIN_DIR)/$(EXEC_NAME)

# Program command line arguments
CLA = 

# Phony targets
.PHONY:
	all clear help run final

all: $(EXEC)

# Create the executable program
$(EXEC): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(LDFLAGS) $^ -o $@

# Compile
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Run the program
run: $(EXEC)
	./$(EXEC) $(CLA)

# Clear files used by the program
clear:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Use valgrind - Output is saved at bin/valgrind.log
help: $(EXEC)
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose --log-file=./bin/valgrind.log ./$(EXEC) $(CLA)
