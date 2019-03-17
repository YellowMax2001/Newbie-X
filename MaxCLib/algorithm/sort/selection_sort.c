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

int classic_selection_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int min_idx_tmp = 0;
    int swap_tmp = 0;

    for (i = 0; i < size; i++) {
        min_idx_tmp = i;
        for (idx = i; idx < size; idx++) {
            if (sort_butt[idx] < sort_butt[min_idx_tmp])
                min_idx_tmp = idx;
        }

        if (i != min_idx_tmp) {
            swap_tmp = sort_butt[i];
            sort_butt[i] = sort_butt[min_idx_tmp];
            sort_butt[min_idx_tmp] = swap_tmp;
        }
    }

    printf("[%s] ", __FUNCTION__);
    return 0;
}

int fast_classic_selection_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int min_idx_tmp = 0, max_idx_tmp = 0;
    int swap_tmp = 0;
    int right_idx = size;

    for (i = 0; i < right_idx; i++) {
        min_idx_tmp = i;
        max_idx_tmp = right_idx-1;
        for (idx = i; idx < right_idx; idx++) {
            if (sort_butt[idx] < sort_butt[min_idx_tmp])
                min_idx_tmp = idx;
            else if (sort_butt[idx] > sort_butt[max_idx_tmp])
                max_idx_tmp = idx;
        }

        if (min_idx_tmp == right_idx-1) {
            swap_tmp = sort_butt[min_idx_tmp];
            sort_butt[min_idx_tmp] = sort_butt[i];
            sort_butt[i] = swap_tmp;

            if (max_idx_tmp != i) {
                swap_tmp = sort_butt[max_idx_tmp];
                sort_butt[max_idx_tmp] = sort_butt[right_idx-1];
                sort_butt[right_idx-1] = swap_tmp;
            }
        } else {
            swap_tmp = sort_butt[max_idx_tmp];
            sort_butt[max_idx_tmp] = sort_butt[right_idx-1];
            sort_butt[right_idx-1] = swap_tmp;

            swap_tmp = sort_butt[min_idx_tmp];
            sort_butt[min_idx_tmp] = sort_butt[i];
            sort_butt[i] = swap_tmp;
        }

        right_idx--;
    }

    printf("[%s] ", __FUNCTION__);
    return 0;
}

int gap_selection_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int min_idx_tmp = 0;
    int swap_tmp = 0;

    for (i = 0; i < size; i++) {
        min_idx_tmp = i;
        for (idx = i; idx < size; idx++) {
            if (sort_butt[idx] < sort_butt[min_idx_tmp])
                min_idx_tmp = idx;
        }
    
        if (i != min_idx_tmp) {
            swap_tmp = sort_butt[i];
            sort_butt[i] = sort_butt[min_idx_tmp];
            sort_butt[min_idx_tmp] = swap_tmp;
        }
    }

    printf("[%s] ", __FUNCTION__);
    return 0;
}

static inline int binsearch(int *sort_butt, int sel_pos)
{
    int left_pos = 0, right_pos = sel_pos;
    int search_idx = 0, find_pos = sel_pos;

    while (left_pos <= right_pos) {
        search_idx = left_pos + (right_pos-left_pos)/2;
        if (sort_butt[search_idx] > sort_butt[sel_pos]) {
            find_pos = search_idx;
            right_pos = search_idx-1;
        } else
            left_pos = search_idx+1;
    }

    return find_pos;
}

int (*selection_sort[])(int *sort_butt, int size, bool ascend) = {
    classic_selection_sort,
    fast_classic_selection_sort,
    NULL,
};

int main(int argc, char *argv[])
{
    int arry_size = atoi(argv[1]);
    int *sort_arry = malloc(sizeof(int)*arry_size);
    int i = 0, fun_idx = 0;

    TIME_INIT_START_END(T);

    for (fun_idx = 0; ; fun_idx++) {
        if (selection_sort[fun_idx] == NULL)
            break;

        for (i = 0; i < arry_size; i++)
            sort_arry[i] = rand();

        TIME_GET_START(T);
        selection_sort[fun_idx](sort_arry, arry_size, 1);
        TIME_GET_END(T);
        printf("Use time %f.\n", TIME_PRINT_DIFF(T));

        printf("[");
        for (i = 1; i < arry_size; i++) {
            if (sort_arry[i] < sort_arry[i-1])
                printf("%d ", sort_arry[i]-sort_arry[i-1]);
        }
        printf("]\n");
    }

    free(sort_arry);
    return 0;
}



