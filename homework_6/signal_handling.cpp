#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <ucontext.h>
#include <pwd.h>

void handler(int signum, siginfo_t* info, void* context_) {
    if(signum != SIGUSR1) {
        return;
    }
    ucontext_t* context = (ucontext_t*) context_;
    std::cout << "Got signal " << signum << std::endl;
    std::cout << "from PID: " << (unsigned int) info->si_pid << std::endl;
    std::cout << "UID: " << info->si_uid << std::endl;
    struct passwd *pws;
    pws = getpwuid(info->si_uid);
    if (pws != NULL) {
        std::cout << "User name: " <<  pws->pw_name << std::endl;
    }
    std::cout << "EIP: " << context->uc_mcontext.gregs[REG_RIP] << std::endl;
    std::cout << "EAX: " << context->uc_mcontext.gregs[REG_RAX] << std::endl;           
    std::cout << "EBX: " << context->uc_mcontext.gregs[REG_RBX] << std::endl;           
    exit(0);
}

int main() {
    struct sigaction action;
    
    memset(&action, 0, sizeof(action));
    
    action.sa_sigaction = &handler;
    action.sa_flags = SA_SIGINFO;

    // redefining action of SIGUSR1
    int result = sigaction(SIGUSR1, &action, NULL);
    // error handling
    if(result < 0) {
        std::cerr << "Error with sigaction " << strerror(errno) << std::endl;
        exit(errno);
    }
    
    while(true) {
        sleep(10);
    }
    return 0;
}