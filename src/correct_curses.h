#ifndef GRAPH_CORRECT_CURSES_H
#define GRAPH_CORRECT_CURSES_H
#if defined(HAVE_CMAKECONFIG_H)
#include <cmakeconfig.h>

#if defined HAVE_NCURSESW_CURSES_H
#  include <ncursesw/curses.h>
#elif defined HAVE_NCURSESW_H
#  include <ncursesw.h>
#elif defined HAVE_NCURSES_CURSES_H
#  include <ncurses/curses.h>
#elif defined HAVE_NCURSES_H
#  include <ncurses.h>
#elif defined HAVE_CURSES_H
#  include <curses.h>
#else
#  error "SysV or X/Open-compatible Curses header file required"
#endif
#else

// Fingers-crossed
#include <curses.h>
#endif
#endif // GRAPH_CORRECT_CURSES_H