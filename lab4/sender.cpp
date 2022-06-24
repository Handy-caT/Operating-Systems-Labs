#include <iostream>
#include <fstream>
#include <Windows.h>
#include <string>
#include <cstdlib>
const CHAR*  EVENT_NAME ="StartSenderEvent";
const int NUM_OF_ARGS = 2;
const CHAR* STOP_MESSAGE = "0";
CRITICAL_SECTION criticalSection;

int main(int args, char* argv[]){
    InitializeCriticalSection(&criticalSection);
    if(args==NUM_OF_ARGS){
        HANDLE Event = OpenEventA(EVENT_MODIFY_STATE, FALSE, argv[0]);
        HANDLE startEvent = OpenEventA(SYNCHRONIZE, FALSE, EVENT_NAME);
        std::ofstream fout(argv[1], std::ios_base::binary | std::ios_base::out | std::ios_base::app);
        std::string message;
        SignalObjectAndWait(Event, startEvent, INFINITE, FALSE);
        bool ind = false;
        while(!ind){
            printf("Input message to send or " + STOP_MESSAGE + " to stop");
            std::cin>> message;
            if(STOP_MESSAGE != message) {
                EnterCriticalSection(&criticalSection);
                fout << message << std::endl;
                SetEvent(Event);
                LeaveCriticalSection(&criticalSection);
            } else ind = true;
        }
        CloseHandle(startEvent);
        fout.close();
        CloseHandle(Event);
    }
    return 0;
}