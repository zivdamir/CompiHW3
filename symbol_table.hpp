#ifndef COMPIHW3symbol_table__
#define COMPIHW3symbol_table__
#endif
#include "table_entry.hpp"

//using namespace std;

class symbol_table
{
public:
    symbol_table *parent;
    vector<table_entry *> entries;
    bool in_while;
    symbol_table(bool is_global_scope = false, symbol_table *parent = nullptr, bool in_while = false) : parent(parent), in_while(in_while)
    {
        if(is_global_scope)
        {
            table_entry* print_func_entry = new table_entry("print", 0, "STRING->VOID",true, false);
            table_entry* printi_func_entry = new table_entry("printi", 0, "INT->VOID",true, false);
            this->entries.push_back(print_func_entry);
            this->entries.push_back(printi_func_entry);
        }
    }
    //need to add Table~
    void set_in_while(bool val){
        this->in_while = val;
    }
    bool get_in_while(){
        return this->in_while;
    }
    void insert(string name,string type,int offset, bool is_func, bool is_override){
       
                table_entry *new_entry = new table_entry(name, offset, type, is_func, is_override);
                //printf("in insert before push back: %s\n", name.data());
                entries.push_back(new_entry);
                //printf("in insert after push back: %s\n", name.data());
                
    }
    bool contains_in_current_scope(string name){
        bool found = false;
        if (!entries.empty())
        {
            for (table_entry* entry: entries){
                if(entry->name == name){
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
        for (table_entry* entry : this->entries)
        {
            //bool num_commas = false;
            if(entry->is_func)
            {
                std::vector<std::string> types = entry->fill_vector_func_params();
                output::printID(entry->name, entry->offset, output::makeFunctionType(entry->get_return_type(), types));
            }
            else
            {
                output::printID(entry->name, entry->offset, entry->type);
            }
            
        }
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
                    //cout<<"global object name "<<entry->name<<endl;
                    found = entry;
                }
            }
            //cout<<"end of iteration on global scope"<<endl;
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