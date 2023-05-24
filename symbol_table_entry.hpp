//this class/struct remsembles an entry in symbol table.
#include <string>
#include "assert.h"

using namespace std;
#define NO_RETURN_TYPE 0
#define EMPTY_STRING ""
//we want to define a struct for offset also, so it can consider the case where.
class symbol_table_entry
{
    public:
    string name;
    string type;
    int offset;
    bool is_func; // perhaps could be deprecated
    symbol_table_entry(string name, int offset, string type ,bool is_func = false)
        : name(name), offset(offset), type(type),is_func(is_func) {
        assert(this->is_func && this->offset == 0);//make sure functions offset is 0 .
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
    string get_function_parameters()
    {
        assert(this->is_func);//make sure this is func!, would crash otherwise
        string iterator = this->type;
        int position = iterator.find("->");
        if(position == NO_RETURN_TYPE) return EMPTY_STRING;
        else return iterator.substr(0,position);
    }
};