# Makefile stuff
include_directories(.)
add_executable(life life.c life.h updatemap.c updatemap.h menus.c menus.h)
target_compile_features(life PUBLIC c_function_prototypes c_variadic_macros)
target_include_directories(life PRIVATE "${CURSES_INCLUDE_DIRS}")
target_link_libraries(life "${CURSES_LIBRARIES}")
