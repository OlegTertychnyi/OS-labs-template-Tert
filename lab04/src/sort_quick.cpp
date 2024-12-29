#include "library.h"
#include <functional>

extern "C" {

LIBRARY_API int* Sort(int* array, int size) {
    std::function<void(int*, int, int)> quicksort = [&](int* arr, int low, int high) {
        if (low < high) {
            int pivot = arr[high];
            int i = low -1;
            for(int j = low; j < high; ++j){
                if(arr[j] < pivot){
                    i++;
                    std::swap(arr[i], arr[j]);
                }
            }
            std::swap(arr[i+1], arr[high]);
            int pi = i +1;
            quicksort(arr, low, pi -1);
            quicksort(arr, pi +1, high);
        }
    };

    quicksort(array, 0, size -1);
    return array;
}

}
