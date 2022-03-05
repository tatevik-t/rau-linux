#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <cerrno>
#include <cstring>
#include <semaphore.h>

int main() {
    // open shared memory file
    int fd = shm_open("prod-cons-buffer", O_CREAT|O_RDWR, 777);
    if (fd < 0) {
        std::cerr << "cons: Error while opening a file due to error: " << strerror(errno) << std::endl;
        exit(errno);
    }

    // truncate file to one page size
    long size = sysconf(_SC_PAGESIZE);
    int result = ftruncate(fd, size);
    if (result < 0) {
        std::cerr << "cons: Error while truncating a file due to error: " << strerror(errno) << std::endl;
        exit(errno);
    }
    
    // mmap in memory, get pointer
    void * addr = mmap(NULL, size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (addr == MAP_FAILED) {
        std::cerr << "cons: Error while mapping memory due to error: " << strerror(errno) << std::endl;
        exit(errno);
    }

    // semaphore for tracking is file full
    char * is_full_name = (char *) "is_full";
    sem_t * is_full_sem = sem_open(is_full_name, O_CREAT, 666, size);
    if (is_full_sem == SEM_FAILED) {
        std::cerr << "cons: Error while opening semaphore is_full due to error: " << strerror(errno) << std::endl;
        exit(errno);
    }

    // semaphore for tracking is file empty
    char * is_empty_name = (char *) "is_empty";
    sem_t * is_empty_sem = sem_open(is_empty_name, O_CREAT, 666, 0);
    if (is_empty_sem == SEM_FAILED) {
        std::cerr << "con: Error while opening semaphore is_empty due to error: " << strerror(errno) << std::endl;
        exit(errno);
    }
    
    // semaphore for tracking is one process accesses file
    char * memory_name = (char *) "memory";
    sem_t * memory_sem = sem_open(memory_name, O_CREAT, 666, 1);
    if (memory_sem == SEM_FAILED) {
        std::cerr << "cons: Error while opening semaphore for shared memory due to error: " << strerror(errno) << std::endl;
        exit(errno);
    }

    char * offset = (char * ) addr;
    char char_1 = '\0';
    char * buffer = &char_1;

    // the loop of deleting characters from file
    while(true) {
        sem_wait(is_empty_sem);
        sem_wait(memory_sem);
        
        std::size_t lseeked = lseek(fd, -1, SEEK_CUR);
        std::size_t deleted = write(fd, buffer, 1);
            if (deleted < 1) {
                std::cerr << "cons: Error while deleting last char due to error: " << strerror(errno) << std::endl;
                exit(errno);
            }
        --(*offset);

        sem_post(memory_sem);
        sem_post(is_full_sem);

    }

    // close and unlink semaphores
    sem_close(is_full_sem);
    sem_close(is_empty_sem);
    sem_close(memory_sem);
    sem_unlink(is_full_name);
    sem_unlink(is_empty_name);
    sem_unlink(memory_name); 

    // unmap and close
    munmap(addr, sizeof(int));
    close(fd);
    shm_unlink("prod-cons-buffer");
    return 0;
    return 0;
}