#include <iostream>
#include "windows.h"

std::pair<int,int> min_max(int* array, int size) {
    int i = 0;
    int max = 0;
    int min = 0;
    while(i < size) {
        if(array[i] > max) max = i;
        Sleep(7);
        if(array[i] < min) min = i;
        Sleep(7);
        ++i;
    }
    return std::pair<int,int>(min,max);
}

int average(int* array, int size) {
    int i = 0;
    int average = 0;
    while (i < size) {
        average += array[i];
        Sleep(12);
        ++i;
    }
    return average/size;
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
