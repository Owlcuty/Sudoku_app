CC = g++

CFLAGS = -Wall -o
SFML_FLAGS = -lsfml-graphics -lsfml-window -lsfml-system

NAME = Sudoku

all: exec main.o Sudoku.o

exec: 
	$(CC) $(CFLAGS) $(NAME) main.cpp $(SFML_FLAGS) && ./$(NAME)

main.o:
	$(CC) $(CFLAGS) $(NAME) main.cpp $(SFML_FLAGS)

Sudoku.o:
	./$(NAME)
