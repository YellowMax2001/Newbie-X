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

static void __merge(int *dst_butt, int *merge_butt, int left_idx, int mid_idx, int right_idx)
{
    int i = left_idx, j = mid_idx+1;
    int sort_tmp_cnt = i;

    j = j > right_idx ? right_idx : j;

    while (i <= mid_idx && j <= right_idx) {
        if (merge_butt[i] <= merge_butt[j])
            dst_butt[sort_tmp_cnt++] = merge_butt[i++];
        else
            dst_butt[sort_tmp_cnt++] = merge_butt[j++];
    }

    if (i > mid_idx) {
        for (i = j; i <= right_idx; i++)
            dst_butt[sort_tmp_cnt++] = merge_butt[i];
    } else {
        for (j = i; j <= mid_idx; j++)
            dst_butt[sort_tmp_cnt++] = merge_butt[j];
    }
}

static inline void __merge_sort(int *sort_butt, int *sort_tmp, int left_idx, int right_idx)
{
    int mid_idx = left_idx + (right_idx - left_idx) / 2;

    if (left_idx + 1 < right_idx) {
        __merge_sort(sort_butt, sort_tmp, left_idx, mid_idx);
        __merge_sort(sort_butt, sort_tmp, mid_idx+1, right_idx);
    }

    __merge(sort_tmp, sort_butt, left_idx, mid_idx, right_idx);

    int i = 0;
    for (i = left_idx; i <= right_idx; i++) {
        sort_butt[i] = sort_tmp[i];
    }
}

void classic_merge_sort(int *sort_butt, int size)
{
    int *sort_tmp = malloc(size*sizeof(int));

    __merge_sort(sort_butt, sort_tmp, 0, size-1);

    free(sort_tmp);

    printf("[%s] ", __FUNCTION__);
}

static void shell_insertion_sort(int *sort_butt, int size)
{
    int i = 0, j = 0, idx = 0, gap_idx = 0;
    int gap[] = {701, 301, 132, 57, 23, 10, 4, 1, -1};

    int insert_tmp = 0;
    int gap_val = gap[gap_idx++];
    while (gap_val != -1) {
        for (i = gap_val; i < size; i++) {
            insert_tmp = sort_butt[i];
            for (idx = i; idx >= gap_val && sort_butt[idx-gap_val] > insert_tmp; idx-=gap_val)
                sort_butt[idx] = sort_butt[idx-gap_val];

            sort_butt[idx] = insert_tmp;
        }

        gap_val = gap[gap_idx++];
    }
}

static inline void __perform_merge_sort(int *sort_butt, int *tmp_butt, int left_idx, int right_idx)
{
    int mid_idx = left_idx + (right_idx - left_idx) / 2;

    if (right_idx - left_idx > 1) {
        __merge_sort(tmp_butt, sort_butt, left_idx, mid_idx);
        __merge_sort(tmp_butt, sort_butt, mid_idx+1, right_idx);
    }

    __merge(sort_butt, tmp_butt, left_idx, mid_idx, right_idx);
}

void perform_merge_sort(int *sort_butt, int size)
{
    int *sort_tmp = malloc(size*sizeof(int));

    memcpy(sort_tmp, sort_butt, sizeof(int)*size);

    __perform_merge_sort(sort_butt, sort_tmp, 0, size-1);

    free(sort_tmp);

    printf("[%s] ", __FUNCTION__);
}


void non_recursive_merge_sort(int *sort_butt, int size)
{
    int *sort_tmp = malloc(size*sizeof(int));

    memcpy(sort_tmp, sort_butt, sizeof(int)*size);

    int gap = 1;
    int left_idx = 0, right_idx = 0, mid_idx = 0;
    bool exchange_flag = 0;
    bool exit_flag = 0;

    while (1) {
        left_idx = 0;
        right_idx = gap*2 - 1;
        mid_idx = left_idx + (right_idx - left_idx) / 2;

        if (right_idx >= size - 1)
            exit_flag = 1;
        right_idx = right_idx >= size ? size - 1 : right_idx;

        if (!exchange_flag) {
            while (mid_idx < size-1) {
                __merge(sort_butt, sort_tmp, left_idx, mid_idx, right_idx);
                left_idx += gap*2;
                right_idx = left_idx + gap*2 - 1;
                mid_idx = left_idx + (right_idx - left_idx) / 2;
                right_idx = right_idx >= size ? size - 1 : right_idx;
            }
            if (left_idx < size)
                memcpy(&sort_butt[left_idx], &sort_tmp[left_idx], sizeof(sort_butt[0])*(size-left_idx));
        } else {
            while (mid_idx < size-1) {
                __merge(sort_tmp, sort_butt, left_idx, mid_idx, right_idx);
                left_idx += gap*2;
                right_idx = left_idx + gap*2 - 1;
                mid_idx = left_idx + (right_idx - left_idx) / 2;
                right_idx = right_idx >= size ? size - 1 : right_idx;
            }
            if (left_idx < size)
                memcpy(&sort_tmp[left_idx], &sort_butt[left_idx], sizeof(sort_tmp[0])*(size-left_idx));
        }

        exchange_flag = !exchange_flag;

        gap *= 2;

        if (exit_flag)
            break;
    }

    if (exchange_flag) {
        printf("Bingo, that is good.\n");
    } else {
        printf("Bad news, should do more copy.\n");
        memcpy(sort_butt, sort_tmp, sizeof(int)*size);
    }

    free(sort_tmp);

    printf("[%s] ", __FUNCTION__);
}

void (*merge_sort[])(int *sort_butt, int sizes) = {
    classic_merge_sort,
    perform_merge_sort,
    non_recursive_merge_sort,
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



