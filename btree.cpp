
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;
string index_name;

template<typename T, int ORDER>
class btree{
public:
    enum state_t {OVERFLOW, UNDERFLOW, B_OK};
    string file_name;
    struct node;
    static node* read(unsigned long dir)
    {
      accesos+=1;
      auto r = new node{};
      std::ifstream f;
      f.open(index_name, std::ios::binary);
      f.seekg(dir,std::ios::beg);
      f.read((char *)r, sizeof(node));
      f.close();
      return r;

    };
    static void write(node& nodo)
    {
      std::ofstream f;
      if (std::ifstream(index_name))
      {
          f.open(index_name, std::ios::binary| std::ios::in |std::ios::ate);
          f.seekp(nodo.dir);

      }
      else
      {
          f.open(index_name, std::ios::binary);
      }
      f.write((char *)&nodo, sizeof(node));
      f.close();
    };

    static node* allocate_node()
    {
      auto z = new node{};
      z->dir=tell();
      write(*z);
      return z;
    };

    static unsigned long tell()
    {
      std::ifstream f;
      f.open(index_name, std::ifstream::binary);
      f.seekg(0,std::ios::end);
      unsigned long aux=f.tellg();
      f.close();
      return aux;
    }


    struct node{
      bool leaf;
      unsigned long dir;
      T data[ORDER+1];
      unsigned long children[ORDER+2];
      size_t count{0};
      void insert_into(size_t index,  T& value) {
          size_t j = this->count;
          while (j > index) {
              children[j+1] = children[j];
              data[j] = data[j-1];
              j--;
          }
          children[j+1] = children[j];
          data[j] = value;
          this->count++;
          write(*this);
      }


        void split(size_t position) {
            node* parent = this;
            node* ptr = read(children[position]);
            node* child1 = allocate_node();
            node* child2 = allocate_node();
            child1->leaf=ptr->leaf;
            child2->leaf=ptr->leaf;

            size_t i = 0;
            for (; i < ptr->count / 2; i++) {
                child1->children[i] = ptr->children[i];
                child1->data[i] = ptr->data[i];
                child1->count++;
            }
            child1->children[i] = ptr->children[i];
            auto mid = i;
            i += 1;
            size_t j = 0;
            for (; i < ptr->count; i++) {
                child2->children[j] = ptr->children[i];
                child2->data[j] = ptr->data[i];
                child2->count++;
                j++;
            }
            write(*ptr);
            child2->children[j] = ptr->children[i];
            parent->insert_into(position, ptr->data[mid]);
            parent->children[position + 1] = child2->dir;
            parent->children[position] = child1->dir;
            write(*parent);
            write(*child1);
            write(*child2);

        }

      state_t insert( T& value) {
          size_t index = 0;
          while (this->data[index].get_key() < value.get_key()  && index < this->count) {
              index += 1;
          }
          if (this->leaf) {
              this->insert_into(index, value);
          }
          else {
                auto a=read(children[index]);
                auto state=a->insert(value);
              if (state == state_t::OVERFLOW) {
                  this->split(index);
              }
          }
          return this->count > ORDER ? OVERFLOW : B_OK;
      }

      void print()
      {
        // index_name="B-"+file_name;
        for(int i=0;i<count;i++)
          cout<<data[i].get_key()<<" ";
      }

      int buscar(T value)
      {
        int i=0;
        for(i;i<this->count;i++)
        {
          if(value.get_key()==data[i].get_key())
            return i;
        }
      }



    };
      btree(string name){
        file_name=name;
        index_name="B-"+name;
        if (!std::ifstream(index_name))
        {
            auto a=new node;
            a->dir=0;
            a->leaf=true;
            ofstream f(index_name,ios::binary);
            f.seekp(0);
            f.write((char*)(a),sizeof(node));
            f.close();
        }
      }

      void split_root() {
        node* ptr = read(0);
        node* child1 = allocate_node();
        node* child2 = allocate_node();
        child1->leaf=ptr->leaf;
        child2->leaf=ptr->leaf;
        ptr->leaf=false;
        size_t i = 0;
        for (; i < ptr->count / 2; i++) {
            child1->children[i] = ptr->children[i];
            child1->data[i] = ptr->data[i];
            child1->count++;
        }
        child1->children[i] = ptr->children[i];
        auto mid = i;
        i += 1;
        size_t j = 0;
        for (; i < ptr->count; i++) {
            child2->children[j] = ptr->children[i];
            child2->data[j] = ptr->data[i];
            child2->count++;
            j++;
        }
        child2->children[j] = ptr->children[i];

        ptr->data[0] = ptr->data[mid];
        ptr->children[0] = child1->dir;
        ptr->children[1] = child2->dir;
        ptr->count = 1;
        write(*ptr);
        write(*child1);
        write(*child2);
    }

      void insert( T& value) {
          index_name="B-"+file_name;
          auto a=read(0);
          auto state=a->insert(value);
        if (state == state_t::OVERFLOW) {
            split_root();
          }
        }

      void printf(node *ptr, int level) {
          int i;
          for (i = ptr->count - 1; i >= 0; i--) {
            if(!ptr->leaf){
              auto a=read(ptr->children[i + 1]);
              printf(a, level + 1);
            }
              for (int k = 0; k < level; k++) {
                std::cout << "    ";
              }
            std::cout << ptr->data[i].get_key() <</* " -> "<<ptr->data[i].dir[0] <<*/ "\n";
          }
          if(!ptr->leaf){
          auto a=read(ptr->children[i + 1]);
          printf(a, level + 1);
          }
  }

      void print()
      {
        index_name="B-"+file_name;
        auto a=read(0);
        printf(a, 0);
        cout<<index_name;
      }

      node* find(T value)
      {
        accesos=0;
        index_name="B-"+file_name;
        auto root=read(0);
        return find_helper(root,value);
      }

      node* find_helper(node* ptr, T& value) {
        size_t index = 0;
        while (ptr->data[index].get_key() < value.get_key()  && index < ptr->count) {
            index += 1;
        }
        if (ptr->data[index].get_key() == value.get_key()) {
            // return true;
            return ptr;
        }
        if(ptr->leaf) return nullptr; //false;
        auto child=read(ptr->children[index]);
        return find_helper(child, value);
    }
};
