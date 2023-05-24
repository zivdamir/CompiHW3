#include <vector>
#include "table_entry.hpp"
using namespace std;

class Table{
    public:
        Table *parent;
        vector<table_entry*> entries;
    Table(bool is_global_scope = false,Table* parent = nullptr):parent(parent){
        
    }
    void insert(string name,string type,int offset,bool is_func){
        return;
    }
    bool contains_in_scope(string name, string type){
        bool found = false;
        if (!entries.empty())
            for (table_entry* entry: entries){
                if(entry->name == name && entry->type == type){

                }
            {
        }
        return found;
    }
};