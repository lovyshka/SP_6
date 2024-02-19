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
    int * numbers;
    int len_of_arr;
};


int * divided_properly(int number_of_child, int number_of_numbers, int * to_much_child);
char * generate_input_filename(int i);
char * generate_output_filename(int i);
void create_n_input_output_files(int * arr, int * distribution, int number_of_child) ;
void print_data_in_the_file(char * filename, int * objects_for_write, int number_of_objects);
int get_total_sum(int number_of_child);
int from_string_to_int(char * arg);
int sum_numbers(int arr_len, int * arr);
void * routine(void * data_struct);
