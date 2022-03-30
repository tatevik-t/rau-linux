#include <iostream>
#include <stdlib.h> 
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/wait.h>


int main(){
    long long N, M;
    std::cin >> N >> M;
    long long * arr = new long long[N];

    // fill matrix with random values
    srand (time(NULL));
    for(long long i = 0; i < N; ++i){
        // arr[i] = 1;                     
        arr[i] = rand() % 10000;
    }

    // struct for keeping child process info
    struct ChildProcessInfo {
        int pid;
        int pipefd[2];
    };
    
    int pipefd_out[2];

    // array of children info
    struct ChildProcessInfo * cpi = new ChildProcessInfo[M];
    
    long long sum = 0;

    // create pipe for output       
    if (pipe(pipefd_out) == -1){
        std::cerr << "(parent) Error while creating pipe_out: " << strerror(errno) << std::endl;
        exit(errno);
    }

    // creating processes
    for(int i = 0; i < M; i++){
        // create pipe for input for each child process
        int pipe_result = pipe(cpi[i].pipefd);        
             
        if (pipe_result == -1){
            std::cerr << "(parent) Error while creating pipe: " << strerror(errno) << std::endl;
            exit(errno);
        }
        
        // write indexes of subarray to pipe 1 
        long long I = i*(N/M);
        long long J = (i+1)*N/M;
        if (write(cpi[i].pipefd[1], &I, sizeof(long long)) < 0){
            std::cerr << "(parent) Error while writing to pipe: " << strerror(errno) << std::endl;
            exit(errno);
        }
        if (write(cpi[i].pipefd[1], &J, sizeof(long long)) < 0){
            std::cerr << "(parent) Error while writing to pipe: " << strerror(errno) << std::endl;
            exit(errno);
        }

        // std::cout << "(parent) " << I << " " << J << std::endl;

        // create new process by forking parent one
        cpi[i].pid = fork();
        if (cpi[i].pid < 0){
            std::cerr << "(parent) Error while forking process " << i << ": " << strerror(errno) << std::endl;
            exit(errno);
        }

        // child process
        if (cpi[i].pid == 0){

            // close unused halves of pipes
            close(cpi[i].pipefd[1]);    
            close(pipefd_out[0]);   
             
            long long I;
            long long J;
            long long temp_sum = 0;

            if (read(cpi[i].pipefd[0], &I, sizeof(long long)) < 0){
                std::cerr << "(child " << i << ") Error while reading from pipe : " << strerror(errno) << std::endl;
                exit(errno);
            }
            if (read(cpi[i].pipefd[0], &J, sizeof(long long)) < 0){
                std::cerr << "(child " << i << ") Error while reading from pipe : " << strerror(errno) << std::endl;
                exit(errno);
            }

            // std::cout << "(child " << i << ") " << I << " " << J << std::endl;
            
            // count temp_sum
            for(long long idx = I; idx < J; ++idx){
                temp_sum += arr[idx];
            }

            std::cout << "(child " << i << ") temp_sum = " << temp_sum << std::endl;
            
            if (write(pipefd_out[1], &temp_sum, sizeof(long long)) < 0){
                std::cerr << "(child " << i << ") Error while writing to pipe: " << strerror(errno) << std::endl;
                exit(errno);
            }

            close(cpi[i].pipefd[0]);
            close(pipefd_out[1]);

            exit(0);
        }
        
        // parent process
        else if (cpi[i].pid > 0) {
            close(cpi[i].pipefd[0]);
            close(cpi[i].pipefd[1]);
        }
    }

    close(pipefd_out[1]);

    int status;
    long long temp_sum;

    // wait for children processes execution end
    for (long long i = 0; i < M; ++i) {
        waitpid(cpi[i].pid, &status, 0);
        
        // get temp_sum from it
        if (read(pipefd_out[0], &temp_sum, sizeof(long long)) < 0){
            std::cerr << "Error while reading from pipe for getting temp_sum : " << strerror(errno) << std::endl;
            exit(errno);
        } 
        sum += temp_sum;
    }

    std::cout << "(parent) sum = " << sum << std::endl;

    close(pipefd_out[0]);

	delete [] arr;
	delete [] cpi;

    return 0;
}