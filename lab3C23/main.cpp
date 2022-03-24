#include <iostream>
#include <thread>
#include "marker.h"

int main() {

    Marker markerClass;
    std::thread t(&Marker::markerMain,std::ref(markerClass));
    t.join();
    return 0;
}
