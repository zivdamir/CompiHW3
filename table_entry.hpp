#ifndef COMPIHW3table_entry__
#define COMPIHW3table_entry__
#endif
//this class/struct remsembles an entry in symbol table.
#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include "assert.h"
#include "hw3_output.hpp"
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
    bool is_override;
    table_entry(string name, int offset, string type ,bool is_func,bool is_override)
        : name(name), offset(offset), type(type),is_func(is_func), is_override(is_override) {
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
            assert(position != std::string::npos);
            return iterator.substr(position+2,length);
        }
    }
    string get_function_parameters_types()
    {
        
        //printf("the entry in get_function_parameters_types:\n this->name:%s\n this->type: %s\n,offset:%d\n this->is_func:%d\n this->is_override:%d\nthis->is_func: %d\n",
        //this->name.data(),this->type.data(), this->offset, this->is_func,this->is_override, this->is_func);
        assert(this->is_func);//make sure this is func!, would crash otherwise
        string iterator = this->type;
        int position = iterator.find("->");
        if(position == std::string::npos) return EMPTY_STRING;
        else 
        {
            //printf("get_function_parameters_types returns: %s\n",iterator.substr(0,position).data());
            return iterator.substr(0,position);
        }
    }
    bool isOverride()
    {
        return this->is_override;
    }
    string getType()
    {
        return this->type;
    }
    bool isFunc()
    {
        return is_func;
    }
    
    vector<string> fill_vector_func_params()
    {
        vector<string> types;
        std::stringstream param_names_stream(this->get_function_parameters_types());
        string segment;
                
        while(std::getline(param_names_stream, segment, ','))
            {
              types.push_back(segment);
            }
        return types;
    }

};