#include <cstdio>
#include <cstdlib>

struct employee{
    int num;
    char name[10];
    double hours;
};

void addHeaderToOuputFile(FILE* outputFile,char* fileName) {
    fprintf(outputFile,"Отчет по файлу %s\n",fileName);
    fprintf(outputFile,"%-35s%-33s%-16s%-24s\n","Номер сотрудника","Имя сотрудника","Часы","Зарплата");
}

void addNextLineToFile(employee nextEmployee,int hoursSalary,FILE* outputFile) {
    fprintf(outputFile,"%20d%20s%12.2lf%16.2lf\n",nextEmployee.num,nextEmployee.name,
                nextEmployee.hours,nextEmployee.hours * (double)hoursSalary);
}

int main(int argc, char* args[]) {

    if(argc < 4) {
        printf("Not enought arguments. Try again!\n");
    } else {
        //args[1] binaryFileName
        //args[2] outputFileName
        int hourSalary = atoi(args[3]);

        employee nextEmployee = {};
        FILE* outputFile;
        FILE* binaryFile;

        outputFile = fopen(args[2],"w");
        binaryFile = fopen(args[1],"rb");

        addHeaderToOuputFile(outputFile,args[1]);

        while(!feof(binaryFile)) {
            fread(&nextEmployee,sizeof(employee),1,binaryFile);
            if(!feof(binaryFile)) addNextLineToFile(nextEmployee,hourSalary,outputFile);
        }
        printf("Programm finished successfully\n");
    }

}