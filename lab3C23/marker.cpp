#include <iostream>
#include <unistd.h>
#include <thread>
#include "marker.h"

void print(int* array, int size) {
    int i = 0;
    while(i < size) {
        std::cout << array[i] << " ";
        ++i;
    }
    std::cout << std::endl;
}

void Marker::markerMain() {

    std::cout << "Input array size: " << std::endl;
    std::cin >> size;

    array = new int[size];

    std::fill(array,array+size,0);

    std::cout << "Input markers count: " << std::endl;
    std::cin >> markerCount;

    std::vector<std::thread> markersThreadsVector;

    continueWork = false;

    int i = 0;
    while(i < markerCount) {
        markersThreadsVector.push_back(std::thread(&Marker::marker,std::ref(*this),i+1));
        ++i;
    }

    continueWork = true;
    markerContinue.notify_all();

    i = 0;
    while( i < markerCount) {
        markersThreadsVector[i].join();
        ++i;
    }

    std::unique_lock<std::mutex> locker(queueMutex);
    while(errorMarkersIndexes.size() < size) {
        queueCheck.wait(locker);
    }
    continueWork = false;
    {
        std::unique_lock<std::mutex> arrayLocker(arrayMutex);
        print(array,size);
        std::cout << "Input marker number to stop: " << std::endl;
        std::cin >> indexMarkerToStop;
        finalWork = false;
        continueWork = true;
        markerContinue.notify_all();
    }

}

void Marker::marker(int markerNumber) {
    srand(markerNumber);

    {
        std::unique_lock<std::mutex> locker(arrayMutex);
        std::cout << markerNumber << std::endl;
        while(!continueWork) markerContinue.wait(locker);
        std::cout << markerNumber << std::endl;
    }
    /*
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
            {
                std::unique_lock<std::mutex> locker(arrayMutex);
                while(!continueWork) {
                    markerContinue.wait(locker);
                }
            }
            if(indexMarkerToStop == markerNumber) {
                std::unique_lock<std::mutex> locker(arrayMutex);
                while(changedCount > 0) {
                    array[changedNumbers[changedCount]] = 0;
                    --changedCount;
                }
                ind = false;
                finalWork = true;
            } else {
                std::unique_lock<std::mutex> locker(arrayMutex);
                while(!finalWork) {
                    finalContinue.wait(locker);
                }
            }

        }
    }*/
}

