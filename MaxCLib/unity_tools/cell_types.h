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
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
#elif (__SIZEOF_POINTER__ == 8)
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
#else
typedef unsigned int uint32_t;
typedef unsigned short uint16_t;
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

#endif /* endof _YM_TYPES_ */


