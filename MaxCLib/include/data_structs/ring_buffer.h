/*
 * ring_buffer.h -- a ring buffer manager
 *
 * Copyright 2018-2021 version 0.1
 *
 * Created by YellowMax <huangbohan2001@163.com>
 *
 */

#ifndef _YM_RING_BUFFER_
#define _YM_RING_BUFFER_

#include <stdbool.h>
#include <cell_types.h>

/* max ring buffer size 3GB */
#define MAX_RB_SIZE  (3*1024*1024*1024)

/* maybe a interlaced ring buffer should be used
*1.interlaced read&write
*2.pre-read
*/

/**
 * struct ring_buffer - ring buffer implement
 *!!!!!!!!!!!!!!!!!!the read write oprtation is not thread safe.!!!!!!!!!
 * @rb_in:	input data to ring buffer
 * @rb_in_f:	force input data to ring buffer,
 * even it will be overwritten(if it was used,RB_FL_FORCE_WRITE flag must be set)
 * @rb_out:	output data from ring buffer(not use until now)
 * @rb_reset:	reset the whole ring buffer
 * @rb_g_bufsize:	get the ring buffer length(the whole buffer size in bytes)
 * @rb_g_validnum: get valid data size in bytes
 * @rb_g_elemsize: get elem size in bytes
 * @private:user should not access it
 */
struct ring_buffer {
    int (*rb_in)(struct ring_buffer *rb_hd,
            void *buf_in, uint32_t elem_num);
    int (*rb_in_f)(struct ring_buffer *rb_hd,
            void *buf_in, uint32_t elem_num);
    int (*rb_out)(struct ring_buffer *rb_hd,
            void *buf_out, uint32_t elem_num);
    void (*rb_reset)(struct ring_buffer *rb_hd);

    uint32_t (*rb_g_bufsize)(struct ring_buffer *rb_hd);
    uint32_t (*rb_g_validnum)(struct ring_buffer *rb_hd);
    uint32_t (*rb_g_elemsize)(struct ring_buffer *rb_hd);

    void *private;
};

/*
*/
#define RB_FL_NONE        0UL
#define RB_FL_FORCE_WRITE 1<<0UL
#define RB_FL_THREAD_SAFE 1<<1UL

/**
 * struct ring_buffer_attr - ring buffer's attribution
 * @force_w:	bool type, choice if use force write operation
 * @safe_t: bool type, is the ring buffer operation thread safe
 */
struct ring_buffer_attr {
    bool force_w;
    bool safe_t;
};

/**
 * struct ring_buffer_entity - ring buffer internal implement
 * @rb_lock: lock of read write operation.
 * @rb_in:	input index
 * @rb_out:	output index
 * @elem_size: meta data size in bytes
 * @elem_cnt:	  how many meta data in this ring buffer
 * @rb_full: is this ring buffer is full?
 * @rb_buf: the real buffer address
 * @rb_handle: used by users, include some callbacks
 */
struct ring_buffer_entity {
    pthread_mutex_t rb_lock;
    uint32_t rb_in;
    uint32_t rb_out;

    uint32_t elem_size;
    uint32_t elem_cnt;
    bool rb_full;

    unsigned char *rb_buf;

    struct ring_buffer_attr rb_attr;
    struct ring_buffer rb_handle;
};

struct ring_buffer *ring_buffer_create
    (uint32_t elem_size, uint32_t elem_num, unsigned long flag);
void ring_buffer_destroy(struct ring_buffer *rb_hd);

#endif /* endof _YM_RING_BUFFER_ */

