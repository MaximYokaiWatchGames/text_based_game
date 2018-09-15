#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cstring>
#include <ncurses.h>
#include <menu.h>

#define MAX_OPTIONS 10
#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))
#define CTRLD 4

class menu
{
  public:
    menu();
    ~menu();
    int init();
    void ref();
    int get_input();
  protected:
  private:
    //Functions

    void print_in_middle(WINDOW *win, int starty, int startx, 
        int width, char *string, chtype color);
    //Data Members
    WINDOW * menu_win;
    MENU * my_menu;
    ITEM **my_items;
    char ** choices;
    int (*funcs[MAX_OPTIONS]) (void);
    int num_choices;
    int startx;
    int starty;
    int width;
    int height;
};

class interface 
{
  public:
    interface();
    ~interface();
    int init();
    void ref();
    void start();
  protected:
  private:
    menu main_menu;
};
