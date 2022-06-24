//
// Created by Максим on 11.03.22.
//
#include "windows.h"
#include <vector>

#ifndef LAB3_MARKER_H
#define LAB3_MARKER_H

struct markerArguments {
    int* array;
    int& markerToStop;
    int size;
    int markerNumber;

    CRITICAL_SECTION &criticalSection;
    HANDLE startEvent;

    markerArguments(int *array, int size, int number, const CRITICAL_SECTION &criticalSection, const void *startEvent,
                    int &markerToStop) : array(array), size(size), markerNumber(number),
                                                                       criticalSection(const_cast<CRITICAL_SECTION &>(criticalSection)), startEvent(const_cast<HANDLE>(startEvent)),
                                                                       markerToStop(markerToStop) {}
};

unsigned int WINAPI marker(void* arguments);

#endif //LAB3_MARKER_H
