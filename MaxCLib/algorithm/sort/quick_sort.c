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

static inline int pick_pivot_mid(int *sort_butt, int sort_left, int sort_right)
{
    return sort_left+(sort_right-sort_left)/2;
}

static inline int pick_pivot_left(int *sort_butt, int sort_left, int sort_right)
{
    return sort_left;
}


static inline int pick_pivot_right(int *sort_butt, int sort_left, int sort_right)
{
    return sort_right;
}

static inline int pick_pivot_three_mid(int *sort_butt, int sort_left, int sort_right)
{
    int mid_idx = sort_left+(sort_right-sort_left)/2;

    if (sort_left + 500 < sort_right) {
        int min_idx = sort_left, max_idx = sort_right;
        int sum_idx = mid_idx + min_idx + max_idx;

        if (sort_butt[min_idx] > sort_butt[mid_idx])
            min_idx = mid_idx;
        if (sort_butt[min_idx] > sort_butt[sort_right])
            min_idx = max_idx;

        if (sort_butt[max_idx] < sort_butt[sort_left])
            max_idx = sort_left;
        if (sort_butt[max_idx] < sort_butt[mid_idx])
            max_idx = mid_idx;

        return sum_idx - min_idx - max_idx;
    } else
        return mid_idx;
}

typedef int (*pick_pivot_fun)(int *sort_butt, int sort_left, int sort_right);

pick_pivot_fun pick_pivot = pick_pivot_three_mid;

void __quick_sort_recursive(int *sort_butt, int left, int right)
{
    int i = left, j = right;
    int pivot_idx = pick_pivot(sort_butt, left, right);
    int pivot_tmp = sort_butt[pivot_idx];
    int swap_tmp = 0;

    swap_tmp = sort_butt[left];
    sort_butt[left] = sort_butt[pivot_idx];
    sort_butt[pivot_idx] = swap_tmp;
    pivot_idx = 0;

    if (left < right) {
        //sort
        while (i < j) {
            while (j > i && sort_butt[j] >= pivot_tmp) j--;
            while (j > i && sort_butt[i] <= pivot_tmp) i++;

            swap_tmp = sort_butt[i];
            sort_butt[i] = sort_butt[j];
            sort_butt[j] = swap_tmp;
        }
        sort_butt[left] = sort_butt[i];
        sort_butt[i] = pivot_tmp;

        //sort left
        __quick_sort_recursive(sort_butt, left, i-1);
        //sort right
        __quick_sort_recursive(sort_butt, i+1, right);
    }
}

void classic_quick_sort(int *sort_butt, int size)
{
    __quick_sort_recursive(sort_butt, 0, size-1);

    printf("[%s] ", __FUNCTION__);
}

void nonrecursive_quick_sort(int *sort_butt, int size)
{
    int *strack_butt = malloc(sizeof(int)*size);
    int strack_cnt = 0;

    int left = 0, right = size-1;

loop:
    while (left < right) {
        int i = left, j = right;
        int pivot_idx = pick_pivot(sort_butt, left, right);
        int pivot_tmp = sort_butt[pivot_idx];
        int swap_tmp = 0;

        swap_tmp = sort_butt[left];
        sort_butt[left] = sort_butt[pivot_idx];
        sort_butt[pivot_idx] = swap_tmp;
        pivot_idx = 0;

        //sort
        while (i < j) {
            while (j > i && sort_butt[j] >= pivot_tmp) j--;
            while (j > i && sort_butt[i] <= pivot_tmp) i++;
        
            swap_tmp = sort_butt[i];
            sort_butt[i] = sort_butt[j];
            sort_butt[j] = swap_tmp;
        }
        sort_butt[left] = sort_butt[i];
        sort_butt[i] = pivot_tmp;

        strack_butt[strack_cnt++] = i+1;
        strack_butt[strack_cnt++] = right;

        left = left;
        right = i;
    }

    if (strack_cnt) {
        right = strack_butt[--strack_cnt];
        left   = strack_butt[--strack_cnt];
        goto loop;
    }

    free(strack_butt);
    printf("[%s] ", __FUNCTION__);
}

void (*merge_sort[])(int *sort_butt, int sizes) = {
    classic_quick_sort,
    nonrecursive_quick_sort,
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
        if (merge_sort[fun_idx] == NULL)
            break;

#ifdef DEBUG_SORT_ARRY
        for (i = 0; i < arry_size; i++) {
            sort_arry[i] = rand();
            printf("%d ", sort_arry[i]);
        }
        printf("\n");
#else
        for (i = 0; i < arry_size; i++)
            sort_arry[i] = rand();
#endif

        TIME_GET_START(T);
        merge_sort[fun_idx](sort_arry, arry_size);
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

