#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/wait.h>
#include <fcntl.h>

// function to count words using spaces
int word_count(char* str){
    int space_c = 0;
    for(int i = 0; i < strlen(str); i++){
        if(str[i] == ' ')
            ++space_c;
    }
    return space_c + 1;
}

// make an array of words from char string
void get_arguments(char str[], char* a[]){
    int j = 0;
    int word_c = word_count(str); 
    
    // add pointer to the first word to a
    a[j++] = str;
    int len = strlen(str);

    // change all spaces to '\0'
    for(int i = 0; i < len; i++){
        if(str[i] == ' '){
            str[i] = '\0';
            a[j++] = str + i + 1;
        }
    }
}

// function to delete multiple spaces in string
void delete_spaces(char str[]){

    // delete spaces before the first word
    for(int i = 0; str[i] == ' '; ){
        int j = i;
        while(true){
            str[j] = str[j+1];
            if(str[j++] == '\0'){
                break;
            }
        }
    }

    // delete multiple spaces in between the words 
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] == ' ' && str[i+1] == ' '){
            int j = i;
            while(true){
                str[j] = str[j+1];
                if(str[j++] == '\0'){
                    break;
                }
            }
            i--;
        }
    }

    // delete spaces after the last word
    for(int i = strlen(str) - 1; str[i] == ' '; ){
        int j = i;
        while(true){
            str[j] = '\0';
            if(str[j-1] != ' '){
                break;
            }
            j--;
        }
    }

    // std::cout << str << std::endl;
}

int main() {
    std::cout << "\u001b[32m=======================================\nStarting little shell:\u001b[0m" << std::endl;
    while(true){
        // size of 64 is enough for the command
        char cmnd[64]; 
        
        // get the line of command
        std::cout << "\u001b[32m>>> \u001b[0m";
        std::cin.getline(cmnd, 64);
        delete_spaces(cmnd);

        // checking if it's exit command
        std::string cmnd_str(cmnd);
        if(cmnd_str == "exit") exit(0);
        
        // create new process
        int pid = fork();

        // error creating process
        if (pid < 0) {
            std::cerr << "Error while creating process " << strerror(errno) << std::endl;
            exit(errno);
        }

        // child process
        if (pid == 0) {           
            // count the words
            int word_c = word_count(cmnd);
            
            // create an array of arguments
            char** a = new char*[word_c];
            get_arguments(cmnd, a);
            
            // execute the command
            int executed = execvp(a[0], a);

            // encountered the error
            if (executed == -1){
                std::cerr << "Error while executing: " << strerror(errno) << std::endl;
                exit(errno);
            }
        }

        int status = 0;

        // wait from parent process
        if (pid > 0) {
            int waited = waitpid(pid, &status, 0);
            if(waited == -1){
                std::cerr << "Error while waiting the process " << strerror(errno) << std::endl;
                exit(errno);
            }
        }
    }
    return 0;
}