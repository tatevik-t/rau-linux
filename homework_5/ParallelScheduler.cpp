#include "ParallelScheduler.h"
#include <stdlib.h> 
#include <pthread.h>
#include <iostream>

void* function(void* arg);

// constructor with max number of threads
ParallelScheduler::ParallelScheduler(int num) :  N(num)
{
    // // mutex for locking the queue 
    // pthread_mutex_t* mutex;
    // pthread_mutex_init(mutex, NULL);
    
    // // cond var for status of queue
    // pthread_cond_t* filled;
    // pthread_cond_init(filled, NULL);
    
    thread_queue = new std::queue<std::pair<void* (*)(void*), void*>>;
    
    std::cout << "THREADS" << std::endl;
    
    // create threads
    threads = new pthread_t[N];
    for(int i = 0; i < N; ++i){
        int created = pthread_create(&threads[i], NULL, function, (void*)this );
        std::cout << "CREATED THREAD NUMBER " << i << std::endl;
        if(created != 0){
            std::cerr << "Error while creating threads: " << created << std::endl;
            exit(created);
        }
    }
}

// destructor
ParallelScheduler::~ParallelScheduler()
{
    std::cout << "DESTRUCTOR" << std::endl;

    while(!thread_queue->empty()) {}

    // join threads
    for(size_t i = 0; i < N; ++i)
    {
       int joined = pthread_join(threads[i], NULL);
       std::cout << "CREATED THREAD NUMBER " << i << std::endl;
       if(joined != 0){
           std::cerr << "Error while joining threads: " << joined << std::endl;
           exit(joined);
       }
    }
    
    pthread_cond_destroy(filled);
    pthread_mutex_destroy(mutex);
    
    // release the memory
    delete [] thread_queue;
}

// run a function in thread
void ParallelScheduler::run(void* (*function)(void*), void* argument)
{
    std::cout << "RUN" << std::endl;

    // lock the queue
    int result = pthread_mutex_lock(mutex);
    if(result != 0){
        std::cerr << "Error while locking mutex: " << result << std::endl;
        exit(result);
    }

    thread_queue->push(std::make_pair(function, argument));

    std::cout << "PUSHED FUNCTION" << std::endl;

    // signal not empty
    int signaled = pthread_cond_signal(filled);
    if(signaled != 0){
        std::cerr << "Error while signaling cond var: " << signaled << std::endl;
        exit(signaled);
    }

    // unlock queue
    result = pthread_mutex_unlock(mutex);
    if(result != 0){
        std::cerr << "Error while locking mutex: " << result << std::endl;
        exit(result);
    }

}

// the function that executes functions from queue
void* function(void* arg){
    ParallelScheduler* sched = (ParallelScheduler*) arg; 

    while(!sched->thread_queue->empty()){
        // lock the queue
        int result = pthread_mutex_lock(sched->mutex);
        if(result != 0){
            std::cerr << "Error while locking mutex: " << result << std::endl;
            exit(result);
        }

        while(sched->thread_queue->empty()) {}

        // get the fucntion and it's argument
        void* (*f)(void*)= sched->thread_queue->front().first;
        void* f_arg = sched->thread_queue->front().second;
        sched->thread_queue->pop();

        if(sched->thread_queue->empty()) { 
            pthread_cond_wait(sched->filled, sched->mutex);
        }

        // unlock queue
        result = pthread_mutex_unlock(sched->mutex);
        if(result != 0){
            std::cerr << "Error while locking mutex: " << result << std::endl;
            exit(result);
        }

        // call f
        f(f_arg);
    }

    return NULL;
}