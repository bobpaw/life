AM_OPTIONS = gnu
ncurses_flags = `ncurses5-config --libs` `ncurses5-config --cflags`
bin_PROGRAMS = life
life_SOURCES = life.c cmdline-life.c
life_CFLAGS = -I$(srcdir) $(ncurses_flags)
include_HEADERS = life.h updatemap.h cmdline-life.h
