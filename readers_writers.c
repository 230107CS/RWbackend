#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>   // sleep
#include <time.h>

#define N_READERS 5
#define N_WRITERS 2

//защищает ридкаунт блок врайтерс
pthread_mutex_t rc_mutex = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t wrt_mutex = PTHREAD_MUTEX_INITIALIZER; 

int read_count = 0; //current num of readers

// timestamp утилиты
double start_time;
double now_seconds() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (ts.tv_sec - start_time) + ts.tv_nsec / 1e9;
}

// Функция читателя трэд
void* reader(void* arg) {
    int id = *(int*)arg;
    free(arg);

    // ридеры пытаются 3 раза читать
    for (int iter = 0; iter < 3; ++iter) {
        //delay before reading
        usleep((rand() % 500 + 100) * 1000); // 100..600 ms

        // Вход в секцию чтения !!!
        pthread_mutex_lock(&rc_mutex);
        read_count++;
        if (read_count == 1) {
            pthread_mutex_lock(&wrt_mutex);//первый пошел
        }
        printf("[%.3f] Reader %d enters (read_count=%d)\n", now_seconds(), id, read_count);
        pthread_mutex_unlock(&rc_mutex);//allows others to enter

        // Критическая секция: чтение !!!
        printf("[%.3f] Reader %d is READING...\n", now_seconds(), id);
        sleep(1); // чтение занимает 1 секунду
        printf("[%.3f] Reader %d finished READING\n", now_seconds(), id);

        // Выход из секции чтения
        pthread_mutex_lock(&rc_mutex);
        read_count--;
        printf("[%.3f] Reader %d leaves (read_count=%d)\n", now_seconds(), id, read_count);
        if (read_count == 0) {
            // последний читатель снимает блокировку для писателей
            pthread_mutex_unlock(&wrt_mutex);
        }
        pthread_mutex_unlock(&rc_mutex);
    }

    return NULL;
}

// Функция писателя трэд
void* writer(void* arg) {
    int id = *(int*)arg;
    free(arg);

    for (int iter = 0; iter < 3; ++iter) {
        // Задержка перед попыткой писать (симуляция работы)
        usleep((rand() % 700 + 200) * 1000); // 200..900 ms

        // Писатель ждёт доступа к ресурсу
        printf("[%.3f] Writer %d wants to write\n", now_seconds(), id);
        pthread_mutex_lock(&wrt_mutex); // эксклюзивный доступ
        printf("[%.3f] Writer %d starts WRITING\n", now_seconds(), id);

        //Критическая секция: запись!!!
        sleep(2); // запись занимает 2 секунды
        printf("[%.3f] Writer %d finished WRITING\n", now_seconds(), id);

        pthread_mutex_unlock(&wrt_mutex);
    }

    return NULL;
}

int main() {
    srand(time(NULL));
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    start_time = ts.tv_sec + ts.tv_nsec / 1e9;

    pthread_t readers[N_READERS];
    pthread_t writers[N_WRITERS];

    // потоки читателей
    for (int i = 0; i < N_READERS; ++i) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&readers[i], NULL, reader, id) != 0) {
            perror("pthread_create reader");
            return 1;
        }
    }

    // потоки писателей
    for (int i = 0; i < N_WRITERS; ++i) {
        int *id = malloc(sizeof(int));
        *id = i + 1;
        if (pthread_create(&writers[i], NULL, writer, id) != 0) {
            perror("pthread_create writer");
            return 1;
        }
    }

    //завершение всех потоков и канес
    for (int i = 0; i < N_READERS; ++i) pthread_join(readers[i], NULL);
    for (int i = 0; i < N_WRITERS; ++i) pthread_join(writers[i], NULL);

    printf("[%.3f] All threads finished. Exiting.\n", now_seconds());
    return 0;
}