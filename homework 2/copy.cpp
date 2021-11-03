#include <iostream>
#include <unistd.h>
#include <fcntl.h>
#include <cerrno>

#define BUFFER_SIZE 16

ssize_t LogSize(int& file) {
    // count logical size using lseek
    off_t logSize = lseek(file, 0, SEEK_END);
    
    return logSize;
}

ssize_t PhysSize(int& file) {
    
    // variables to keep the physical size, size, hole offset and data offset values
    ssize_t physSize = LogSize(file);
    ssize_t size = 0;
    off_t dOff;
    off_t hOff;
    off_t tmp;

    // iterate over the file
    while(true) {
        // jump to the next hole
        dOff = lseek(file, size, SEEK_HOLE);
        
        // no holes encountered
        if(dOff < 0) {
            // not end of file
            if(errno != ENXIO) {
                std::cerr << "Something went wrong" << std::endl;
                exit(errno);
            }
            break;
        }
        // change pointer
        size = dOff;

        // std::cout << "dOff = " << dOff << std::endl;
        
        // search for the data and get hole offset (hOff)
        hOff = lseek(file, size, SEEK_DATA);
        
        // std::cout << "hOff = " << hOff << std::endl;

        // if no data encountered -> break (end of file)
        if(hOff <= 0) {
            // not end of file
            if(errno != ENXIO) {
                std::cerr << "Something went wrong" << std::endl;
                exit(errno);
            }
            break;
        }
        // change pointer
        size = hOff;

        // delete hole from physSize
        physSize -=  (hOff - dOff);
    }

    return physSize;
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
    
    // variables for size, data offset, hole offset
    ssize_t size = 0;
    off_t dOff = 0;
    off_t hOff = 0;
    ssize_t dataSize;
    ssize_t holeSize;
    
    // iterate over the file
    while(true) {
        // jump to the next hole
        dOff = lseek(file, size, SEEK_HOLE);
        
        // no holes encountered
        if(dOff < 0) {
            // not end of file
            if(errno != ENXIO) {
                std::cerr << "Something went wrong" << std::endl;
                exit(errno);
            }
            break;
        }

        // change pointer
        size = dOff;
        dataSize = dOff - hOff;

        // std::cout << "dataSize = " << dataSize << std::endl;
        
        // change cursor in origin file
        off_t tmp = lseek(file, size - dataSize, SEEK_SET);

        // error
        if (tmp < 0) {
            std::cerr << "Some error occured" << std::endl;
            exit(errno);
        }

        // change cursor in destination
        tmp = lseek(dest, size - dataSize, SEEK_SET);

        // error
        if (tmp < 0) {
            std::cerr << "Some error occured" << std::endl;
            exit(errno);
        }

        // copy
        for(size_t i = 1; (i-1)*BUFFER_SIZE < dataSize; ++i) {
            
            // read
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

        // std::cout << "dOff = " << dOff << std::endl;
        
        // search for the data and get hole offset (hOff)
        hOff = lseek(file, size, SEEK_DATA);
        
        // std::cout << "hOff = " << hOff << std::endl;

        if(hOff <= 0) {
            // not end of file
            if(errno != ENXIO) {
                std::cerr << "Something went wrong" << std::endl;
                exit(errno);
            }
            break;
        }
        // change pointer
        size = hOff;

        // holesize
        holeSize = hOff - dOff;
        
        // std::cout << "holeSize = " << holeSize << std::endl;
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