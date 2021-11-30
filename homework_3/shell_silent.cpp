#include <iostream>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>

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
    while(true){
        // create new process
        int pid = fork();

        // error creating process
        if (pid < 0) {
            std::cerr << "Error while creating process " << strerror(errno) << std::endl;
            exit(errno);
        }

        // child process
        if (pid == 0) {

            // size of 64 is enough for the command
            char cmnd[64]; 
            
            // get the line of command
            std::cin.getline(cmnd, 64);
            delete_spaces(cmnd);
            // std::cout << cmnd << std::endl;
            
            // if empty -> continue
            if(std::string(cmnd) == ""){
                continue;
            }
            
            int child_pid = getpid();
            
            // create silentshell folder and {PID} subfolder
            int create_dir = mkdir(((std::string)"/opt/silentshell/" + std::to_string(child_pid)).c_str(), 0777);
            if(create_dir == -1){
                std::cerr << "Error while creating the folder /opt/silentshell/" <<  std::to_string(child_pid) << " : " << strerror(errno) << std::endl;
                exit(errno);
            }

            // change "in" file descriptors
            std::string in_path_str = "/opt/silentshell/" + std::to_string(child_pid) + "/in.std";
            const char* in_path = in_path_str.c_str();
            // std::cout << in_path << std::endl;

            int in_fd = open(in_path, O_RDONLY|O_CREAT, S_IRWXO|S_IRWXU|S_IRWXG);
            if (in_fd < 0) {
                std::cerr << "Error while opening the file " << in_path << ". " << strerror(errno) << std::endl;
                exit(errno);
            }

            int duped0 = dup2(in_fd, 0);
            // error with duplicating
            if(duped0 < 0){
                std::cerr << "Couldn't duplicate the out descriptor due to error " << strerror(errno) << std::endl;
                exit(errno); 
            }


            // change "out" file descriptors
            std::string out_path_str = "/opt/silentshell/" + std::to_string(child_pid) + "/out.std";
            const char* out_path = out_path_str.c_str();
            // std::cout << out_path << std::endl;

            int out_fd = open(out_path, O_WRONLY|O_CREAT, S_IRWXO|S_IRWXU|S_IRWXG);
            if (out_fd < 0) {
                std::cerr << "Error while opening the file " << out_path << ". " << strerror(errno) << std::endl;
                exit(errno);
            }

            int duped1 = dup2(out_fd, 1);
            // error with duplicating
            if(duped1 < 0){
                std::cerr << "Couldn't duplicate the out descriptor due to error " << strerror(errno) << std::endl;
                exit(errno); 
            }


            // change "err" file descriptors
            std::string err_path_str = "/opt/silentshell/" + std::to_string(child_pid) + "/err.std";
            const char* err_path = err_path_str.c_str();
            // std::cout << err_path << std::endl;

            int err_fd = open(err_path, O_WRONLY|O_CREAT, S_IRWXO|S_IRWXU|S_IRWXG);
            if (err_fd < 0) {
                std::cerr << "Couldn't open the child's err std due to error " << strerror(errno) << std::endl;
                exit(errno); 
            }

            int duped2 = dup2(err_fd, 2);
            // error with duplicating
            if(duped2 < 0){
                std::cerr << "Couldn't duplicate the err descriptor due to error " << strerror(errno) << std::endl;
                exit(errno); 
            }
            
            // count the words
            int word_c = word_count(cmnd);
            
            // create an array of arguments
            char** a = new char*[word_c];
            get_arguments(cmnd, a);
            
            // execute the command
            int executed = execvp(a[0], a);

            // encountered the error
            if (executed == -1){
                std::cerr << "Error while executing " << cmnd << ": "<< strerror(errno) << std::endl;
                exit(errno);
            }

            int close0 = close(in_fd);
            int close1 = close(out_fd);
            int close2 = close(err_fd);
            return (close0 < 0 || close1 || close2) ? errno : 0;
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

// to run create file with commands
// ./shell_silent < cmnd 