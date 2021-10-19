#include <iostream>
#include "complex.h"
#include <vector>

void merge(std::vector<Complex>& arr, size_t start, size_t mid, size_t end)
{
    size_t l1 = mid - start + 1; // length of 1st
    size_t l2 = end - mid; // length of 2nd
    std::vector<Complex> arr1(l1); // 1st vector
    std::vector<Complex> arr2(l2); // 2nd vector
    
    for(size_t i = 0; i < l1; ++i)
        arr1[i] = arr[start + i];
    for(size_t i = 0; i < l2; ++i)
        arr2[i] = arr[mid + 1 + i];

    size_t idx1 = 0, idx2 = 0, idx = start;
    while (idx1 < l1 && idx2 < l2)
    {
        if (arr1[idx1].abs() <= arr2[idx2].abs())
            arr[idx++] = arr1[idx1++];
        else
            arr[idx++] = arr2[idx2++];
    }
    while (idx1 < l1)
        arr[idx++] = arr1[idx1++];
    while (idx2 < l2)
        arr[idx++] = arr2[idx2++];
    
}

void mergeSortComplex(std::vector<Complex>& arr, size_t start = 0, size_t end = 0)
{
    if(start >= end)
        return;
    size_t mid = start + (end - start) / 2;
    mergeSortComplex(arr, start, mid);
    mergeSortComplex(arr, mid+1, end);
    merge(arr, start, mid, end);
}

int main()
{
    size_t n = 6;
    std::vector<Complex> arr(n);
    arr[0] = Complex(1, 3);
    arr[1] = Complex(1);
    arr[2] = Complex();
    int re, im;
    std::cout << "Enter 3 complex numbers (divide real and imaginary parts by space): " << std::endl;
    for (size_t i = 3; i < n; ++i)
    {
        std::cin >> re >> im;
        arr[i].set_real(re);
        arr[i].set_imaginary(im);
    }

    std::cout << "Before sorting" << std::endl;
    for(size_t i = 0; i < n; ++i)
    {
        std::cout << arr[i] << ", ";
    }
    std::cout << std::endl;
    
    mergeSortComplex(arr, 0, n-1);
    std::cout << "After sorting" << std::endl;
    for(size_t i = 0; i < n; ++i)
    {
        std::cout << arr[i] << ", ";
    }
    std::cout << std::endl;
}