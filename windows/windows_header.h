#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>

int from_string_to_int(char * arg);
int * divided_properly(int number_of_child, int number_of_numbers, int * to_much_child);
void create_n_input_output_files(int * arr, int * distribution, int number_of_child);
void print_data_in_the_file(char * filename, int * objects_for_write, int number_of_objects);
char * generate_input_filename(int i);
char * generate_output_filename(int i);
int get_total_sum(int number_of_child);
int sum_numbers(int arr_len, int * arr);
char * concat_string(char * args[4]);
