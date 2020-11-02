#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include "global.h"
#include "btree.cpp"
#include "record.cpp"
#include "diccionario.cpp"
using namespace std;


int main( )
{

    string d[5]={"spanish.txt","french.txt","german.txt","italian.txt","portuguese.txt"};
    diccionario a("prueba",d);
    a.find("ab");
}
