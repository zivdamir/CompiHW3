#ifndef table_entry__
#define table_entry__
#endif
//this class/struct remsembles an entry in symbol table.
#include <string>
#include "assert.h"

using namespace std;
#define NO_RETURN_TYPE 0
#define EMPTY_STRING ""
//we want to define a struct for offset also, so it can consider the case where.
class table_entry
{
    public:
    string name;
    string type;
    int offset;
    bool is_func; // perhaps could be deprecated
    table_entry(string name, int offset, string type ,bool is_func = false)
        : name(name), offset(offset), type(type),is_func(is_func) {
        if(this->is_func)
        {
            assert(this->offset == 0);
        }
        
    }
    string get_return_type(){
        if(!is_func){
            return type;
        }
        else{
            string iterator = this->type;
            int length = iterator.length();
            int position = iterator.find("->");
            if(position == NO_RETURN_TYPE) return EMPTY_STRING;
            else return iterator.substr(position+2,length);
        }
    }
    string get_function_parameters_types()
    {
        assert(this->is_func);//make sure this is func!, would crash otherwise
        string iterator = this->type;
        int position = iterator.find("->");
        if(position == NO_RETURN_TYPE) return EMPTY_STRING;
        else return iterator.substr(0,position);
    }
};