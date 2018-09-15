#include "interface.h"

menu::menu()
{
  init();
}

menu::~menu()
{
  int i = 0;
  unpost_menu(my_menu);
  for(i = 0; i < num_choices; ++i)
    free_item(my_items[i]);
  free_menu(my_menu);
  delwin(menu_win);
  //endwin();
}

int menu::init()
{
  int i = 0;
  curs_set(0); //set cursor to invisible (1 is normal, 2 is very visible)
  keypad(stdscr, TRUE);
  init_pair(1, COLOR_RED, COLOR_BLACK);
  init_pair(2, COLOR_GREEN, COLOR_BLACK);
  init_pair(3, COLOR_MAGENTA, COLOR_BLACK);

  choices = new char * [6];
  choices[0] = new char [strlen("choice 1") + 1];
  strcpy(choices[0], "choice 1");
  choices[1] = new char [strlen("choice 2") + 1];
  strcpy(choices[1], "choice 2");
  choices[2] = new char [strlen("choice 3") + 1];
  strcpy(choices[2], "choice 3");
  choices[3] = new char [strlen("choice 4") + 1];
  strcpy(choices[3], "choice 4");
  choices[4] = new char [strlen("Exit") + 1];
  strcpy(choices[4], "Exit");

  //num_choices = ARRAY_SIZE(choices); 
  num_choices = 6; 
  my_items = (ITEM **)calloc(num_choices + 1, sizeof(ITEM *));

  my_items[0] = new_item(choices[0], choices[0]);
  //set_item_userptr(my_items[0], (void *) choice1);

  for(i = 1; i < num_choices; ++i)
  {       
    my_items[i] = new_item(choices[i], choices[i]);
    /* Set the user pointer */
    //set_item_userptr(my_items[i], (void *) func);
  }
  my_items[num_choices] = (ITEM *)NULL;

  /* Create menu */
  my_menu = new_menu((ITEM **)my_items);

  //Initialize and set menu window
  height = 10;
  width = 40;
  starty = 3; 
  startx = 1; 
  menu_win = newwin(height, width, starty, startx);
  keypad(menu_win, TRUE);

  set_menu_win(my_menu, menu_win);
  set_menu_sub(my_menu, derwin(menu_win, 6, 38, 3, 1));

  /* Set menu mark to the string " * " */
  set_menu_mark(my_menu, " * ");

  /* Print a border around the main window and print a title */
  //box(menu_win, 0, 0);
  wborder(menu_win, '|', '|', '-', '-', '+', '+', '+', '+'); //set border of window
  char * menu_title= 0;
  menu_title = new char [strlen("My Menu") + 1];
  strcpy(menu_title, "My Menu");
  print_in_middle(menu_win, 1, 0, 40, menu_title, COLOR_PAIR(1));
  //mvwaddch(menu_win, 2, 0, ACS_LTEE);
  mvwhline(menu_win, 2, 1, '-', 38);
  //mvwaddch(menu_win, 2, 39, ACS_RTEE);
  mvprintw(LINES - 2, 0, "'q' to exit");

  //refresh();

  /* Post the menu */
  //mvprintw(LINES - 3, 0, "Press <ENTER> to see the option selected");
  //mvprintw(LINES - 2, 0, "Up and Down arrow keys to naviage ('q' to Exit)");
  //post_menu(my_menu);
  //wrefresh(menu_win);
  //ref();
  return 0;
}

void menu::print_in_middle(WINDOW *win, int starty, int startx, int width, char *string, chtype color)
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
  //refresh();
}

void menu::ref()
{
  refresh();
  post_menu(my_menu);
  wrefresh(menu_win);
}

int menu::get_input()
{
  int c = 0;
  this->ref();
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
    wrefresh(menu_win);
  } 
  return 0;
}

interface::interface()
{
  init();
}

interface::~interface()
{
  endwin();
}

int interface::init()
{
  initscr();
  if(LINES < 36 || COLS < 64)
  {
    printf("Terminal size too small (%d,%d). \
    Please make your window larger.\n", LINES, COLS);
    endwin();
    return -1;
  }
  start_color();
  cbreak();
  noecho();
  curs_set(0); //set cursor to invisible (1 is normal, 2 is very visible)
  keypad(stdscr, TRUE);
  return 0;
}

void interface::ref()
{
  ref();
}

void interface::start()
{
  main_menu.get_input();
}

