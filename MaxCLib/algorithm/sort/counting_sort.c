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

void classic_counting_sort(int *sort_butt, int size)
{
    int i = 1;
    int radix_butt[SORT_RANGE];
    int *radix_arry = malloc(sizeof(int)*size);

    /* Should check the maxmum and minimum value of the sorted array. */
    memcpy(radix_arry, sort_butt, sizeof(int)*size);

    memset(&radix_butt[0], 0, sizeof(radix_butt));
    for (i = 0 ; i < size; i++) {
        radix_butt[radix_arry[i]]++;
    }

    int last_tmp = 0;
    i = 0;
    while (radix_butt[i] == 0 && i < 10)
        i++;

    last_tmp = radix_butt[i];
    i++;

    for (; i < SORT_RANGE; i++) {
        if (radix_butt[i] > 0) {
            radix_butt[i] = radix_butt[i] + last_tmp;
            last_tmp = radix_butt[i];
        }
    }

    int radix_idx = 0;
    for (i = size-1; i >= 0; i--) {
        radix_idx = radix_arry[i];
        radix_butt[radix_idx]--;
        sort_butt[radix_butt[radix_idx]] = radix_arry[i];
    }

    free(radix_arry);
    printf("[%s] ", __FUNCTION__);
}

void (*counting_sort[])(int *sort_butt, int sizes) = {
    classic_counting_sort,
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
        if (counting_sort[fun_idx] == NULL)
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
        counting_sort[fun_idx](sort_arry, arry_size);
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




