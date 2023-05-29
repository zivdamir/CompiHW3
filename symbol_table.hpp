#ifndef COMPIHW3symbol_table__
#define COMPIHW3symbol_table__
#endif
#include <vector>
#include <iostream>
#include "table_entry.hpp"

//using namespace std;

class symbol_table
{
public:
    symbol_table *parent;
    vector<table_entry *> entries;
    //bool is_global_scope;
    bool is_in_while = false;
    symbol_table(bool is_global_scope = false, symbol_table *parent = nullptr) : parent(parent)
    {
        if(is_global_scope)
        {
        table_entry* print_func_entry = new table_entry("print", 0, "STRING->VOID",true);
        table_entry* printi_func_entry = new table_entry("printi", 0, "INT->VOID",true);
        this->entries.push_back(print_func_entry);
        this->entries.push_back(printi_func_entry);
        }
    }
    //need to add Table~
    void insert(string name,string type,int offset){
       
                table_entry *new_entry = new table_entry(name, offset, type);
                entries.push_back(new_entry);
                
    }
    bool contains_in_current_scope(string name){
        bool found = false;
        if (!entries.empty())
        {
            for (table_entry* entry: entries){
                if(entry->name == name ){
                    found = true;
                    break;
                }
            }
        }
        return found;
    }
    bool contains(string name)
    {
        bool is_contains = false;
        symbol_table *curr_table = this;
        while(curr_table != nullptr )
        {
            is_contains = curr_table->contains_in_current_scope(name);
            if (is_contains)
            {
                break;
            }
            curr_table = curr_table->parent;
        }
        return is_contains; 
    }
    void printTable()
    {
        cout << "hello levi" << endl;
    }
    void set_is_in_while(bool val){
        this->is_in_while = val;
    }
    bool get_in_while()
    {
        return this->is_in_while;
    }
    table_entry* findByNameInScope(string name)
    {
        table_entry* found = nullptr;
        if (!entries.empty())
        {
            for (table_entry* entry: entries){
                if(entry->name == name){
                    found = entry;
                    break;
                }
            }
        }
        return found;
    }
    table_entry* findByName(string name)
    {
        table_entry* found = nullptr;
        symbol_table *curr_table = this;
        while(curr_table != nullptr )
        {
            found = curr_table->findByNameInScope(name);
            if (found)
            {
                break;
            }
            curr_table = curr_table->parent;
        }
        return found;
    }
    table_entry* getLastDefinedInScope(string name)
    {
        table_entry* found = nullptr;
        if (!entries.empty())
        {
            for (table_entry* entry: entries){
                if(entry->name == name){
                    found = entry;
                }
            }
        }
        return found;
    }
    table_entry* getLastEntry()
    {
        if(entries.empty())
        {
            return nullptr;
        }
        else{
        return entries.back();
        }
    }
};
