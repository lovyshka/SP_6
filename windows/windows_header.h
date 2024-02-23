#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <tchar.h>
#include <strsafe.h>

struct arguments{
    int * arr;
    int len_of_arr;
};

int from_string_to_int(char * arg);
int * divided_properly(int number_of_child, int number_of_numbers);
int get_number_of_input(FILE * fp);
void check_and_reduce(int * number_of_threads, int arr_len);
void init_arguments(int number_of_threads, int * distribution, struct arguments * original_args, int * arr);
void create_threads_and_pass_args(int number_of_threads, HANDLE * arr_of_threads, struct arguments * original_args, DWORD * thread_id_arr);
void work_with_thread(int number_of_threads, int * arr, int arr_len);