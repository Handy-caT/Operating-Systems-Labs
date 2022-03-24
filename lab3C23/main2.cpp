#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <vector>
#include <queue>
#include <unistd.h>

int size;
int*array;
int markerCount;
int indexMarkerToStop;
bool continueWork;
bool stop;
int j = 0;

void print(int* array, int size) {
    int i = 0;
    while(i < size) {
        std::cout << array[i] << " ";
        ++i;
    }
    std::cout << std::endl;
}

std::condition_variable markerContinue;
std::condition_variable mainContinue;
std::mutex arrayMutex;

void waits(int markerNumber)
{
    while(j != markerCount) {
        {
            std::unique_lock<std::mutex> locker(arrayMutex);
            std::cout << markerNumber << " locked " << j << std::endl;
            while(!stop) {
                markerContinue.wait(locker);
            }
            std::cout << markerNumber << " unlocked " << std::endl;
        }
        if(indexMarkerToStop == markerNumber) {
            {
                break;
            }
        }
        {
            std::unique_lock<std::mutex> locker(arrayMutex);
            j++;
            std::cout << markerNumber << " ended " << j << std::endl;
        }
    }

}

void signals()
{
    std::cout << "Input array size: " << std::endl;
    std::cin >> size;

    array = new int[size];

    std::fill(array,array+size,0);

    std::cout << "Input markers count: " << std::endl;
    std::cin >> markerCount;

    std::vector<std::thread> markersThreadsVector;

    stop = false;

    int i = 0;
    while(i < markerCount) {
        markersThreadsVector.push_back(std::thread(waits,i+1));
        ++i;
    }
    i = 0;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    while(j < markerCount)
    {
        std::unique_lock<std::mutex> arrayLocker(arrayMutex);
        print(array, size);
        std::cout << "Input marker number to stop: " << std::endl;
        std::cin >> indexMarkerToStop;

        stop = true;
        markerContinue.notify_all();

    }

    while( i < markerCount) {
        markersThreadsVector[i].join();
        ++i;
    }
}

int main()
{
    std::thread t4(signals);
    t4.join();
}