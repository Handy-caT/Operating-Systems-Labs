#include <iostream>
#include <condition_variable>
#include <thread>
#include <chrono>
#include <vector>
#include <queue>

void print(int* array, int size) {
    int i = 0;
    while(i < size) {
        std::cout << array[i] << " ";
        ++i;
    }
    std::cout << std::endl;
}

int size;
int*array;
int markerCount;
int deletedMarkersCount;
int indexMarkerToStop;

bool finalWork;
bool continueWork;
bool stop;
bool markerStopWait;

std::condition_variable finalContinue;
std::condition_variable markerContinue;
std::condition_variable queueCheck;
std::condition_variable endMarker;

std::mutex arrayMutex;
std::mutex queueMutex;
std::mutex printMutex;

std::queue<int> errorMarkersIndexes;

void marker(int markerNumber) {

    {
        std::unique_lock<std::mutex> locker(arrayMutex);
        while(!continueWork) markerContinue.wait(locker);
    }
    srand(markerNumber);

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
            /*
            {
                std::unique_lock<std::mutex> locker(printMutex);
                std::cout << markerNumber << " " << changedCount << " " << rNumber << std::endl;
            }
            */
            {
                std::unique_lock<std::mutex> locker(queueMutex);
                errorMarkersIndexes.push(markerNumber);
                queueCheck.notify_one();
            }
            /*
            {
                std::unique_lock<std::mutex> locker(printMutex);
                std::cout << markerNumber << " break " << errorMarkersIndexes.size() << std::endl;
            }
            */
            {
                std::unique_lock<std::mutex> locker(arrayMutex);
                //std::cout << markerNumber << " locked " <<  std::endl;
                while(!stop) {
                    markerContinue.wait(locker);
                }
                //std::cout << markerNumber << " unlocked " << std::endl;
            }

            if(indexMarkerToStop == markerNumber) {
                std::unique_lock<std::mutex> locker(arrayMutex);
                //std::cout << markerNumber << " changing " << errorMarkersIndexes.size() << std::endl;
                while(changedCount > -1) {
                    --changedCount;
                    //std::cout << changedCount << " " << changedNumbers[changedCount] << std::endl;
                    array[changedNumbers[changedCount]] = 0;

                }
                ind = false;
                finalWork = true;
            } else {
                std::unique_lock<std::mutex> locker(arrayMutex);
                //std::cout << markerNumber << " final waiting " <<  std::endl;
                while(!finalWork) {
                    finalContinue.wait(locker);
                }
                //std::cout << markerNumber << " working again " <<  std::endl;
            }
        }
    }

    {
        //std::unique_lock<std::mutex> locker(printMutex);
        //std::cout << markerNumber << " stop " << errorMarkersIndexes.size() << std::endl;
        markerStopWait = true;
        endMarker.notify_all();
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


    while(deletedMarkersCount < markerCount) {

        stop = false;

        {
            std::unique_lock<std::mutex> locker(queueMutex);
            while (errorMarkersIndexes.size() < markerCount - deletedMarkersCount) queueCheck.wait(locker);
        }

        finalWork = false;
        markerStopWait = false;
        /*
        {
            std::unique_lock<std::mutex> locker(printMutex);
            std::cout << "Main queue full " << errorMarkersIndexes.size() << std::endl;
        }
        */
        {
            {
                std::unique_lock<std::mutex> arrayLocker(arrayMutex);
                print(array, size);
                std::cout << "Input marker number to stop: " << std::endl;
                std::cin >> indexMarkerToStop;
            }

            {
                std::unique_lock<std::mutex> locker(queueMutex);
                while (!errorMarkersIndexes.empty()) errorMarkersIndexes.pop();
            }

            stop = true;
            markerContinue.notify_all();
            deletedMarkersCount++;

            {
                std::unique_lock<std::mutex> locker(queueMutex);
                while (!markerStopWait) endMarker.wait(locker);
            }

            {
                std::unique_lock<std::mutex> locker(printMutex);
                print(array, size);
                //std::cout << errorMarkersIndexes.size() << " " << deletedMarkersCount << std::endl;
            }

            finalWork = true;
            finalContinue.notify_all();
        }


    }

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

