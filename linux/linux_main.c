#include "linux_header.h"

int SUM = 0;
pthread_mutex_t mutex_routine;

void * routine(void * data_struct){
    struct arguments * arg = (struct arguments *) data_struct;
    int * arr = arg->arr;
    int len = arg->len_of_arr;
    int tmp_sum;
    for (int i = 0; i < len; i++){
        if (arg->arr[i] % 2 == 0){
            tmp_sum += arg->arr[i];
        }    
    }

    if (pthread_mutex_lock(&mutex_routine) != 0){
        printf("Error while locking mutex\n");
    }
    else SUM += tmp_sum;
    if (pthread_mutex_unlock(&mutex_routine) != 0){
        printf("Error while unlocking mutex\n");
    }
}


int main(int argc, char *argv[]){
    if (check_argc(argc) == 1) return -1; //checek argc

    int number_of_threads = from_string_to_int(argv[2]); 
    FILE * fp = fopen(argv[1], "r");
    if (lovit_cal(fp, number_of_threads) == 1) return -1; //check valid fp and number_of_threads 
    
    int data_cnt = get_number_of_input(fp);
    if (check_data(data_cnt) == 1) return -1; //validate number of data in file
    
    body(fp, number_of_threads, data_cnt);

    fclose(fp);     
    return 0;
}

void body(FILE * fp, int number_of_threads, int data_cnt){
    check_and_reduce(&number_of_threads, data_cnt);

    int batch_size = 10;
    int tmp, arr_len = 0;
    int * arr = (int *) calloc(1, sizeof(int) * batch_size); 

    while (fscanf(fp, "%d", &tmp) == 1){
        if ((arr_len % batch_size == 0) && arr_len != 0){
            work_with_thread(number_of_threads, arr, 10);
            memset(arr, 0, sizeof(int) * batch_size);
        }
        arr[arr_len % batch_size] = tmp;
        arr_len++;
        if ((arr_len == data_cnt) && arr_len != 0){
            work_with_thread(number_of_threads, arr, 10);
            memset(arr, 0, sizeof(int) * batch_size);
        }
    }

    printf("total sum for curr input is = %d\n", SUM);

}

int check_argc(int argc){
    int flag = 0;
    if (argc != 3){
        flag = 1;
        printf("Invalid number of arguments: see usage\n ./a.out [input file] [nuber of child process]\n");
    }
    return flag;
}

int lovit_cal(FILE * fp, int number_of_threads){
    int flag = 0;
    if (fp == NULL){
        flag = 1;
        printf("No such file\n");
    }
    else if (number_of_threads <= 0 || number_of_threads >= 32750){
        flag = 1;
        printf("Invalid parameter file or number or children process given, be careful, that maximum number of threads is 32750\nAlso check the syntax, it has to be ./a.out [input file] [number of threads]\n");
    }
    return flag;
}

int check_data(int data_cnt){
    int flag = 0;
    if (data_cnt < 1){
        flag = 1;
        printf("Empty file given or there are less then 2 numbers\n");
    }
    return flag;
}

int get_number_of_input(FILE * fp){
    int cnt = 0;
    int tmp;
    while (fscanf(fp, "%d", &tmp) == 1){
        cnt++;
    }
    rewind(fp);
    return cnt;
}

void check_and_reduce(int * number_of_threads, int arr_len){
    if (*number_of_threads > ((float) arr_len) / 2.0){
        *number_of_threads = (int)floor(arr_len / 2);
        printf("number of threads were reduced to %d\n", *number_of_threads);
    }
}

void work_with_thread(int number_of_threads, int * arr, int arr_len){
    struct arguments * original_args = (struct arguments *)malloc(sizeof(struct arguments) * number_of_threads);
    pthread_t * arr_of_threads = (pthread_t *)malloc(sizeof(pthread_t) * number_of_threads);
    int * distribution = divided_properly(number_of_threads, arr_len);

    if (pthread_mutex_init(&mutex_routine, NULL) != 0){
        printf("Error while init mutex\n");
        free_dynamic(distribution, arr, original_args, arr_of_threads);
        exit(-1);
    }
    init_arguments(number_of_threads, distribution, original_args, arr);
    
    if (create_threads_and_pass_args(number_of_threads, arr_of_threads, original_args) == -1){
        printf("Error while creating process\n");
        free_dynamic(distribution, arr, original_args, arr_of_threads);
        // exit(-3);
        pthread_exit(0);
    }

    if (wait_for_threads(number_of_threads, arr_of_threads) == -1){
        printf("Error while waiting process\n");
        free_dynamic(distribution, arr, original_args, arr_of_threads);
        exit(-4);
    }

    if (pthread_mutex_destroy(&mutex_routine) != 0){
        printf("Error while destroying mutex\n");
        free_dynamic(distribution, arr, original_args, arr_of_threads);
        exit(-5);
    }
    
}

void free_dynamic(int * distribution, int * arr, struct arguments * original_args, pthread_t * arr_of_threads){
    free(distribution);
    free(arr);
    free(arr_of_threads);
    free(original_args);
}

void init_arguments(int number_of_threads, int * distribution, struct arguments * original_args, int * arr){
    int index = 0;
    for (int i = 0; i < number_of_threads; i++){
        original_args[i].arr = (int *) malloc(sizeof(int) * distribution[i]);
        original_args[i].len_of_arr = distribution[i];
        
        for (int j = 0; j < distribution[i]; j++){
            original_args[i].arr[j] = arr[index];
            index++;
        }
    }
}

int create_threads_and_pass_args(int number_of_threads, pthread_t * arr_of_threads, struct arguments * original_args){
    int status = 0;
    for (int i = 0; i < number_of_threads; i++){
        if (pthread_create((arr_of_threads + i), NULL, &routine, (void * ) (original_args + i)) != 0){
            printf("Error while creating process\n");
            status = -1;
        }
    }
    return status;
}

int wait_for_threads(int number_of_threads, pthread_t * arr_of_threads){
    int status = 0;
    for (int i = 0; i < number_of_threads; i++){
        if (pthread_join(arr_of_threads[i], NULL) != 0) status = -1;
    }
    return status;
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



/*
    логика должна быть следующей: если количество чисел нацело делится между количеством процессов, то делим поровну и кайфуем
    если не делится то делаем следующее:
    M - числа
    N - процессы
    первым N - 1 процессам достается n = [M / N] - с округлением вниз
    N- ому процессу должно достаться n = M - (N - 1) * [M / N]
    !!!если количество процессов больше чем половина данных (N > [M / 2]) -> уменьшаем N до  [M / 2]
*/
int * divided_properly(int number_of_threads, int number_of_numbers){
    int * distribution = (int *) malloc(sizeof(int) * number_of_threads);
    if (number_of_numbers % number_of_threads == 0){ //если можно по братски поделить
        for (int i = 0; i < number_of_threads; i++) distribution[i] = number_of_numbers / number_of_threads;    
    }
    else { //воровской дележ
        int first = (int) floor((float) number_of_numbers / (float) number_of_threads);
        
        int last = number_of_numbers - ((number_of_threads - 1) * (int) (floor( (float) number_of_numbers / (float) number_of_threads)));
        for (int i = 0; i < number_of_threads; i++){ 
            if (i != number_of_threads - 1) distribution[i] = first;
            else distribution[i] = last;
        }
    }
    return distribution;
}