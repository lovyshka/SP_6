#include "linux_header.h"

int SUM = 0;
pthread_mutex_t mutex;

void * routine(void * data_struct){
    
    struct arguments * arg = (struct arguments *) data_struct;
    int * arr = arg->numbers;
    int len = arg->len_of_arr;
    // printf("len of ar = %d\n", len);
    int tmp_sum;
    for (int i = 0; i < len; i++){
        printf("%d\n", arr[i]);
        if (arg->numbers[i] % 2 == 0){
            tmp_sum += arg->numbers[i];
        }    
    }

    pthread_mutex_lock(&mutex);
    SUM += tmp_sum;
    pthread_mutex_unlock(&mutex); 
    
    pthread_exit(0);
}


int main(int argc, char *argv[]){
    
    if (argc == 3){
        int number_of_threads = from_string_to_int(argv[2]);
        FILE * fp = fopen(argv[1], "r");
        if (number_of_threads > 0 && fp){
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
                int index = 0;
                if (number_of_threads > ((float) arr_len) / 2.0){
                    number_of_threads = (int)floor(arr_len / 2);
                    printf("number of threads were reduced to %d\n", number_of_threads);
                }
                distribution = divided_properly(number_of_threads, arr_len, &flag_child);
                
                pthread_t * arr_of_threads = (pthread_t *)malloc(sizeof(pthread_t) * number_of_threads); // создаем массивчик со структурами потоков
                if (pthread_mutex_init(&mutex, NULL) != 0){
                    printf("error while creating thread\n");
                    return -1;
                };

                for (int t = 0; t < number_of_threads; t++){
                    struct arguments * arg = (struct arguments *)malloc(sizeof(struct arguments));

                    int tmp_buf[distribution[t]];

                    for (int j = 0; j < distribution[t]; j++){
                        tmp_buf[j] = arr[index];                        
                        index++;
                    }
                    
                    arg[0].numbers = tmp_buf;
                    // debud(&arg, number_of_threads);
                    arg[0].len_of_arr = distribution[t];

 
                    if (pthread_create(&arr_of_threads[t], NULL, &routine, (void *) &arg[0]) != 0){
                        printf("some error while creating the threads\n");
                        pthread_mutex_destroy(&mutex);
                        return -2;
                    };

                    if (pthread_join(arr_of_threads[t], NULL) != 0){
                        printf("error while waiting of exit thread\n");
                        return -4;
                    }

                }

                // for (int n = 0; n < number_of_threads; n++){
                    
                // }
                // for (int n = 0; n < number_of_threads; n++){
                    
                // }
                
                pthread_mutex_destroy(&mutex);           
                
                printf("total sum for curr input is = %d\n", SUM);
                free(distribution);
            }
            else {
                printf("Empty file given or there are less then 2 numbers\n");             
            }
            free(arr);
        }
        else {
            printf("Invalid parameter file or number or children process given\n");
        }
    } else {
        printf("Invalid number of arguments: see usage\n ./a.out [input file] [nuber of child process]\n");
    }
    return 0;
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
int * divided_properly(int number_of_threads, int number_of_numbers, int * to_much_child){
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