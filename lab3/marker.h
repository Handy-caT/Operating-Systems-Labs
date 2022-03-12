//
// Created by Максим on 11.03.22.
//

#ifndef LAB3_MARKER_H
#define LAB3_MARKER_H

struct markerArguments {
    int* array;
    int size;
    int number;
};

DWORD WINAPI marker(void* arguments);

#endif //LAB3_MARKER_H
