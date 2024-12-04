# Compiler and flags
CC = clang
CFLAGS = -fcolor-diagnostics -fansi-escape-codes -g -Wall
LDFLAGS = -mconsole

# Targets and source files
TARGET = test_malloc
SRC = malloc.c test_malloc.c
OBJ = $(SRC:.c=.o)

# Default target: build the program
all: $(TARGET)

# Link the object files into the final executable
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up generated files
clean:
	rm -f $(OBJ) $(TARGET).exe

# Run the program
run: $(TARGET)
	./$(TARGET).exe
