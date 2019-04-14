#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

#define SORT_RANGE 100

void classic_heap_sort(int *sort_butt, int size)
{
    int heap_cnt = size;
    int i = 0;

    /* Parent: i, left_cld: 2*i+1, right_cld: 2*i+2 */
    int non_leaf_node = 0;
    int max_val_pos = 0;
    int swap_tmp = 0;
    for (i = 0; i < size-1; i++) {
        /* Find the first unsorted non-leaf node, and heap the list. */
        non_leaf_node = heap_cnt/2-1;
        while (non_leaf_node >= 0) {
            max_val_pos = non_leaf_node;
            if (sort_butt[max_val_pos] < sort_butt[2*non_leaf_node+1])
                max_val_pos = 2*non_leaf_node+1;
            if (2*non_leaf_node+2 < heap_cnt && sort_butt[max_val_pos] < sort_butt[2*non_leaf_node+2])
                max_val_pos = 2*non_leaf_node+2;

            if (max_val_pos == non_leaf_node) {
                non_leaf_node--;
                continue;
            }

            swap_tmp = sort_butt[max_val_pos];
            sort_butt[max_val_pos] = sort_butt[non_leaf_node];
            sort_butt[non_leaf_node] = swap_tmp;
            non_leaf_node--;
        }

        heap_cnt--;
        swap_tmp = sort_butt[0];
        sort_butt[0] = sort_butt[heap_cnt];
        sort_butt[heap_cnt] = swap_tmp;
    }

    printf("[%s] ", __FUNCTION__);
}

void (*heap_sort[])(int *sort_butt, int sizes) = {
    classic_heap_sort,
    NULL,
};

//#define DEBUG_SORT_ARRY

int main(int argc, char *argv[])
{
    int arry_size = atoi(argv[1]);
    int *sort_arry = malloc(sizeof(int)*arry_size);
    int i = 0, fun_idx = 0;

    TIME_INIT_START_END(T);

    for (fun_idx = 0; ; fun_idx++) {
        if (heap_sort[fun_idx] == NULL)
            break;

#ifdef DEBUG_SORT_ARRY
        for (i = 0; i < arry_size; i++) {
            sort_arry[i] = rand()%SORT_RANGE;
            printf("%d ", sort_arry[i]);
        }
        printf("\n");
#else
        for (i = 0; i < arry_size; i++)
            sort_arry[i] = rand()%SORT_RANGE;
#endif

        TIME_GET_START(T);
        heap_sort[fun_idx](sort_arry, arry_size);
        TIME_GET_END(T);
        printf("Use time %f.\n", TIME_PRINT_DIFF(T));

        printf("[");
#ifdef DEBUG_SORT_ARRY
        for (i = 0; i < arry_size; i++)
            printf("%d ", sort_arry[i]);
#else
        for (i = 1; i < arry_size; i++) {
            if (sort_arry[i] < sort_arry[i-1])
                printf("%d ", sort_arry[i]-sort_arry[i-1]);
        }
#endif
        printf("]\n");
    }

    free(sort_arry);
    return 0;
}




