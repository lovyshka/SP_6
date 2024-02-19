#include "linux_header.h"

int main(int argc, char * argv[]) {
           
    char * input_fn = argv[1];
    char * output_fn = argv[2];
    int i = 0, tmp, arr_len = 0, sum = 0;
    int * arr = (int *) malloc(sizeof(int));
    FILE * in_fp = fopen(input_fn, "r");
    FILE * out_fp = fopen(output_fn, "r+");

    while (fscanf(in_fp, "%d", &tmp) == 1)
    {
        arr = (int *)realloc(arr, sizeof(int) *(i + 1));
        arr[i] = tmp;
        i++;
    }

    fprintf(out_fp, "%d", sum_numbers(i, arr));
    // printf("pid = %d\n", getpid());

    free(arr);
    fclose(in_fp);
    fclose(out_fp);
    // sleep(30000);
    return 0;
}


int sum_numbers(int arr_len, int * arr){
    int sum = 0;
    for (int i = 0; i < arr_len; i++) {
        if (arr[i] % 2 == 0) sum += arr[i];
    }  
    return sum;
}
