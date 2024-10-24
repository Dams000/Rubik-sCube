CC = gcc
CFLAGS = -Wall -Wextra -g -O0 #-fsanitize=address
LIBS = -L./include/ -lraylib -lm -pthread
INCLUDES = -I./include
OUTPUT = rubiks

OBJDIR = build

SRC = include/raygui.c \
      kociemba/twoPhase.c \
      kociemba/move.c \
      kociemba/faceCube.c \
      kociemba/enums.c \
      kociemba/cubieCube.c \
      kociemba/coordCube.c \
      average.c \
      cublet.c \
      cube.c \
      queue.c \
      timer.c \
      scramble.c \
      rubiksCube.c \
      utils.c

OBJ = $(patsubst %.c, $(OBJDIR)/%.o, $(SRC))

all: $(OUTPUT)

$(OUTPUT): $(OBJ)
	@echo "===== Linking $@... ====="
	$(CC) $(OBJ) $(LIBS) -o $(OUTPUT)

$(OBJDIR)/%.o: %.c
	@echo "===== Compiling $<... ====="
	@mkdir -p $(dir $@) 
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	@echo "Cleaning up..."
	rm -rf $(OBJDIR) $(OUTPUT)

.PHONY: all clean
