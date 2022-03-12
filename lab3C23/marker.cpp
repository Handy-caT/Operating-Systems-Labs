#include <iostream>
#include <unistd.h>
#include "marker.h"

void marker(int* array,int size, int number) {
    srand(number);

    int rNumber;
    int* changedNumbers = new int[size];
    int changedCount = 0;
    bool ind = true;

    while(ind) {
        rNumber = rand();
        rNumber %= size;
        if(array[rNumber] == 0) {
            sleep(5);
            array[rNumber] = rNumber;
            changedNumbers[changedCount] = rNumber;
            ++changedCount;
            sleep(5);
        } else {
            std::cout << number << " " << changedCount << " " << rNumber << std::endl;
            ind = false;
        }
    }
}

