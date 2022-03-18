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

    std::queue<int> errorMarkersIndexes;

    bool continueWork;

    int* array;

    int size;
    int indexMarkerToStop;


public:
    void marker(int markerNumber);
    void markerMain();
};



#endif //LAB3C23_MARKER_H
