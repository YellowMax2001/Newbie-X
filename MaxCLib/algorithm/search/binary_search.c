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

void shell_insertion_sort(int *sort_butt, int size)
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

static int binsearch_find_first_equal(int *sort_array, int size, int find_val)
{
    int left_pos = 0, right_pos = size - 1;
    int cur_pos = 0, find_pos = -1;

    while (left_pos <= right_pos) {
        cur_pos = left_pos + ((right_pos-left_pos)>>1);
        if (sort_array[cur_pos] >= find_val) {
            if (sort_array[cur_pos] == find_val)
                find_pos = cur_pos;
            right_pos = cur_pos - 1;
        } else
            left_pos = cur_pos + 1;
    }

    printf("[%s] ", __FUNCTION__);
    return find_pos;
}

static int binsearch_find_last_equal(int *sort_array, int size, int find_val)
{
    int left_pos = 0, right_pos = size - 1;
    int cur_pos = 0, find_pos = -1;

    while (left_pos <= right_pos) {
        cur_pos = left_pos + ((right_pos-left_pos)>>1);
        if (sort_array[cur_pos] > find_val) {
            right_pos = cur_pos - 1;
        } else {
            if (sort_array[cur_pos] == find_val)
                find_pos = cur_pos;
            left_pos = cur_pos + 1;
        }
    }

    printf("[%s] ", __FUNCTION__);
    return find_pos;
}

static int binsearch_find_first_large(int *sort_array, int size, int find_val)
{
    int left_pos = 0, right_pos = size - 1;
    int cur_pos = 0, find_pos = -1;

    while (left_pos <= right_pos) {
        cur_pos = left_pos + ((right_pos-left_pos)>>1);
        if (sort_array[cur_pos] > find_val) {
            find_pos = cur_pos;
            right_pos = cur_pos - 1;
        } else
            left_pos = cur_pos + 1;
    }

    printf("[%s] ", __FUNCTION__);
    return find_pos;
}

static int binsearch_find_last_small(int *sort_array, int size, int find_val)
{
    int left_pos = 0, right_pos = size - 1;
    int cur_pos = 0, find_pos = -1;

    while (left_pos <= right_pos) {
        cur_pos = left_pos + ((right_pos-left_pos)>>1);
        if (sort_array[cur_pos] >= find_val) {
            right_pos = cur_pos - 1;
        } else {
            find_pos = cur_pos;
            left_pos = cur_pos + 1;
        }
    }

    printf("[%s] ", __FUNCTION__);
    return find_pos;
}

static int binsearch_find_with_range(int *sort_array, int size, int find_val)
{
    int left_pos = 0, right_pos = size - 1;
    int cur_pos = 0, find_l_pos = -1, find_r_pos = -1;

    while (left_pos <= right_pos) {
        cur_pos = left_pos + ((right_pos-left_pos)>>1);
        if (sort_array[cur_pos] >= find_val-2) {
            find_l_pos = cur_pos;
            right_pos = cur_pos - 1;
        } else {
            left_pos = cur_pos + 1;
        }
    }
    printf("[%d][%d], left[%d],val[%d]\n",
        find_l_pos-1, sort_array[find_l_pos-1], find_l_pos, sort_array[find_l_pos]);

    left_pos = 0, right_pos = size - 1;
    while (left_pos <= right_pos) {
        cur_pos = left_pos + ((right_pos-left_pos)>>1);
        if (sort_array[cur_pos] <= find_val+2) {
            find_r_pos = cur_pos;
            left_pos = cur_pos + 1;
        } else {
            right_pos = cur_pos - 1;
        }
    }

    printf("right[%d],val[%d] [%d][%d]\n",
        find_r_pos, sort_array[find_r_pos], find_r_pos+1, sort_array[find_r_pos+1]);

    printf("[%s] ", __FUNCTION__);
    return find_l_pos;
}

#define SORT_RANGE 100

int (*bin_search[])(int *sort_butt, int size, int find_val) = {
    binsearch_find_first_equal,
    binsearch_find_last_equal,
    binsearch_find_first_large,
    binsearch_find_last_small,
    binsearch_find_with_range,
    NULL,
};


int main(int argc, char *argv[])
{
    int arry_size = atoi(argv[1]);
    int *sort_arry = malloc(sizeof(int)*arry_size);
    int i = 0, find_idx = 0, find_val = rand()%SORT_RANGE;
    int fun_idx = 0;

    TIME_INIT_START_END(T);

    for (i = 0; i < arry_size; i++)
        sort_arry[i] = rand()%SORT_RANGE;

    shell_insertion_sort(sort_arry, arry_size);

    for (fun_idx = 0; ; fun_idx++) {
        if (bin_search[fun_idx] == NULL)
            break;

        TIME_GET_START(T);
        find_idx = bin_search[fun_idx](sort_arry, arry_size, find_val);
        TIME_GET_END(T);
        printf("Use time %f.\n", TIME_PRINT_DIFF(T));

        if (find_idx >= 0) {
            int left_prt_idx  = find_idx - 2 > 0 ? find_idx - 2 : 0;
            int right_prt_idx = find_idx + 2 < arry_size ? find_idx + 2 : arry_size - 1;

            printf("find[%d], pos[%d]\n", find_val, find_idx);
            for (i = left_prt_idx; i < right_prt_idx; i++) {
                printf("idx[%d], val[%d]\n", i, sort_arry[i]);
            }
        } else {
            printf("Not find[%d], pos[%d]\n", find_val, find_idx);
        }
    }

    free(sort_arry);
    return 0;
}





