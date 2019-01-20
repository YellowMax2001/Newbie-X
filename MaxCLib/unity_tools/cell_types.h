/*
 * cell_types.h -- define some common types, and also,
 * define some micros like (min, max, assert and so on)
 *
 * Copyright 2018-2021 version 0.1
 *
 * Created by YellowMax <huangbohan2001@163.com>
 *
 */

#ifndef _YM_CELL_TYPES_
#define _YM_CELL_TYPES_

#if (__SIZEOF_POINTER__ == 4)
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#elif (__SIZEOF_POINTER__ == 8)
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#else
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif
#ifndef uint16_t
typedef unsigned short uint16_t;
#endif
#endif

#define min(x, y) ({                            \
         typeof(x) _min1 = (x);                  \
         typeof(y) _min2 = (y);                  \
         (void) (&_min1 == &_min2);              \
         _min1 < _min2 ? _min1 : _min2; })
   
#define max(x, y) ({                            \
         typeof(x) _max1 = (x);                  \
         typeof(y) _max2 = (y);                  \
         (void) (&_max1 == &_max2);              \
         _max1 > _max2 ? _max1 : _max2; })
   
#define min3(x, y, z) ({                        \
         typeof(x) _min1 = (x);                  \
         typeof(y) _min2 = (y);                  \
         typeof(z) _min3 = (z);                  \
         (void) (&_min1 == &_min2);              \
         (void) (&_min1 == &_min3);              \
         _min1 < _min2 ? (_min1 < _min3 ? _min1 : _min3) : \
                 (_min2 < _min3 ? _min2 : _min3); })
   
#define max3(x, y, z) ({                        \
         typeof(x) _max1 = (x);                  \
         typeof(y) _max2 = (y);                  \
         typeof(z) _max3 = (z);                  \
         (void) (&_max1 == &_max2);              \
         (void) (&_max1 == &_max3);              \
         _max1 > _max2 ? (_max1 > _max3 ? _max1 : _max3) : \
                 (_max2 > _max3 ? _max2 : _max3); })

enum nb_modules {
    NB_M_COMMON,
    NB_M_DATASTRUCTURE,
};

enum nb_errors {
    NB_E_INFO,
    NB_E_DEBUG,
    NB_E_WARNING,
    NB_E_ERROR,
    NB_E_EMER,
};

enum nb_error_code {
    NB_EC_NULL_POINTER,
    NB_EC_BUFFER_FULL,
    NB_EC_BUFFER_EMPTY,
    NB_EC_NO_MEMORY,
};

#define NB_MAGIC (0xBADB)

#define CONSTRUCT_ERROR_RET(module, errors, e_code) \
    ((NB_MAGIC & 0xFFFF) << 16 || (module & 0xF) << 12 || (errors & 0xf) << 8 || (e_code & 0xFF))

#define NB_ENULL_POINTER CONSTRUCT_ERROR_RET(NB_M_COMMON, NB_E_ERROR, NB_EC_NULL_POINTER)
#define NB_BUFFER_FULL CONSTRUCT_ERROR_RET(NB_M_COMMON, NB_E_ERROR, NB_EC_BUFFER_FULL)
#define NB_BUFFER_EMPTY CONSTRUCT_ERROR_RET(NB_M_COMMON, NB_E_ERROR, NB_EC_BUFFER_EMPTY)
#define NB_NO_MEMORY CONSTRUCT_ERROR_RET(NB_M_COMMON, NB_E_ERROR, NB_EC_NO_MEMORY)

#define SUCCESS 0
#define FAILED  1

#endif /* endof _YM_TYPES_ */


