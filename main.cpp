#include <iostream>
#include <string>
#include <ncurses.h>

using namespace std;

int main()
{
  initscr(); //start curses mode
  printw("Hello World!!!"); //print
  refresh(); //print it on the real screen
  getch(); //wait for user input
  endwin(); //end curses mode

  return 0;
}
