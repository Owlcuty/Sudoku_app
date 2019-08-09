CC = g++

CFLAGS = -Wall -o
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

NAME = Sudoku

SOLVE_NAME = Solver

all: exec main.o Sudoku.o

exec: 
	$(CC) $(CFLAGS) $(NAME) main.cpp $(SFML_FLAGS) && ./$(NAME)

main.o:
	$(CC) $(CFLAGS) $(NAME) main.cpp $(SFML_FLAGS)

Sudoku.o:
	./$(NAME)

solver.o:
	$(CC) $(CFLAGS) $(SOLVE_NAME) solver.cpp && ./$(SOLVE_NAME)

