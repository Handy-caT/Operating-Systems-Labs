#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <vector>
#include <queue>



int size;
int*array;
int markerCount;
bool continueWork;


std::condition_variable markerContinue;
std::condition_variable queueCheck;
std::mutex arrayMutex;
std::mutex queueMutex;
std::mutex printMutex;

std::queue<int> errorMarkersIndexes;

void marker(int markerNumber) {
    {
        std::unique_lock<std::mutex> locker(arrayMutex);
        while(!continueWork) markerContinue.wait(locker);
    }


    int rNumber;
    int* changedNumbers = new int[size];
    int changedCount = 0;
    bool ind = true;

    while(ind) {

        rNumber = rand();
        rNumber %= size;
        std::unique_lock<std::mutex> array_locker(arrayMutex);
        if (array[rNumber] == 0) {
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            array[rNumber] = markerNumber;
            changedNumbers[changedCount] = rNumber;
            ++changedCount;
            array_locker.unlock();
            std::this_thread::sleep_for(std::chrono::milliseconds(5));

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
            ind = false;
            {
                std::unique_lock<std::mutex> locker(printMutex);
                std::cout << markerNumber << " break " << errorMarkersIndexes.size() << std::endl;
            }
        }
    }


}

void markerMain() {

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
        markersThreadsVector.push_back(std::thread(marker,i+1));
        ++i;
    }
    i = 0;

    continueWork = true;
    markerContinue.notify_all();
    {
        std::unique_lock<std::mutex> locker(queueMutex);
        while(errorMarkersIndexes.size() < markerCount) queueCheck.wait(locker);
    }

    {
        std::unique_lock<std::mutex> locker(printMutex);
        std::cout << "Main queue full " << errorMarkersIndexes.size() << std::endl;
    }
    continueWork = false;

    while( i < markerCount) {
        markersThreadsVector[i].join();
        ++i;
    }

}



int main()
{
    std::thread t4(markerMain);
    t4.join();
}

