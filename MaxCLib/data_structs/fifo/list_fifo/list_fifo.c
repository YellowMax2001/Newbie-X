#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#include <lish.h>

#include <list_fifo.h>

struct list_fifo_elem {
    struct list_head list_idle;
    struct list_head list_valid;
    struct list_head list_used;
    unsigned int buf_cnt;
    struct list_fifo_attr list_attr;
    struct list_fifo list_fifo;

    pthread_mutex_t list_lock;
    pthread_cond_t list_cond;
    unsigned char bitBufIdMap[(MAX_PRESET_NUM+7)/8];
    bool b_wait_used_empty;
};

struct list_fifo_buf_internal {
    struct list_fifo_buf list_buf;
    struct list_head mlist;
};

static int push_fifo(struct list_fifo *fifo, struct list_fifo_buf *buf)
{
    int ret = 0;
    struct list_fifo_elem *fifo_elem =
        (struct list_fifo_elem*)fifo->private;

    pthread_mutex_lock(&fifo_elem->list_lock);

    if (list_empty(&fifo_elem->list_idle)
        && fifo_elem->buf_cnt >= fifo_elem->list_attr.max_num) {
        ret = NB_BUFFER_FULL;
        goto Ebuffer_full;
    }

    if (list_empty(&fifo_elem->list_idle)) {
        struct list_fifo_buf_internal *buf =
            malloc(sizeof(struct list_fifo_buf_internal));

        if (NULL == buf) {
            ret = NB_NO_MEMORY;
            goto Ebuf_no_mem;
        }

        list_add_tail(&buf->mlist, &fifo_elem->list_idle);
    }

    struct list_fifo_buf_internal *buf_push;
    buf_push = list_first_entry(&fifo_elem->list_idle,
        struct list_fifo_buf_internal, mlist);

    memcpy(&buf_push->list_buf, buf, sizeof(struct list_fifo_buf_internal));
    /* This means the user can not keep his private buffers all the time where it may be used.
     * then we malloc one memory space and keep it. */
    if (!buf_push->list_buf.malloc_by_self) {
        buf_push->list_buf.pdata =
            malloc(sizeof(buf_push->list_buf.pdata_len));
        if (NULL == buf_push->list_buf.pdata) {
            nb_loge("There has no memory for private buffer. errno(%d).\n", errno);
            ret = NB_NO_MEMORY;
            goto Epbuf_no_mem;
        }

        memcpy(buf_push->list_buf.pdata, buf->pdata, buf->pdata_len);
    }

    /* Alloc one message id. */
    unsigned int i = 0;
    int iSizeofMapElem = sizeof(fifo_elem->bitBufIdMap[0])*8;
    for (i = 0; i < sizeof(fifo_elem->bitBufIdMap)*iSizeofMapElem); i++) {
        if (!(fifo_elem->bitBufIdMap[i/iSizeofMapElem] & 1 << (i%iSizeofMapElem))) {
            buf_push->list_buf.id = i;
        }
    }
    fifo_elem->buf_cnt++;

    list_move_tail(&buf_push->mlist, &fifo_elem->list_valid);

/* We needn't free the memory malloced for <struct list_fifo_buf_internal> */
Epbuf_no_mem:
Ebuf_no_mem:
Ebuffer_full:
    pthread_mutex_unlock(&fifo_elem->list_lock);
    return ret;
}

static int pull_fifo(struct list_fifo *fifo, struct list_fifo_buf *buf)
{
    int ret = 0;
    struct list_fifo_elem *fifo_elem =
        (struct list_fifo_elem*)fifo->private;

    pthread_mutex_lock(&fifo_elem->list_lock);

    if (list_empty(&fifo_elem->list_valid)) {
        nb_loge("There has no valid fifo buffer.\n");
        ret = NB_BUFFER_EMPTY;
        goto Ebuf_empty;
    }

    struct list_fifo_buf_internal *buf_pull;
    buf_pull = list_first_entry(&fifo_elem->list_valid,
        struct list_fifo_buf_internal, mlist);

    memcpy(buf, &buf_pull->list_buf, sizeof(struct list_fifo_buf));

    list_move_tail(&buf_pull->mlist, &fifo_elem->list_used);

Ebuf_empty:
    pthread_mutex_unlock(&fifo_elem->list_lock);
    return ret;
}

static int release_fifo(struct list_fifo *fifo, struct list_fifo_buf *buf)
{
    int ret = 0;
    struct list_fifo_elem *fifo_elem =
        (struct list_fifo_elem*)fifo->private;

    pthread_mutex_lock(&fifo_elem->list_lock);

    if (list_empty(&fifo_elem->list_used)) {
        nb_loge("There has no used fifo buffer.\n");
        ret = NB_BUFFER_EMPTY;
        goto Ebuf_empty;
    }

    struct list_fifo_buf_internal *buf_cur;
    struct list_fifo_buf_internal *buf_next;
    bool bFindFlag= 0;
    list_for_each_entry_safe
        (buf_cur, buf_next, &fifo_elem->list_used, mlist)
    {
        if (buf_cur->list_buf.id == buf->id) {
            list_del(&buf_cur->mlist);
            bFindFlag = 1;
            break;
        }
    }

    if (bFindFlag) {
        int iSizeofMapElem = sizeof(fifo_elem->bitBufIdMap[0])*8;
        if (!buf_cur->list_buf.malloc_by_self)
            free(buf_cur->list_buf.pdata);
        list_add_tail(&buf_cur->mlist, &fifo_elem->list_idle);

        fifo_elem->bitBufIdMap[buf_cur->list_buf.id/iSizeofMapElem]
            &= ~(1 << (buf_cur->list_buf.id%iSizeofMapElem));
        fifo_elem->buf_cnt--;
        if (fifo_elem->b_wait_used_empty)
            pthread_cond_signal(&fifo_elem->list_cond, &fifo_elem->list_lock);
    } else {
        nb_loge("Cannot find message id is [%d].\n", buf->id);
    }

Ebuf_empty:
    pthread_mutex_unlock(&fifo_elem->list_lock);
    return ret;
}

static int wait_release_all_fifos(struct list_fifo *fifo)
{
    int ret = 0;
    struct list_fifo_elem *fifo_elem =
        (struct list_fifo_elem*)fifo->private;

    pthread_mutex_lock(&fifo_elem->list_lock);

    while (!list_empty(&fifo_elem->list_used)) {
        fifo_elem->b_wait_used_empty = 1;
        pthread_cond_wait(&fifo_elem->list_cond, &fifo_elem->list_lock);
    }

    fifo_elem->b_wait_used_empty = 0;

    pthread_mutex_unlock(&fifo_elem->list_lock);
    return ret;
}

static int reset_fifo(struct list_fifo *fifo)
{
    int ret = 0;
    struct list_fifo_elem *fifo_elem =
        (struct list_fifo_elem*)fifo->private;

    pthread_mutex_lock(&fifo_elem->list_lock);

    if (list_empty(&fifo_elem->list_valid) && list_empty(&fifo_elem->list_used)) {
        nb_loge("There has no valid or used fifo buffer.\n");
        goto Ebuf_empty;
    }

#if 1
    struct list_fifo_buf_internal *buf_cur, *buf_next;
    list_for_each_entry_safe
        (buf_cur, buf_next, &fifo_elem->list_valid, mlist)
    {
        list_move_tail(&buf_cur->mlist, &fifo_elem->list_idle);
    }

    list_for_each_entry_safe
        (buf_cur, buf_next, &fifo_elem->list_used, mlist)
    {
        list_move_tail(&buf_cur->mlist, &fifo_elem->list_idle);
    }
#else
    list_splice_tail_init(&fifo_elem->list_valid, &fifo_elem->list_idle);
    list_splice_tail_init(&fifo_elem->list_used, &fifo_elem->list_idle);
#endif

Ebuf_empty:
    pthread_mutex_lock(&fifo_elem->list_lock);
    return ret;
}

static int revert_fifo(struct list_fifo *fifo, enum fifo_type type)
{
    int ret = 0;
    struct list_fifo_elem *fifo_elem =
        (struct list_fifo_elem*)fifo->private;

    pthread_mutex_lock(&fifo_elem->list_lock);

    switch ()

    pthread_mutex_unlock(&fifo_elem->list_lock);
    return ret;
}

static int merge_fifo(struct list_fifo *fifo,
     enum fifo_type type_head, enum fifo_type type_body)
{
    struct list_fifo_elem *fifo_elem =
        (struct list_fifo_elem*)fifo->private;
    struct list_head *head = NULL, *body = NULL;

    switch (type_head) {
        case YM_LIST_FIFO_IDLE:
            head = &fifo_elem->list_idle;
        case YM_LIST_FIFO_VALID:
            head = &fifo_elem->list_valid;
        case YM_LIST_FIFO_USED:
            head = &fifo_elem->list_used;
    }

    switch (type_body) {
        case YM_LIST_FIFO_IDLE:
            body = &fifo_elem->list_idle;
        case YM_LIST_FIFO_VALID:
            body = &fifo_elem->list_valid;
        case YM_LIST_FIFO_USED:
            body = &fifo_elem->list_used;
    }

    if (head && body)
        list_splice_tail_init(head, body);

    return 0;
}

void get_fifo_attr(struct list_fifo *fifo, struct list_fifo_attr *attr)
{
    struct list_fifo_elem *fifo_elem =
        (struct list_fifo_elem*)fifo->private;

    memcpy(attr, &fifo_elem->list_attr, sizeof(struct list_fifo_attr));
}

struct list_fifo *list_fifo_create(struct list_fifo_attr *fifo_attr)
{
    stuct list_fifo_elem *elem = NULL;

    elem = (struct list_fifo_elem*)malloc(sizeof(struct list_fifo_elem));
    if (NULL == elem) {
        nb_loge("Alloc list_fifo_elem failed. errno[%d].\n", errno);
        goto Ealloc_elem;
    }
    memset(elem, 0, sizeof(struct list_fifo_elem));

    pthread_mutex_init(&elem->list_lock, NULL);
    pthread_cond_init(&elem->list_cond, NULL);

    INIT_LIST_HEAD(&elem->list_idle);
    INIT_LIST_HEAD(&elem->list_valid);
    INIT_LIST_HEAD(&elem->list_used);
    memcpy(&elem->list_attr, fifo_attr, sizeof(struct list_fifo_attr));
    if (elem->list_attr.max_num > MAX_PRESET_NUM) {
        nb_loge("The max message number set by yourself"
            "is larger than [%d], wiil be set to [%d].\n", MAX_PRESET_NUM, MAX_PRESET_NUM);
    }

    elem->list_attr.max_num = min(elem->list_attr.max_num, MAX_PRESET_NUM);

    int i = 0;
    struct list_fifo_buf_internal *buf_tmp = NULL;
    for (i = 0; i < fifo_attr->preset_num && i < MAX_PRESET_NUM; i++) {
        buf_tmp = malloc(sizeof(typeof(buf_tmp)));
        if (NULL == buf_tmp) {
            nb_loge("Alloc list_fifo_buf failed, now have keep[%d]."
                " errno[%d]\n", errno, i);
            break;
        }
        memset(buf_tmp, 0, siezof(typeof(buf_tmp)));

        list_add_tail(&buf_tmp->mlist, &elem->list_idle);
    }
    nb_logd("Alloc %d list_fifo_buf when fifo_elem is initialized.\n", i);

    struct list_fifo *list_fifo = &elem->list_fifo;
    list_fifo->private = (void *)elem;
    list_fifo->push_fifo = push_fifo;
    list_fifo->pull_fifo = pull_fifo;
    list_fifo->release_fifo = release_fifo;
    list_fifo->wait_release_all_fifos = wait_release_all_fifos;
    list_fifo->reset_fifo = reset_fifo;
    list_fifo->revert_fifo = revert_fifo;
    list_fifo->merge_fifo = merge_fifo;
    list_fifo->get_fifo_attr = get_fifo_attr;

    memcpy(fifo_attr, &elem->list_attr, sizeof(struct list_fifo_attr));

Ealloc_buf:
    return &elem->list_fifo;

Ealloc_elem:
    return NULL;
}

void list_fifo_destroy(struct list_fifo *fifo)
{
    if (!fifo || !fifo->private) {
        nb_loge("You input a wrong param, null pointer.");
        return;
    }

    struct list_fifo_elem *elem =
            (struct list_fifo_elem*)fifo->private;
    if (!list_empty(&elem->list_used))
        nb_logw("Still got some using fifo elements, you will lose it.");

    struct list_fifo_buf_internal *buf_tmp = NULL, *buf_next = NULL;
    list_for_each_entry_safe(buf_tmp, buf_next, &elem->list_used, mlist)
    {
        list_del(&buf_tmp->mlist);
        free(buf_tmp);
    }

    list_for_each_entry_safe(buf_tmp, buf_next, &elem->list_valid, mlist)
    {
        list_del(&buf_tmp->mlist);
        free(buf_tmp);
    }

    list_for_each_entry_safe(buf_tmp, buf_next, &elem->list_idle, mlist)
    {
        list_del(&buf_tmp->mlist);
        free(buf_tmp);
    }

    free(elem);
}
