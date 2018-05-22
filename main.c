#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <signal.h>
#include <unistd.h>

#include <plat_log.h>
#include <cell_types.h>

#include <data_structs/ring_buffer.h>

static bool sys_exit = 0;

#define ELEM_SIZE 4
#define ELEM_NUM  6

void signal_handle(int sig_num)
{
    sys_exit = 1;
}

void *rb_write_thread(void *arg)
{
    struct ring_buffer *rb = 
        (struct ring_buffer *)arg;

    int i = 0;
    int j = 0;
    char rb_buf[4*10];
    
    while (1) {
        if (sys_exit) {
            break;
        }

        for (i = 0; i < sizeof(rb_buf)/sizeof(typeof(rb_buf[0]))/4; i++) {
            rb_buf[4*i+0] = 4*j+0;
            rb_buf[4*i+1] = 4*j+1;
            rb_buf[4*i+2] = 4*j+2;
            rb_buf[4*i+3] = 4*j+3;
        }
        j++;

        rb->rb_in_f(rb, &rb_buf[0], ELEM_NUM);
        usleep(300*1000);
    }

    pthread_exit(NULL);
}

void *rb_read_thread(void *arg)
{
    struct ring_buffer *rb = 
        (struct ring_buffer *)arg;

    int i = 0;
    int ret = 0;
    char rb_buf[4];

    while (1) {
        if (sys_exit) {
            break;
        }
        ret = rb->rb_out(rb, &rb_buf[0], 1);
        if (ret > 0) {
            for (i = 0; i < ELEM_SIZE; i++) {
                printf("get one data(%d)\r\n", rb_buf[i]);
            }
        }
        usleep(1000*1000);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int ret = 0;
    pthread_t rd_trd;
    pthread_t wr_trd;

    struct ring_buffer *rb;

    rb = ring_buffer_create(ELEM_SIZE, ELEM_NUM, RB_FL_THREAD_SAFE);
    if (NULL == rb) {
        nb_loge("ring_buffer_create failed!!\r\n");
        ret = -1;
        goto erb_create;
    }

    ret = pthread_create(&rd_trd, NULL, rb_read_thread, (void *)rb);
    ret |= pthread_create(&wr_trd, NULL, rb_write_thread, (void *)rb);
    if (ret < 0) {
        nb_loge("create read&write thread failed!!\r\n");
        goto rtrd_create;
    }

    signal(SIGINT, signal_handle);

    pthread_join(wr_trd, NULL);
    pthread_join(rd_trd, NULL);

rtrd_create:
    ring_buffer_destroy(rb);
erb_create:
    return ret;
}

