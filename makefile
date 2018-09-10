all:
	g++ -g -Wall -o run main.cpp -lmenu -lncurses
test:
	gcc -g -Wall -o test menu_test.c -lmenu -lncurses

