#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <errno.h>
#include <pthread.h>

struct arguments{
    int * arr;
    int len_of_arr;
};


int * divided_properly(int number_of_child, int number_of_numbers);
int from_string_to_int(char * arg);
void * routine(void * data_struct);
int wait_for_threads(int number_of_threads, pthread_t * arr_of_threads);
int create_threads_and_pass_args(int number_of_threads, pthread_t * arr_of_threads, struct arguments * original_args);
void init_arguments(int number_of_threads, int * distribution, struct arguments * original_args, int * arr);
void free_dynamic(int * distribution, int * arr, struct arguments * original_args, pthread_t * arr_of_threads);
void work_with_thread(int number_of_threads, int * arr, int arr_len);
void check_and_reduce(int * number_of_threads, int arr_len);
int get_number_of_input(FILE * fp);
int check_argc(int argc);
int lovit_cal(FILE * fp, int number_of_threads);
int check_data(int data_cnt);
void body(FILE * fp, int number_of_threads, int data_cnt);