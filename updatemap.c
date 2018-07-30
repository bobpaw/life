#include "updatemap.h"

#ifndef has
#define has(var, bit) ((var & bit) == bit)
#endif

char * update_map (char * base_map, const int width, const int height, char livecell, char deadcell, const unsigned int ruleint) {
  char * intermap = (char*) strndup(base_map, width * height);
  for (int i = 0, x = 0, y = 0, surround = 0; i < width*height; i++, surround = 0) {
    x = i % width;
    y = (i - x) / width;
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
      if (y < height - 1) {
	if (base_map[i-1+width] == livecell) surround++; // C1
      }
    }
    if (x < width - 1) {
      if (base_map[i+1] == livecell) surround++; // B3
      if (y > 0) {
	if (base_map[i+1-width] == livecell) surround++; // A3
      }
      if (y < height - 1) {
	if (base_map[i + 1 + width] == livecell) surround++; // C3
      }
    }
    if (y > 0) {
      if (base_map[i - width] == livecell) surround++;
    }
    if (y < height - 1) {
      if (base_map[i + width] == livecell) surround++;
    }
    if (base_map[i] == deadcell) {
      if (has(ruleint, 1 << surround)) {
	intermap[i] = livecell;
      }
    } else if (base_map[i] == livecell) {
      if (has(ruleint, 1 << surround + 9)) {
	intermap[i] = deadcell;
      }
    }
  }

  // Rewrite map
  strcpy(base_map, intermap);
  free(intermap);
  return base_map;
}
