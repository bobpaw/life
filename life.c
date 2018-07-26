#include "life.h"

// Declare TRUE and FALSE
#ifndef TRUE
#define TRUE (1==1)
#define FALSE (!TRUE)
#endif

char * update_map (char * base_map, const int width, const int height, char livecell, char deadcell, const unsigned int ruleint) {
  char * intermap = malloc((width * height)+1);
  memcpy(intermap, base_map, (width*height)+1);
  // Change intermap and stuff
  for (int i = 0, x = 0, y = 0, surround = 0; i < width*height; i++, surround = 0) {
    x = i % width;
    y = (i - x)/width;
    /*
      Where i is the current cell:
      A1 | A2 | A3
      ----|----|----
      B1 | i  | B3
      ----|----|----
      C1 | C2 | C3
    */
    if (x > 0) {
      if (base_map[i-1] == livecell) surround++; // B1
      if (y > 0) {
	if (base_map[i-width-1] == livecell) surround++; // A1
      }
      if (y < height) {
	if (base_map[i-1+width] == livecell) surround++; // C1
      }
    }
    if (x < width - 1) {
      if (base_map[i+1] == livecell) surround++; // B3
      if (y > 0) {
	if (base_map[i+1-width] == livecell) surround++; // A3
      }
      if (y < height) {
	if (base_map[i+1+width] == livecell) surround++; // C3
      }
    }
    if (y > 0) {
      if (base_map[i-width] == livecell) surround++;
    }
    if (y < height) {
      if (base_map[i+width] == livecell) surround++;
    }
    if (base_map[i] == deadcell) {
      if ((ruleint & (1 << surround)) == (1 << surround)) {
	intermap[i] = livecell;
      }
    } else if (base_map[i] == livecell) {
      if ((ruleint & (1 << (surround + 9))) != (1 << (surround + 9))) {
	intermap[i] = deadcell;
      }
    }
  }
  // Rewrite map
  memcpy(base_map, intermap, (width*height)+1);
  free(intermap);
  return base_map;
}

int main (int argc, char * argv[]) {
  unsigned int RULE = 0;
  struct gengetopt_args_info args_info;
  if (cmdline_parser(argc, argv, &args_info) != 0) exit(EXIT_FAILURE);
  if (args_info.ruleint_given) {
    RULE = args_info.ruleint_arg;
  } else {
    int rule_size = 0;
    char * rulestr = NULL;
    printf("Rule Integer: ");
    getline(&rulestr, (size_t *) &rule_size, stdin);
    RULE = atoi(rulestr);
    if (RULE > 262143) {
      fprintf(stderr, "Invalid integer\n");
      exit(EXIT_FAILURE);
    }
  }
  srand(time(NULL));
  int ch = 0;
  int width = 36;
  int height = 18;
  int playing = 0;
  unsigned int delay = 0; // Counts up to 10 so display is slow
  int livecell = '#';
  int deadcell = '.';
  if (args_info.width_given) width = args_info.width_arg;
  if (args_info.height_given) height = args_info.height_arg;
  if (args_info.delay_given) delay = args_info.delay_arg;
  cmdline_parser_free(&args_info);
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
    if (playing == FALSE) {
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
      } else if (ch == '\n') {
	playing = TRUE;
	timeout(50);
      }
    } else {
      if (ch == '\n') {
	playing = FALSE;
	timeout(-1);
      }
    }
    for (int i = 0; i < height; ++i) {
      if (i == y) {
	printw("%.*s", x, map + (y * width));
	standout();
	printw("%.1s", map + (y * width) + x);
	standend();
	printw("%.*s\n", width-(x+1), (map + (i * width) + x + 1));
      } else {
	printw("%.*s\n", width, (map + (i * width)));
      }
    }
    printw("(%d, %d)\n", x, y);
    refresh();
    if (playing == TRUE) {
      delay++;
      if (delay == 10) {
	delay = 0;
	update_map(map, width, height, livecell, deadcell, RULE);
      }
    }
    ch = getch();
  }
  endwin();
  free(map);
  exit(EXIT_SUCCESS);
}
