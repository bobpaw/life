#ifndef NARG
#define NARG(...) NARG_(__VA_ARGS__, RSEQ_N)
#define NARG_(...) NARG_N(__VA_ARGS__)
#define RSEQ_N 8, 7, 6, 5, 4, 3, 2, 1, 0
#define NARG_N(_1, _2, _3, _4, _5, _6, _7, _8, N, ...) N
#endif

#ifndef foreach
#define FOREACH_0(func) } while (0)
#define FOREACH_1(func, first) func(first); FOREACH_0(func)
#define FOREACH_2(func, first,...) func(first); FOREACH_1(func, __VA_ARGS__)
#define FOREACH_3(func, first,...) func(first); FOREACH_2(func, __VA_ARGS__)
#define FOREACH_4(func, first,...) func(first); FOREACH_3(func, __VA_ARGS__)
#define FOREACH_5(func, first,...) func(first); FOREACH_4(func, __VA_ARGS__)
#define FOREACH_6(func, first,...) func(first); FOREACH_5(func, __VA_ARGS__)
#define FOREACH_7(func, first,...) func(first); FOREACH_6(func, __VA_ARGS__)
#define FOREACH_8(func, first,...) func(first); FOREACH_7(func, __VA_ARGS__)

#define CONCAT1(x, y) x##y
#define CONCATENATE(x, y) CONCAT1(x, y)

#define foreach(func, ...) do { CONCATENATE(FOREACH_,NARG(__VA_ARGS__))(func, __VA_ARGS__)
#endif

