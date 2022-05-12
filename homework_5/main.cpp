#include <iostream>
#include "ParallelScheduler.h"
#include <string>

void* write(void* arg){
	int* num = (int*) arg;
	for(int i = 0; i < *num; i++){
		std::cout << "function " << *num << std::endl;
	}
	return NULL;
}

int main(){
	// create a scheduler
	ParallelScheduler* scheduler = new ParallelScheduler(32);
	for(int i = 0; i < 64; i++){
		scheduler -> run(write, &i);
	}
	return 0;
}
