#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

struct args{
    int * arr;
    int len_of_arr;
};

int SUM = 0;
pthread_mutex_t mutex;

void * routine(void * data_struct){
    printf("pid = %d\n", getpid());
    sleep(300);
    pthread_mutex_lock(&mutex);
    struct args * copy = (struct args *) data_struct;
    int * local_arr = copy->arr;
    int local_len_arr = copy->len_of_arr;
    
    for (int i = 0; i < local_len_arr; i++){
        if (local_arr[i] % 2 == 0) {
            SUM += local_arr[i];
        }
    }
    pthread_mutex_unlock(&mutex);
}

int main(){
    int number_of_threads = 2;

    struct args * original_args = (struct args *)malloc(sizeof(struct args) * number_of_threads);
    pthread_t * arr_of_threads = (pthread_t *)malloc(sizeof(pthread_t) * number_of_threads);

    int  original_arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
    int len_arr[] = {4, 4};
    
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < number_of_threads; i++){
        original_args[i].arr = (int *) malloc(sizeof(int) * len_arr[i]);
        original_args[i].len_of_arr = len_arr[i];
        
        for (int j = 0; j < len_arr[i]; j++){
            if (i == 0){
                original_args[i].arr[j] = original_arr[j];
            }
            else{
                original_args[i].arr[j] = original_arr[4 + j];
            }
       }
    }

    for (int i = 0; i < number_of_threads; i++){
        
        pthread_create(&arr_of_threads[i], NULL, &routine, (void * ) (original_args + i));

    }

    for (int i = 0; i <number_of_threads; i++){
        pthread_join(arr_of_threads[i], NULL);
    }

    pthread_mutex_destroy(&mutex);

    printf("total sum = %d\n", SUM);

    free(original_args);
    free(arr_of_threads);
    return 0;
}