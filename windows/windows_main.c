#include "windows_header.h"

int main(int argc, char * argv[]){
    if (argc == 3){
        int number_of_child = from_string_to_int(argv[2]);
        FILE * fp = fopen(argv[1], "r");
        if (number_of_child > 0 && fp){
            int tmp, arr_len; //для fscanf
            int i = 0, flag_child = 0; //для подсчета длины
            int * arr = (int *) malloc(sizeof(int)); 
            int * distribution; //массив с числами из файла и распределением 
            while (fscanf(fp, "%d", &tmp) == 1)
            {
                arr = (int *)realloc(arr, sizeof(int) *(i + 1));
                arr[i] = tmp;
                i++;
            }
            
            arr_len = i;

            if (arr_len > 2){
                
                if (number_of_child > ((float) arr_len) / 2.0){
                    number_of_child = (int)floor(arr_len / 2);
                    printf("number of child were reduced to %d\n", number_of_child);
                }
                distribution = divided_properly(number_of_child, arr_len, &flag_child);
                if (flag_child == 0){
                    create_n_input_output_files(arr, distribution, number_of_child);

                    PROCESS_INFORMATION pi[number_of_child];
                    STARTUPINFO si[number_of_child];                        
                                               

                    for (int j = 0; j < number_of_child; j++){
                        char *args[4] = {"./subproc.exe", generate_input_filename(j), generate_output_filename(j), NULL};
                        LPSTR concated_args = concat_string(args);
                        
                        GetStartupInfo(&si[j]);

                        BOOL res = CreateProcess("subproc.exe",
                            concated_args,
                            NULL,
                            NULL,
                            TRUE,
                            0,
                            NULL,
                            NULL,
                            &si[j],
                            &pi[j]);
                        printf("pid = %d\n", GetProcessId(pi[j].hProcess));
                        if (res == 0){
                            printf("Error while creating the process");
                        }
                    }
                    for(int y = 0; y < number_of_child; y++){
                        DWORD dwres = WaitForSingleObject(pi[y].hProcess, INFINITE);
                        if (dwres == WAIT_FAILED){
                            printf("Error while waiting\n");
                        }
                        else {
                            CloseHandle(pi[y].hProcess);
                            CloseHandle(pi[y].hThread);
                        }
                    }

                    
                    printf("total cal = %d\n", get_total_sum(number_of_child));
                    
                }else {
                    printf("reduce number of child to %d\n", (int)floor(arr_len / 2));                    
                }
                free(distribution);
            } else {
                printf("Empty file given or there are less then 2 numbers\n");             
            }

        }else {
            printf("Invalid parameter file or number or children process given\n");
        }
    } else {
        printf("Invalid number of arguments: see usage\n ./a.out [input file] [nuber of child process]\n");
    }

}


char * concat_string(char * args[4]){
    char res[100] = "";
    strcat(res, args[0]);
    strcat(res, " ");
    strcat(res, args[1]);
    strcat(res, " ");
    strcat(res, args[2]);


    char * tmp = (char *)malloc(strlen(res) + 1);
    int i;
    for (i = 0; i < strlen (res); i++){
        tmp[i] = res[i];
    }

    tmp[i] = '\0';

    return tmp;
}


int get_total_sum(int number_of_child){
    int sum = 0;
    for (int i = 0; i < number_of_child; i++){
        int tmp;
        char * ou = generate_output_filename(i);    
        FILE * fp = fopen(ou, "r");
        fscanf(fp, "%d", &tmp);
        // printf("tmp = %d\n", tmp);
        sum += tmp;
        fclose(fp);
    }

    return sum;
}


int from_string_to_int(char * arg){
    int number = 0;
    size_t len = strlen(arg);
    for (int i = 0; i < len; ++i) {
        if (!(isdigit(arg[i]))){
            number = -1;
        }
    }
    if (number != -1) {
        sscanf(arg, "%d", &number);
    }
    return number;
}


void create_n_input_output_files(int * arr, int * distribution, int number_of_child) {
    int index = 0;
    for(int i = 0; i < number_of_child; i++){
        char * input_fn = generate_input_filename(i);
        char * output_fn = generate_output_filename(i);


        int tmp_buf[distribution[i]];

        for (int j = 0; j < distribution[i]; j++){
            tmp_buf[j] = arr[index];
            index++;
        }

        print_data_in_the_file(input_fn, tmp_buf, distribution[i]);

        fopen(output_fn, "w+");
        free(input_fn);
        free(output_fn);
    }
}

void print_data_in_the_file(char * filename, int * objects_for_write, int number_of_objects){
    FILE * fp = fopen(filename, "w+");

    for (int i = 0; i < number_of_objects; i++){
        fprintf(fp, "%d ", objects_for_write[i]);
    }

    fclose(fp);
}

char * generate_input_filename(int i){
    char * res = (char *)malloc(15);
    memset(res, 0 , 14);
    char tmp_buf[3];
    sprintf(tmp_buf, "%d", i);
    strcat(res, "input");
    strcat(res, tmp_buf);
    strcat(res, ".txt");
    res[15] = '\0';
    return res;
}


char * generate_output_filename(int i){
    char * res = (char *)malloc(15);
    memset(res, 0 , 14);
    char tmp_buf[3];
    sprintf(tmp_buf, "%d", i);
    strcat(res, "output");
    strcat(res, tmp_buf);
    strcat(res, ".txt");
    res[15] = '\0';
    return res;
}

/*
    логика должна быть следующей: если количество чисел нацело делится между количеством процессов, то делим поровну и кайфуем
    если не делится то делаем следующее:
    M - числа
    N - процессы
    первым N - 1 процессам достается n = [M / N] - с округлением вниз
    N- ому процессу должно достаться n = M - (N - 1) * [M / N]
    !!!если количество процессов больше чем половина данных (N > [M / 2]) -> уменьшаем N до  [M / 2]
*/
int * divided_properly(int number_of_child, int number_of_numbers, int * to_much_child){
    int * distribution = (int *) malloc(sizeof(int) * number_of_child);
    if (number_of_numbers % number_of_child == 0){ //если можно по братски поделить
        for (int i = 0; i < number_of_child; i++) distribution[i] = number_of_numbers / number_of_child;    
    }
    else if (number_of_child > ((float) number_of_numbers) / 2.0){ //проверка на аборт
        *to_much_child = 1;
    }
    else { //воровской дележ
        int first = (int) floor((float) number_of_numbers / (float) number_of_child);
        
        int last = number_of_numbers - ((number_of_child - 1) * (int) (floor( (float) number_of_numbers / (float) number_of_child)));
        for (int i = 0; i < number_of_child; i++){ 
            if (i != number_of_child - 1) distribution[i] = first;
            else distribution[i] = last;
        }
    }
    return distribution;
}