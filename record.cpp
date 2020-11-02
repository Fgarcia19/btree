#include<iostream>
#include <stdio.h>
#include <string.h>
#include "global.h"
using namespace std;

struct record
{
    char key[key_size];
    long long dir[total_diccionarios]=iniciales;
    void set_key(string a)
    {
        string aux;
        aux.append(key_size-a.length(),' ');
        aux+=a;
        strcpy(key, aux.c_str());
    }

    string get_key()
    {
        return string(key);
    }
};
