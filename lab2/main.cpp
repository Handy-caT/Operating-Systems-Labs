#include <iostream>



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
