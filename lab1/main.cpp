#include <cstdio>
#include <windows.h>
#include <string>

void CreatorUtilityStart(char* commandLineArgument) {

    STARTUPINFO si;
    PROCESS_INFORMATION piApp;

    ZeroMemory(&si,sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    CreateProcessA("Creator.exe",commandLineArgument,NULL,NULL,
                   FALSE,NULL,NULL,NULL,&si,&piApp);
    WaitForSingleObject(piApp.hThread,INFINITE);
    CloseHandle(piApp.hThread);
     CloseHandle(piApp.hProcess);

};

void ReporterUtilityStart(char* commandLineArgument) {

    STARTUPINFO si;
    PROCESS_INFORMATION piApp;

    ZeroMemory(&si,sizeof(STARTUPINFO));
    si.cb = sizeof(STARTUPINFO);

    CreateProcessA("Reporter.exe",commandLineArgument,NULL,NULL,
                   FALSE,NULL,NULL,NULL,&si,&piApp);
    WaitForSingleObject(piApp.hThread,INFINITE);
    CloseHandle(piApp.hThread);
    CloseHandle(piApp.hProcess);

}

void printReportToConsole(const char* reportFileName) {

    FILE* inputFile;
    inputFile = fopen(reportFileName,"r");

    char string[50];
    while(fgets(string,50,inputFile)) {
        printf("%s",string);
    }

    printf("\n");
    fclose(inputFile);
}

int main(int argc, char* args[]) {

    system("chcp 1251");

    if(argc < 3) {
        printf("Not enought arguments. Try again!\n");
    } else {

        char* reportFileName = new char[20];
        char* hourSalary = new char[10];

        int size = strlen("code ") + strlen(args[1]) + strlen(args[2]) + 2;
        char* commandLineArgument = new char[size];
        commandLineArgument[0] = '\0';
        strcat(commandLineArgument,"code ");
        strcat(commandLineArgument,args[1]);
        strcat(commandLineArgument," ");
        strcat(commandLineArgument,args[2]);

        CreatorUtilityStart(commandLineArgument);

        printf("Enter report file name:");
        scanf("%s",reportFileName);
        printf("Enter hourly salary:");
        scanf("%s",hourSalary);

        size = strlen("code ") + strlen(args[1]) + strlen(reportFileName) + strlen(hourSalary) + 3;
        commandLineArgument = new char[size];
        commandLineArgument[0] = '\0';
        strcat(commandLineArgument,"code ");
        strcat(commandLineArgument,args[1]);
        strcat(commandLineArgument," ");
        strcat(commandLineArgument,reportFileName);
        strcat(commandLineArgument," ");
        strcat(commandLineArgument,hourSalary);

        ReporterUtilityStart(commandLineArgument);

        printReportToConsole(reportFileName);

    }

    return 0;
}
