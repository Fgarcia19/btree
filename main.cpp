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

    string d[6]={"latin.txt","spanish.txt","french.txt","german.txt","italian.txt","portuguese.txt"};
    diccionario a("diccionario",d);
    a.find("ab");
}
