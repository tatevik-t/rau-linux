#pragma once
#include <queue>
#include <pthread.h>

class ParallelScheduler {
protected:
    int N;
    pthread_t* threads;
    std::queue<std::pair<void* (*)(void*), void*>>* thread_queue;
    pthread_mutex_t* mutex;
    pthread_cond_t* filled;

    friend void* function(void* arg);

public:
    ParallelScheduler(int num);
    ~ParallelScheduler();
    void run(void* (*function)(void*), void* argument);
};