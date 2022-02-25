#include <iostream>
#include "windows.h"

std::pair<int,int> min_max(int* array, int size) {
    int i = 0;
    int max = array[0];
    int min = max;
    while(i < size) {
        if(array[i] > max) max = array[i];
        Sleep(7);
        if(array[i] < min) min = array[i];
        Sleep(7);
        ++i;
    }
    return std::pair<int,int>(min,max);
}

int main() {
    int size;
    int* array;

    printf("Input array size:");
    scanf("%i",size);
    array = new int[size];
    printf("Input array numbers:");

    int i = 0;
    while(i < size) {
        scanf("%i",array[i]);
        ++i;
    }


}
