#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

#include <plat_log.h>

#define TIME_INIT_START_END(x) struct timeval start_##x; \
                       struct timeval end_##x; \
                       float DiffTime_##x;
#define TIME_GET_START(x) (gettimeofday(&start_##x, NULL))
#define TIME_GET_END(x) (gettimeofday(&end_##x, NULL)); \
                (DiffTime_##x = \
                (((end_##x.tv_sec + end_##x.tv_usec * 1e-6) - \
                (start_##x.tv_sec + start_##x.tv_usec * 1e-6)) * 1000))
#define TIME_PRINT_DIFF(x) (DiffTime_##x)

int gap_bubble_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int end_idx = 0;
    int swap_tmp = 0;
    int gap = size;
    bool swap_flag = 1;

    while(swap_flag || gap != 1) {
        swap_flag = 0;
        gap /= 1.3;
        gap = (gap < 1) ? 1 : gap;
        i++;

        end_idx = size-gap;
        for (idx = 0; idx < end_idx; idx++) {
            if (sort_butt[idx] > sort_butt[idx+gap]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx+gap];
                sort_butt[idx+gap] = swap_tmp;
                swap_flag = 1;
            }
        }
    }

end:
    nb_logd("[%s] %d\n", __FUNCTION__, i);
    return 0;
}

int fast_bubble_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int end_idx = size-1;
    int swap_tmp = 0;
    bool swap_flag = 0;

    for (i = 0; i < size-1; i++) {
        for (idx = 0; idx < end_idx; idx++) {
            if (sort_butt[idx] > sort_butt[idx+1]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx+1];
                sort_butt[idx+1] = swap_tmp;
                swap_flag = 1;
            }
        }
        if (!swap_flag) {
            nb_logd("i[%d]\n", i);
            goto end;
        }

        swap_flag = 0;
        end_idx--;
    }

end:
    nb_logd("[%s] ", __FUNCTION__);
    return 0;
}

int classic_bubble_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int end_idx = size;
    int swap_tmp = 0;

    for (i = 0; i < size; i++) {
        for (idx = 0; idx < end_idx; idx++) {
            if (sort_butt[idx] > sort_butt[idx+1]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx+1];
                sort_butt[idx+1] = swap_tmp;
            }
        }
    }

    nb_logd("[%s] ", __FUNCTION__);
    return 0;
}

int shaker_bubble_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int end_idx = size-1, start_idx = 0;
    int swap_tmp = 0;
    bool swap_flag = 0;

    while (start_idx < end_idx) {
        i++;
        for (idx = start_idx; idx < end_idx; idx++) {
            if (sort_butt[idx] > sort_butt[idx+1]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx+1];
                sort_butt[idx+1] = swap_tmp;
                swap_flag = 1;
            }
        }
        if (!swap_flag) {
            nb_logd("j 1[%d]\n", i);
            goto end;
        }
        swap_flag = 0;
        end_idx--;

        for (idx = end_idx; idx > start_idx; idx--) {
            if (sort_butt[idx] < sort_butt[idx-1]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx-1];
                sort_butt[idx-1] = swap_tmp;
                swap_flag = 1;
            }
        }

        if (!swap_flag) {
            nb_logd("j 2[%d]\n", i);
            goto end;
        }
        swap_flag = 0;
        start_idx++;
    }

end:
    nb_logd("[%s] ", __FUNCTION__);
    return 0;
}

#define ARRY_SIZE 2000

int (*bubble_sort[])(int *sort_butt, int size, bool ascend) = {
    gap_bubble_sort,
    fast_bubble_sort,
    classic_bubble_sort,
    shaker_bubble_sort,
    NULL,
};

int main(int argc, char *argv[])
{
    int arry_size = atoi(argv[1]);
    int *sort_arry = malloc(sizeof(int)*arry_size);
    int i = 0, fun_idx = 0;

    TIME_INIT_START_END(T);

    for (fun_idx = 0; ; fun_idx++) {
        if (bubble_sort[fun_idx] == NULL)
            break;

        for (i = 0; i < arry_size; i++)
            sort_arry[i] = rand();

        TIME_GET_START(T);
        bubble_sort[fun_idx](sort_arry, arry_size, 1);
        TIME_GET_END(T);
        nb_logd("Use time %f.\n", TIME_PRINT_DIFF(T));

        nb_logd("[");
        for (i = 1; i < arry_size; i++) {
            if (sort_arry[i] < sort_arry[i-1])
                nb_logd("%d ", sort_arry[i]-sort_arry[i-1]);
        }
        nb_logd("]\n");
    }

    free(sort_arry);
    return 0;
}

