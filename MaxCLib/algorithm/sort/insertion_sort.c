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

/* I will insert @sort_butt[sel_pos] into @sort_butt[insert_pos-1] */
static inline void insert_elem(int *sort_butt, int sel_pos, int insert_pos)
{
    int insert_tmp = sort_butt[sel_pos];

    int i = 0;

    for (i = sel_pos-1; i >= insert_pos; i--)
        sort_butt[i+1] = sort_butt[i];

    sort_butt[insert_pos] = insert_tmp;
}

int classic_insertion_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;

    for (i = 1; i < size; i++) {
        for (idx = 0; idx < i; idx++) {
            if (sort_butt[idx] > sort_butt[i])
                insert_elem(sort_butt, i, idx);
        }
    }

    printf("[%s] ", __FUNCTION__);
    return 0;
}

int fast_classic_insertion_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;
    int insert_tmp = 0;
    for (i = 1; i < size; i++) {
        insert_tmp = sort_butt[i];
        for (idx = i; idx >= 1 && sort_butt[idx-1] > insert_tmp; idx--)
            sort_butt[idx] = sort_butt[idx-1];

        sort_butt[idx] = insert_tmp;
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

int binsearch_insertion_sort(int *sort_butt, int size, bool ascend)
{
    int i = 0, idx = 0;

    for (i = 1; i < size; i++)
        insert_elem(sort_butt, i, binsearch(sort_butt, i));

    printf("[%s] ", __FUNCTION__);
    return 0;
}


/* I will insert @sort_butt[sel_pos] into @sort_butt[insert_pos-gap] */
static inline void insert_elem_with_gap(int *sort_butt, int sel_pos, int insert_pos, int gap)
{
    int insert_tmp = sort_butt[sel_pos];

    int i = 0;

    for (i = sel_pos-gap; i >= insert_pos; i-=gap)
        sort_butt[i+gap] = sort_butt[i];

    sort_butt[insert_pos] = insert_tmp;
}

int shell_insertion_sort(int *sort_butt, int size, bool ascend)
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

    printf("[%s] ", __FUNCTION__);
    return 0;
}

#define ARRY_SIZE 2000

int (*insertion_sort[])(int *sort_butt, int size, bool ascend) = {
    classic_insertion_sort,
    fast_classic_insertion_sort,
    binsearch_insertion_sort,
    shell_insertion_sort,
    NULL,
};

int main(int argc, char *argv[])
{
    int arry_size = atoi(argv[1]);
    int *sort_arry = malloc(sizeof(int)*arry_size);
    int i = 0, fun_idx = 0;

    TIME_INIT_START_END(T);

    for (fun_idx = 0; ; fun_idx++) {
        if (insertion_sort[fun_idx] == NULL)
            break;

        for (i = 0; i < arry_size; i++)
            sort_arry[i] = rand();

        TIME_GET_START(T);
        insertion_sort[fun_idx](sort_arry, arry_size, 1);
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


