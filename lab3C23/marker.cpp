#include <iostream>
#include <unistd.h>
#include "marker.h"

void Marker::markerMain() {

    std::cout << "Input array size: " << std::endl;

}

void Marker::marker(int markerNumber) {
    srand(markerNumber);

    int rNumber;
    int* changedNumbers = new int[size];
    int changedCount = 0;
    bool ind = true;

    while(ind) {

        rNumber = rand();
        rNumber %= size;
        std::unique_lock<std::mutex> array_locker(arrayMutex);
        if(array[rNumber] == 0) {
            sleep(5);
            array[rNumber] = markerNumber;
            changedNumbers[changedCount] = rNumber;
            ++changedCount;
            sleep(5);
            array_locker.unlock();
        } else {
            array_locker.unlock();
            {
                std::unique_lock<std::mutex> locker(printMutex);
                std::cout << markerNumber << " " << changedCount << " " << rNumber << std::endl;
            }
            {
                std::unique_lock<std::mutex> locker(queueMutex);
                errorMarkersIndexes.push(markerNumber);
                queueCheck.notify_one();
            }
            if(indexMarkerToStop == markerNumber) {
                std::unique_lock<std::mutex> locker(arrayMutex);
                while(changedCount > 0) {
                    array[changedNumbers[changedCount]] = 0;
                    --changedCount;
                }
                ind = false;
            } else {
                std::unique_lock<std::mutex> locker(arrayMutex);
                while(!continueWork) {
                    markerContinue.wait(locker);
                }
            }

        }
    }
}

