all:
	g++ -g -Wall -o run main.cpp -lmenu -lncurses
test:
	g++ -g -Wall -o test menu_test.c -lmenu -lncurses

