#pragma once

class ParallelMatrix {
protected:
    int* matrix_all;
    int** matrix;
    int n;
    int m;
    friend void* sum_arr(void* arg);

public:
    ParallelMatrix(int n_, int m_);
    ~ParallelMatrix();
    void Init();
    int Sum();
    int SumParallel(int threads_n);
};
