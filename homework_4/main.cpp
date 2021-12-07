#include <iostream>
#include "parallelmatrix.h"
#include <chrono>

typedef std::chrono::steady_clock::time_point t;

int main(){
	// initialize a matrix
	ParallelMatrix* m = new ParallelMatrix(1000, 1000);
	m->Init();

	int sum;
	t begin;
	t end;
	
	// normal sum
	begin = std::chrono::steady_clock::now();
	sum = m->Sum();
	end = std::chrono::steady_clock::now();
	std::cout << "Sum() = " << sum << "\t\t\t Time = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	
	// 4 threads
	begin = std::chrono::steady_clock::now();
	sum = m->SumParallel(4);
	end = std::chrono::steady_clock::now();
	std::cout << "SumParallel(4) = " << sum << "\t\t Time = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	
	// 8 threads
	begin = std::chrono::steady_clock::now();
	sum = m->SumParallel(8);
	end = std::chrono::steady_clock::now();
	std::cout << "SumParallel(8) = " << sum << "\t\t Time = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	
	// 16 threads
	begin = std::chrono::steady_clock::now();
	sum = m->SumParallel(16);
	end = std::chrono::steady_clock::now();
	std::cout << "SumParallel(16) = " << sum << "\t\t Time = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	
	// 100 threads
	begin = std::chrono::steady_clock::now();
	sum = m->SumParallel(100);
	end = std::chrono::steady_clock::now();
	std::cout << "SumParallel(100) = " << sum << "\t\t Time = " << std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin).count() << "[ns]" << std::endl;
	
	return 0;
}
