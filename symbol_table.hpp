#ifndef COMPIHW3symbol_table__
#define COMPIHW3symbol_table__
#endif
#include <vector>
#include <iostream>
#include "table_entry.hpp"
#include "hw3_output.hpp"
using namespace std;

class symbol_table{
    public:
        symbol_table *parent;
        vector<table_entry*> entries;
        bool is_global_scope;
        symbol_table(bool is_global_scope = false, symbol_table *parent = nullptr) : parent(parent),is_global_scope(is_global_scope)
        {
        }
    //need to add Table~
    void insert(string name,string type,int offset,bool is_func,bool is_override,int yylineno){
       if (!this->is_global_scope)
       {
           if (is_func)
           {
               assert(false);
           }
        }
        if (!is_func)
        {
            if(contains_in_current_scope(name,type))
            {
                output::errorDef(yylineno,name);// maybe return error or something.
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
    bool contains(string name,string type)
    {
        bool is_contains = false;
        symbol_table *curr_table = this;
        while(curr_table != nullptr )
        {
            is_contains = curr_table->contains_in_current_scope(name, type);
            if (is_contains)
            {
                break;
            }
            curr_table = curr_table->parent;
        }
        return is_contains; 
    }
};
