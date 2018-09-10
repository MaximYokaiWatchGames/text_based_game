#include <iostream>
#include <stdlib.h>
//#include <string>
#include <ncurses.h>
#include <menu.h>

int main()
{
  initscr();
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

  int ch;
  while(1)
  {
    ch = getch(); endwin(); printf("KEY NAME : %s - %d\n", keyname(ch),ch);
  }

  endwin();

  return 0;
}


