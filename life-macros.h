#ifndef has
#define has(var, bit) (((var) & (bit)) == (bit))
#endif

#ifndef foreach
#define foreach(func, count, first,...)                         \
  {typeof(first) things[] = {__VA_ARGS__};                      \
    func(first);                                                \
    for (int _i = 0; _i < count - 1; _i++) func(things[_i]);}
#endif
