#include "life.h"

int main (int argc, char * argv[]) {
  unsigned int RULE = 0;
  int rule_size = 0;
  char * rulestr = NULL;
  printf("Rule Integer: ");
  getline(&rulestr, (size_t *) &rule_size, stdin);
  RULE = atoi(rulestr);
  if (RULE > 262143) {
    fprintf(stderr, "Invalid integer\n");
    exit(EXIT_FAILURE);
  }
  srand(time(NULL));
  int ch = 0;
  int width = 36;
  int height = 18;
  char * base_map = NULL;
  base_map = malloc(height * width+1);
  if (base_map == NULL) {
    fprintf(stderr, "Ran out of memory\n");
    exit(EXIT_FAILURE);
  }
  memset(base_map, 0, height * width + 1);
  memset(base_map, '.', height * width);
  char * map = NULL;
  map = malloc(height * width+1);
  memset(map, 0, height * width + 1);
  memcpy(map, base_map, height * width);
  int x = 3;
  int y = 3;
  int * invent_item_count = NULL;
  initscr();
  raw();
  curs_set(0);
  keypad(stdscr, TRUE);
  noecho();
  while (ch != 'q') {
    erase();
    if (ch == KEY_UP) {
      if (y > 0) y--;
    } else if (ch == KEY_DOWN) {
      if (y < height - 1) y++;
    } else if (ch == KEY_LEFT) {
      if (x > 0) x--;
    } else if (ch == KEY_RIGHT) {
      if (x < width - 1) x++;
    } else if (ch == KEY_A1) {
      if (x > 0) x--;
      if (y > 0) y--;
    } else if (ch == KEY_A3) {
      if (x < width - 1) x++;
      if (y > 0) y--;
    } else if (ch == KEY_C1) {
      if (x > 0) x--;
      if (y < height - 1) y++;
    } else if (ch == KEY_C3) {
      if (x < width - 1) x++;
      if (y < height - 1) y++;
    }
    memcpy(map, base_map, height * width);
    for (int i = 0; i < height; ++i) {
      if (i == y) {
	printw("%.*s", x-1, map + (i * width));
	standout();
	printw("%c", *(map + (i * width) + x));
	standend();
	printw("%.*s\n", width-x, (map + (i * width) + x + 1));
      } else {
	printw("%.*s\n", width, (map + (i * width)));
      }
    }
    refresh();
    ch = getch();
  }
  endwin();
  free(base_map);
  free(map);
  exit(EXIT_SUCCESS);
}
