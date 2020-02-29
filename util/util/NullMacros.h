// Copyright 2016 Nicolaus Anderson

#ifndef NULL_DEFINING_HEADER
#define NULL_DEFINING_HEADER

#ifdef REAL_NULL
#undef REAL_NULL
#endif

#ifdef USE_CPP_11
#define REAL_NULL nullptr
#else
#define REAL_NULL 0
#endif

#ifdef isNull
#undef isNull
#endif
#define isNull(x) (x == REAL_NULL)

#ifdef notNull
#undef notNull
#endif
#define notNull(x) (x != REAL_NULL)

#endif
