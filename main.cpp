#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <ctime>
#include "global.h"
#include "btree.cpp"
#include "record.cpp"
#include "diccionario.cpp"
using namespace std;
unsigned inicio, fin;
long accesos;

int main( )
{

    string d[6]={"latin.txt","spanish.txt","french.txt","german.txt","italian.txt","portuguese.txt"};
    diccionario a("diccionario",d);
    inicio=clock();
    a.find("zoom");
    fin=clock();
    double time = (double(fin-inicio)/CLOCKS_PER_SEC);
    cout << "Execution Time: " << time << endl;
    cout<<"Disk access: "<<accesos<<endl;
}
