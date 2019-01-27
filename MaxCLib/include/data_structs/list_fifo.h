/*
 * list_fifo.h -- a fifo structure based on linked list
 *
 * Copyright 2018-2021 version 0.1
 *
 * Created by YellowMax <huangbohan2001@163.com>
 *
 */

#ifndef _YM_LIST_FIFO_
#define _YM_LIST_FIFO_

#include <stdbool.h>
#include <cell_types.h>
#include <list.h>

enum fifo_type {
    YM_LIST_FIFO_IDLE,
    YM_LIST_FIFO_VALID,
    YM_LIST_FIFO_USED,
};

/* You can push one fifo element with initilize one [list_fifo_buf].
* @id: this is reserved until now.
* @data1: the first data you want push.
* @data2: the second ...
* @pdata: if you have one extra data, push it into here.
* @pdata_len: how much length is you @pdata, Unit(char).
* @malloc_by_self: if the extra data is malloc by yourself(malloc_by_self == 1),
* then I think you can hold your datas memory until you don't need it, or
* I will malloc one mermory region to store you datas, and you don't
* need care about you datas life cycle.
*/
struct list_fifo_buf {
    unsigned int id;
    unsigned int data1;
    unsigned int data2;
    void *pdata;
    unsigned int pdata_len; /* Unit: char. */
    bool malloc_by_self;
};

#define MAX_PRESET_NUM 500

/* The fifo's attribute.
* @has_num_limit: if the fifo has a max elements limit, the max number is @preset_num
* @preset_num: how much elements do you need with your thinking.
* @max_num: just in case, if you push buffers more than @preset_num, and also should
* limit its max element number.
*/
struct list_fifo_attr {
    bool has_num_limit;
    unsigned int preset_num;
    unsigned int max_num;
};

/* The fifo elemention, users can do fifo operations with this structure.
* @push_fifo: push one element into fifo list.
* @pull_fifo: get one element from fifo list.
* @release_fifo: If you have used the @fifo_list_buf, then give it back.
* @wait_release_all_fifos: Wait if used is not empty.
* @reset_fifo: Invalid all valid fifo elements. NNNNNNNNotice: you responsible for this invoke.
* @revert_fifo: revert one fifo list.
* @merge_fifo: merge two fifo list, move @type_head list into &type_body list.
*/
struct list_fifo {
    int (*push_fifo)(struct list_fifo *fifo, struct list_fifo_buf *buf);
    int (*pull_fifo)(struct list_fifo *fifo, struct list_fifo_buf *buf);
    int (*release_fifo)(struct list_fifo *fifo, struct list_fifo_buf *buf);
    int (*wait_release_all_fifos)(struct list_fifo *fifo);
    int (*reset_fifo)(struct list_fifo *fifo, bool all_list);
    int (*revert_fifo)(struct list_fifo *fifo, enum fifo_type type);
    int (*merge_fifo)(struct list_fifo *fifo,
         enum fifo_type type_head, enum fifo_type type_body);

    void (*get_fifo_attr)(struct list_fifo *fifo, struct list_fifo_attr *attr);

    /* Used by list_fifo internal, do not touch it. */
    void *private;
};

struct list_fifo *list_fifo_create(struct list_fifo_attr *fifo_attr);
void list_fifo_destroy(struct list_fifo *fifo);

#endif /* end of _YM_LIST_FIFO_ */
