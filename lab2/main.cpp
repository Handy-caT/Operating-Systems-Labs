#include <iostream>
#include <conio.h>
#include <windows.h>


struct min_max_struct {
    int* array;
    int size;
    std::pair<int,int>& pair;
};

struct average_struct {
    int* array;
    int size;
    int& result;
};

DWORD WINAPI min_max(void* params) {
    min_max_struct* a = (min_max_struct*)params;
    int size = a->size;
    int* array = a->array;
    int i = 0;
    int max = 0;
    int min = 0;
    while(i < size) {
        if(array[i] > array[max]) max = i;
        Sleep(7);
        if(array[i] < array[min]) min = i;
        Sleep(7);
        ++i;
    }
    a->pair =  std::pair<int,int>(min,max);
}

DWORD WINAPI average(void* params) {
    average_struct* a = (average_struct*)params;
    int size = a->size;
    int* array = a->array;
    int i = 0;
    int average = 0;
    while (i < size) {
        average += array[i];
        Sleep(12);
        ++i;
    }
    a->result = average/size;
}

int main() {
    int size;
    int* array;

    printf("Input array size:");
    scanf("%i",&size);
    array = new int[size];
    printf("Input array numbers:");

    int i = 0;
    while(i < size) {
        scanf("%i",&array[i]);
        ++i;
    }
    std::pair<int,int> pair;
    min_max_struct a = {array,size,pair};
    int result;
    average_struct b = {array,size,result};
    HANDLE averageThread = CreateThread(NULL,0,average,&b, 0, NULL);
    HANDLE minMaxThread = CreateThread(NULL,0,min_max,&a, 0, NULL);
    WaitForSingleObject(averageThread,INFINITE);
    WaitForSingleObject(minMaxThread,INFINITE);

    array[a.pair.first] = b.result;
    array[a.pair.second] = b.result;

    i = 0;
    while(i < size) {
        printf("%i ",array[i]);
        ++i;
    }

}
