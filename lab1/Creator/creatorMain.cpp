#include <cstdio>
#include <cstdlib>

struct employee{
    int num;
    char name[10];
    double hours;
};

void addEmployeeToFile(FILE* outputFile) {
    employee newEmployee = {};
    printf("Input new employee data\n");
    printf("Number: ");
    scanf("%d", &newEmployee.num);
    printf("Name: ");
    scanf("%s",newEmployee.name);
    printf("Hours: ");
    scanf("%lf",&newEmployee.hours);

    fwrite(&newEmployee,sizeof(employee),1,outputFile);
}

int main(int argc, char* args[]) {

    if(argc < 3) {
        printf("Not enought arguments. Try again!\n");
    } else {
        int numOfLines = atoi(args[2]);
        FILE* outputFile;
        outputFile = fopen(args[1],"wb");
        if(outputFile) {
            while(numOfLines != 0) {
                addEmployeeToFile(outputFile);
                numOfLines--;
            }
            fclose(outputFile);
        } else {
            printf("Error with opening a file occurs\n");
        }
        printf("Programm finished successfully\n");
    }

    return 0;
}