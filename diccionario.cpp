#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <time.h>
#include <vector>


struct diccionario{
    B_TREE* dicc;
    string names[total_diccionarios];
    diccionario(string name,string a[]){
      if (!std::ifstream("B-"+name))
      {
        dicc= new B_TREE (name);
        for(int i=0;i<total_diccionarios;i++)
        {
          names[i]=a[i];
          create(names[i],i);
        }
      }
      else
      {
        dicc= new B_TREE (name);
        for(int i=0;i<total_diccionarios;i++)
          names[i]=a[i];
      }
    }

    string extract_key(string line)
    {
        string file;
        for(int i=0;i<line.size();i++)
        {
            if(line[i]==' ')
            return file;

            file=file+line[i];
        }
    }

    vector<int> pos;
    void create(string n,int i)
    {
        string line,key;
        ifstream file(n);
        while(!file.eof())
        {
            pos.push_back(file.tellg());
            getline(file,line);
            string k=extract_key(line);
            record nuevo;
            nuevo.set_key(k);
            auto aux=dicc->find(nuevo);
            if(aux==nullptr)
            {
              nuevo.dir[i]=pos.back();
              dicc->insert(nuevo);
            }
            else
            {
              int p=aux->buscar(nuevo);
              aux->data[p].dir[i]=pos.back();
              dicc->write(*aux);
            }
            pos.pop_back();
        }
        file.close();
    }

    void find(string value)
    {
        record bus;
        bus.set_key(value);
        auto aux=dicc->find(bus);
        if(aux==nullptr)
          cout<<"No se encuentra"<<endl;
        else
        {
          int p=aux->buscar(bus);
          bus=aux->data[p];
          for(int i =0;i<total_diccionarios;i++)
          {
            if(bus.dir[i]==-1)
            {
              cout<<"En "<<names[i]<<" no se encuentra"<<endl;
            }
            else
            {
              ifstream file(names[i]);
              file.seekg(bus.dir[i]);
              string def;
              getline(file,def);
              cout<<"En "<<names[i]<<" "<<def<<endl;
              file.close();
            }
          }

        }

    };

};
