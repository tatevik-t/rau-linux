#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

#define BUFFER_SIZE 16

ssize_t PhysSize(int& file) {
    // reset cursor
    lseek(file, 0, SEEK_SET);
    
    // variables to keep the size and tmp values
    ssize_t physSize = 0, add, d;
    
    // count physical size
    do {
        // jump to the next hole, register the size
        add = lseek(file, 0, SEEK_HOLE);
        if(add < 0) {
            break;
        }
        physSize += add;
        
        // jump to the next data
        d = lseek(file, 0, SEEK_DATA);
    } while (add > 0 && d > 0);

    // reset cursor
    lseek(file, 0, SEEK_SET);

    return physSize;
}

ssize_t LogSize(int& file) {
    // reset cursor
    lseek(file, 0, SEEK_SET);
    
    // count logical size using lseek
    lseek(file, 0, SEEK_SET);
    off_t logSize = lseek(file, 0, SEEK_END);

    // reset cursor
    lseek(file, 0, SEEK_SET);
    
    return logSize;
}

void displaySize(int& file) {
    std::cout << "logical size of file " << file << " = " << LogSize(file) << "\t";
    std::cout << "physical size of file " << file << " = " << PhysSize(file) << std::endl;
}

int main(int argc, char** argv) {
    
    // if file to copy is not provided
    if (argc < 2) {
        std::cerr << "Please provide a file to copy" << std::endl;
        exit(1);
    }
    // if the place where to copy is not specified
    else if (argc < 3) {
        std::cerr << "Please specify the destination" << std::endl;
        exit(1);
    }

    // file paths for file and destination
    const char* filepath = argv[1];
    const char* destpath = argv[2];

    // open file to copy in read-only mode
    int file = open(filepath, O_RDONLY);

    // check if can open a file
    if (file < 0) {
        std::cerr << "Could not open the file due to error " << errno << std::endl;
        exit(errno);
    }

    // open destination file (if doesn't exist create it)
    int dest = open(destpath, O_WRONLY | O_TRUNC | O_CREAT, S_IRUSR | S_IWUSR);
    
    // the destination is wrong
    if (dest < 0) {
        std::cerr << "Could not open the destination due to error " << errno << std::endl;
        exit(errno);
    }

    // display the sizes
    displaySize(file);
    displaySize(dest);

    // create a buffer for reading
    char* buffer = new char[BUFFER_SIZE];

    ssize_t logSize = 0, add, d;
    
    ssize_t size = LogSize(file);

    // iterate over holes and data
    while (true) {
        // jump to the next hole, register the size
        add = lseek(file, 0, SEEK_HOLE);
        if(add < 0) {
            break;
        }
        
        // return for copying
        lseek(file, logSize, SEEK_SET);
        
        logSize += add;
        // copy
        for(size_t i = 1; (i-1)*BUFFER_SIZE < add; ++i) {
            ssize_t readBytes = read(file, buffer, BUFFER_SIZE);
            
            // check if could not read due to error
            if (readBytes < 0) {
                std::cerr << "Could not read from file due to error " << errno << std::endl;
                exit(errno);
            }

            // write content of the buffer into dest
            ssize_t written = write(dest, buffer, readBytes);

            // could not write a buffer
            if (written < 0) {
                std::cerr << "Could not write buffer due to error " << errno << std::endl;
                exit(errno);
            }
        }
        
        // jump to the next data
        d = lseek(file, 0, SEEK_DATA);
        if (d < 0) {
            break;
        }
        logSize += d;

        // create a hole
        lseek(dest, d, SEEK_CUR);
    };


    std::cout << logSize << " " << size << std::endl;

    if(logSize < size){
        // lseek(dest, 1000000000000000, SEEK_END);
        lseek(dest, size-logSize, SEEK_END);
    }

    std::cout << "Copied!" << std::endl;

    // release used memory
	delete [] buffer;

    // display the sizes
    displaySize(file);
    displaySize(dest);

	// close files
	int closed = close(file);
    int closed2 = close(dest);

    return 0;
}