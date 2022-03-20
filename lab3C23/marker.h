#include <mutex>
#include <queue>


#ifndef LAB3C23_MARKER_H
#define LAB3C23_MARKER_H


class Marker {
    std::mutex printMutex;
    std::mutex queueMutex;
    std::mutex arrayMutex;

    std::condition_variable queueCheck;
    std::condition_variable markerContinue;
    std::condition_variable finalContinue;

    std::queue<int> errorMarkersIndexes;

    bool continueWork;
    bool finalWork;

    int* array;

    int size;
    int markerCount;
    int indexMarkerToStop;


public:
    void marker(int markerNumber);
    void markerMain();
};



#endif //LAB3C23_MARKER_H
