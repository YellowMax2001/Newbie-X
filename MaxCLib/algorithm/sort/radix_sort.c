#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#include <math.h>

#define TIME_INIT_START_END(x) struct timeval start_##x; \
                       struct timeval end_##x; \
                       float DiffTime_##x;
#define TIME_GET_START(x) (gettimeofday(&start_##x, NULL))
#define TIME_GET_END(x) (gettimeofday(&end_##x, NULL)); \
                (DiffTime_##x = \
                (((end_##x.tv_sec + end_##x.tv_usec * 1e-6) - \
                (start_##x.tv_sec + start_##x.tv_usec * 1e-6)) * 1000))
#define TIME_PRINT_DIFF(x) (DiffTime_##x)

#define SORT_RANGE 100000000000

static inline int binsearch(int *sort_butt, int sel_pos, int len_idx)
{
    int left_pos = 0, right_pos = sel_pos;
    int search_idx = 0, find_pos = sel_pos;

    while (left_pos <= right_pos) {
        search_idx = left_pos + (right_pos-left_pos)/2;
        if (sort_butt[search_idx]/(int)pow(10,len_idx)%10
                > sort_butt[sel_pos]/(int)pow(10,len_idx)%10) {
            find_pos = search_idx;
            right_pos = search_idx-1;
        } else
            left_pos = search_idx+1;
    }

    return find_pos;
}

/* I will insert @sort_butt[sel_pos] into @sort_butt[insert_pos-1] */
static inline void insert_elem(int *sort_butt, int sel_pos, int insert_pos)
{
    int insert_tmp = sort_butt[sel_pos];

    int i = 0;

    for (i = sel_pos-1; i >= insert_pos; i--)
        sort_butt[i+1] = sort_butt[i];

    sort_butt[insert_pos] = insert_tmp;
}

int binsearch_insertion_sort(int *sort_butt, int size, int len_idx)
{
    int i = 0, idx = 0;

    for (i = 1; i < size; i++)
        insert_elem(sort_butt, i, binsearch(sort_butt, i, len_idx));

    return 0;
}

void classic_counting_sort(int *sort_butt, int *radix_arry, int size, int len_idx)
{
    int i = 1;
    int radix_butt[10];

    memset(&radix_butt[0], 0, sizeof(radix_butt));
    for (i = 0 ; i < size; i++) {
        radix_butt[(radix_arry[i]/(int)pow(10,len_idx))%10]++;
    }

    int last_tmp = 0;
    i = 0;
    while (radix_butt[i] == 0 && i < 10)
        i++;

    last_tmp = radix_butt[i];
    i++;

    for (; i < 10; i++) {
        if (radix_butt[i] > 0) {
            radix_butt[i] = radix_butt[i] + last_tmp;
            last_tmp = radix_butt[i];
//            printf("radix_butt[%d]=[%d].\n", i, radix_butt[i]);
        }
    }

    int radix_idx = 0;
    for (i = size-1; i >= 0; i--) {
        radix_idx = radix_arry[i]/(int)pow(10,len_idx)%10;
        radix_butt[radix_idx]--;
        sort_butt[radix_butt[radix_idx]] = radix_arry[i];
    }
}

#define MAX_LEN 11

void classic_radix_sort(int *sort_butt, int size)
{
    int *radix_arry = malloc(sizeof(int)*size);
    int len_idx = 0;

    memcpy(radix_arry, sort_butt, sizeof(int)*size);

    for (len_idx = 0; len_idx < MAX_LEN; len_idx++) {
        if (len_idx%2 == 0)
            classic_counting_sort(sort_butt, radix_arry, size, len_idx);
        else
            classic_counting_sort(radix_arry, sort_butt, size, len_idx);
    }

    free(radix_arry);
    printf("[%s] ", __FUNCTION__);
}

void (*radix_sort[])(int *sort_butt, int sizes) = {
    classic_radix_sort,
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
        if (radix_sort[fun_idx] == NULL)
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
        radix_sort[fun_idx](sort_arry, arry_size);
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



