COMPILER = g++
FLAGS = -O3

make:
	$(COMPILER) $(FLAGS) puzzle.cpp -o calendar_puzzle_solver

.PHONY: clean
clean:
	rm calendar_puzzle_solver

