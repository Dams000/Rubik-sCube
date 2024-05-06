gcc -Wall -Wextra -o rubiks cublet.c cube.c timer.c scramble.c rubiksCube.c -L ./include/ -lraylib -lm -pthread -g -O0 -fsanitize=address
