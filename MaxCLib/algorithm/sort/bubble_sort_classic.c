#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

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
    int gap = size/1.3;

    for (i = 1; i < size; i++) {
        end_idx = size-gap;
        for (idx = 0; idx < end_idx; idx++) {
            if (sort_butt[idx] > sort_butt[idx+gap]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx+gap];
                sort_butt[idx+gap] = swap_tmp;
            }
        }

        if (gap == 1)
            return 0;

        gap--;
    }

    return 0;
}

int fast_bubble_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int end_idx = size-1;
    int swap_tmp = 0;
    bool swap_flag = 0;

    for (i = 1; i < size; i++) {
        for (idx = 0; idx < end_idx; idx++) {
            if (sort_butt[idx] > sort_butt[idx+1]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx+1];
                sort_butt[idx+1] = swap_tmp;
                swap_flag = 1;
            }
        }
        if (!swap_flag)
            return 0;

        swap_flag = 0;
        end_idx--;
    }

    return 0;
}

int classic_bubble_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int end_idx = size-1;
    int swap_tmp = 0;

    for (i = 1; i < size; i++) {
        for (idx = 0; idx < end_idx; idx++) {
            if (sort_butt[idx] > sort_butt[idx+1]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx+1];
                sort_butt[idx+1] = swap_tmp;
            }
        }
    }

    return 0;
}

int shaker_bubble_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int end_idx = size-1, start_idx = 0;
    int swap_tmp = 0;
    bool swap_flag = 0;

    while (start_idx < end_idx) {
        for (idx = start_idx; idx < end_idx; idx++) {
            if (sort_butt[idx] > sort_butt[idx+1]) {
                swap_tmp = sort_butt[idx];
                sort_butt[idx] = sort_butt[idx+1];
                sort_butt[idx+1] = swap_tmp;
                swap_flag = 1;
            }
        }
        if (!swap_flag)
            return 0;
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

        if (!swap_flag)
            return 0;
        swap_flag = 0;
        start_idx++;
    }

    return 0;

}

#define ARRY_SIZE 2000

int main(int argc, char *argv[])
{
    int arry_size = atoi(argv[1]);
    int *sort_arry = malloc(sizeof(int)*arry_size);
    int i = 0;
#if 1
    printf("[");
    for (i = 0; i < arry_size; i++) {
        sort_arry[i] = rand();
//        printf("%d ", sort_arry[i]);
    }
    printf("]\n");
#endif

    TIME_INIT_START_END(T);
    TIME_GET_START(T);
    gap_bubble_sort(sort_arry, arry_size, 1);
    TIME_GET_END(T);
    printf("Use [gap_bubble_sort] time %f.\n", TIME_PRINT_DIFF(T));

    for (i = 0; i < arry_size; i++)
        sort_arry[i] = rand();

    TIME_GET_START(T);
    shaker_bubble_sort(sort_arry, arry_size, 1);
    TIME_GET_END(T);
    printf("Use [shaker_bubble_sort] time %f.\n", TIME_PRINT_DIFF(T));

    for (i = 0; i < arry_size; i++)
        sort_arry[i] = rand();

    TIME_GET_START(T);
    classic_bubble_sort(sort_arry, arry_size, 1);
    TIME_GET_END(T);
    printf("Use [classic_bubble_sort] time %f.\n", TIME_PRINT_DIFF(T));

    for (i = 0; i < arry_size; i++)
        sort_arry[i] = rand();

    TIME_GET_START(T);
    fast_bubble_sort(sort_arry, arry_size, 1);
    TIME_GET_END(T);
    printf("Use [fast_bubble_sort] time %f.\n", TIME_PRINT_DIFF(T));

#if 1
    printf("[");
    for (i = 1; i < arry_size; i++) {
        if (sort_arry[i] < sort_arry[i-1])
            printf("%d ", sort_arry[i]-sort_arry[i-1]);
    }
    printf("]\n");
#endif
    return 0;
}

