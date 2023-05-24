#include <vector>
#include "table_entry.hpp"
using namespace std;

class Table{
    public:
        Table *parent;
        vector<table_entry*> entries;
        bool is_global_scope;
        Table(bool is_global_scope = false, Table *parent = nullptr) : parent(parent),is_global_scope(is_global_scope)
        {
        }
    //need to add Table~
    void insert(string name,string type,int offset,bool is_func){
        assert(!(!this->is_global_scope && is_func));//idk if should be condition or not todo
        if (!is_func)
        {
            if(contains_in_current_scope(name,type))
            {
                return;// maybe return error or something.
            }
            else{
                table_entry *new_entry = new table_entry(name, offset, type);
                if(!new_entry){
                    assert (false); //TODO
                }
                entries.push_back(new_entry);
            }
        }

        return;
    }
    bool contains_in_current_scope(string name, string type){
        bool found = false;
        if (!entries.empty())
        {
            for (table_entry* entry: entries){
                if(entry->name == name && entry->type == type){
                    found = true;
                    break;
                }
            }
        }
        return found;
    }
    
};