//#include <iostream>
#include <stdlib.h>
//#include <string>
#include <ncurses.h>
#include <menu.h>

//#define ARRAY_SIZE 5
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

//using namespace std;

char *choices[] = {
  "Choice 1",
  "Choice 2",
  "Choice 3",
  "Choice 4",
  "Choice 5",
  "Choice 6",
  "Choice 7",
  "Exit",
};

void func(char *name);
void choice1(char *name);

int main()
{ 
  ITEM **my_items;
  int c;        
  MENU *my_menu;
  int n_choices, i;
  ITEM *cur_item;

  /* Initialize curses */ 
  initscr();
  if(!has_colors())
  {
    printw("Terminal has no colors!");
    printf("\nTerminal has no colors!\n");
    refresh();
    endwin();
    return -1;
  }
    
  start_color();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

  /* Initialize items */
  n_choices = ARRAY_SIZE(choices);
  my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));

  my_items[0] = new_item(choices[0], choices[0]);
  set_item_userptr(my_items[0], (void *) choice1);

  for(i = 1; i < n_choices; ++i)
  {       
    my_items[i] = new_item(choices[i], choices[i]);
    /* Set the user pointer */
    set_item_userptr(my_items[i], (void *) func);
  }
  my_items[n_choices] = (ITEM *)NULL;

  /* Create menu */
  my_menu = new_menu((ITEM **)my_items);

  /* Post the menu */
  mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
  mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviage ('q' to Exit)");
  post_menu(my_menu);
  refresh();

  while((c = getch()) != 113 /* q */) //press 'q' to quit
  {
    switch(c)
    { 
      case KEY_DOWN:
      menu_driver(my_menu, REQ_DOWN_ITEM);
      break;
      case KEY_UP:
      menu_driver(my_menu, REQ_UP_ITEM);
      break;
      case 10: /* Enter */
      { 
        ITEM *cur;
        void (*p)(char *);

        cur = current_item(my_menu);
        p = (void (*)(char *))item_userptr(cur);
        wattron(stdscr, COLOR_PAIR(1));
        p((char *)item_name(cur));
        wattroff(stdscr, COLOR_PAIR(1));
        pos_menu_cursor(my_menu);
        break;
      }
      break;
    }
  } 
  unpost_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  free_menu(my_menu);
  endwin();
}

void func(char *name)
{ 
  move(20, 0);
  clrtoeol();
  mvprintw(20, 0, "Item selected is : %s", name);
}

void choice1(char *name)
{
  move(20, 0);
  clrtoeol();
  mvprintw(20, 0, "Item selected is : %s....SUCK IT", name);
}

