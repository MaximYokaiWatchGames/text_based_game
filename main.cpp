//#include <iostream>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>
#include <menu.h>

//#define ARRAY_SIZE 5
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

using namespace std;

char *choices[] = {
  "Choice 1",
  "Choice 2",
  "Choice 3",
  "Choice 4",
//  "Choice 5",
//  "Choice 6",
//  "Choice 7",
  "Exit",
  (char *) NULL,
};

void func(char *name);
void choice1(char *name);
void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color);

int main()
{ 
  ITEM **my_items;
  int c;        
  MENU * my_menu;
  int n_choices, i;
  ITEM * cur_item;
  WINDOW * my_menu_win;
  int startx, starty, width, height;
  int ch;

  /* Initialize curses */ 
  initscr();
  /*
  if(!has_colors())
  {
    printw("Terminal has no colors!");
    printf("\nTerminal has no colors!\n");
    refresh();
    endwin();
    return -1;
  }
  */
  if(LINES < 36 || COLS < 64)
  {
    printf("Terminal size too small (%d,%d). Please make your window larger.\n", LINES, COLS);
    endwin();
    return -1;
  }
   
  start_color();
  cbreak();
  noecho();
  curs_set(0); //set cursor to invisible (1 is normal, 2 is very visible)
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

  //Initialize and set menu window
  height = 10;
  width = 40;
  starty = 3; 
  startx = 1; 
  my_menu_win = newwin(height, width, starty, startx);
  keypad(my_menu_win, TRUE);

  set_menu_win(my_menu, my_menu_win);
  set_menu_sub(my_menu, derwin(my_menu_win, 6, 38, 3, 1));

  /* Set menu mark to the string " * " */
  set_menu_mark(my_menu, " * ");

  /* Print a border around the main window and print a title */
  //box(my_menu_win, 0, 0);
  wborder(my_menu_win, '|', '|', '-', '-', '+', '+', '+', '+'); //set border of window
  print_in_middle(my_menu_win, 1, 0, 40, "My Menu", COLOR_PAIR(1));
  //mvwaddch(my_menu_win, 2, 0, ACS_LTEE);
  mvwhline(my_menu_win, 2, 1, '-', 38);
  //mvwaddch(my_menu_win, 2, 39, ACS_RTEE);
  mvprintw(LINES - 2, 0, "'q' to exit");

  refresh();

  /* Post the menu */
  //mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
  //mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviage ('q' to Exit)");
  post_menu(my_menu);
  wrefresh(my_menu_win);
  //refresh();

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
    wrefresh(my_menu_win);
  } 
  unpost_menu(my_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(my_items[i]);
  free_menu(my_menu);
  endwin();
}

#if 0
WINDOW * create_newwin(int height, int width, int starty, int startx)
{
  WINDOW * local_win;
  local_win = newwin(height, width, starty, startx);
  box(local_win, 0 , 0);    /* 0, 0 gives default characters 
                             * for the vertical and horizontal
                             *           * lines      */
  wrefresh(local_win);    /* Show that box    */
  return local_win;
}

void destroy_win(WINDOW *local_win)
{ 
  /* box(local_win, ' ', ' '); : This won't produce the desired
   *   * result of erasing the window. It will leave it's four corners 
   *     * and so an ugly remnant of window. 
   *       */
  wborder(local_win, ' ', ' ', ' ',' ',' ',' ',' ',' ');
  /* The parameters taken are 
     1. win: the window on which to operate
     2. ls: character to be used for the left side of the window 
     3. rs: character to be used for the right side of the window 
     4. ts: character to be used for the top side of the window 
     5. bs: character to be used for the bottom side of the window 
     6. tl: character to be used for the top left corner of the window 
     7. tr: character to be used for the top right corner of the window 
     8. bl: character to be used for the bottom left corner of the window 
     9. br: character to be used for the bottom right corner of the window
     */
  wrefresh(local_win);
  delwin(local_win);
}
#endif

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
{ 
  int length, x, y;
  float temp;

  if(win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if(startx != 0)
    x = startx;
  if(starty != 0)
    y = starty;
  if(width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length)/ 2;
  x = startx + (int)temp;
  wattron(win, color);
  mvwprintw(win, y, x, "%s", string);
  wattroff(win, color);
  refresh();
}

void func(char *name)
{ 
  move(20, 0);
  clrtoeol();
  mvprintw(20, 0, "Item selected is : %s LINES: %d COLS: %d", name, LINES, COLS);
}

void choice1(char *name)
{
  move(20, 0);
  clrtoeol();
  mvaddstr(20, 0, "Item selected is : ....SUCK IT");
}

