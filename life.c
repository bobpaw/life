#include "life.h"

char * update_map (const char * base_map, const int width, const int height, char livecell, char deadcell, const int ruleint) {
  char * ret = malloc((width * height)+1);
  memcpy(ret, base_map, (width*height)+1);
  return ret;
}

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
  int livecell = '#';
  int deadcell = '.';
  char * map = NULL;
  map = malloc((height * width)+1);
  memset(map, 0, height * width + 1);
  memset(map, deadcell, height * width);
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
    } else if (ch == ' ') {
      if (map[y*width+x] == deadcell) {
	map[y*width+x] = livecell;
      } else if (map[y*width+x] == livecell) {
	map[(y*width)+x] = deadcell;
      }
    }
    for (int i = 0; i < height; ++i) {
      if (i == y) {
	printw("%.*s", x, map + (y * width));
	standout();
	printw("%c", *(map + (y * width) + x));
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
  free(map);
  exit(EXIT_SUCCESS);
}
