#ifndef COMPIHW3symbol_table_stack__
#define COMPIHW3symbol_table_stack__
#endif
#include "symbol_table.hpp"

#include <stack>
#include <iostream>
#include <sstream>

class symbol_tables_stack{
    public:

        // symbol_table* glob_scope = new symbol_table(true);
        // tables.push(glob_scope);
        // offsets.push(0);
        stack<symbol_table*> tables;
        stack<int> offsets;
        bool is_in_while = false;
        symbol_tables_stack(){
            push_scope();
            // 1.do we want to init a global scope with te construction
            // 2. levi needs to undetrstands pop deletes the object!
            //  inWhile = false;
        }//init tables and offset
        void push_scope()
        {
            symbol_table *parent = nullptr;
            bool is_global = true;
            int offset = 0;
            if (!tables.empty())
            {
                assert(!offsets.empty());// we should require that offsets be not empty too!else we have a bug
                parent = this->tables.top();
                is_global = false;
                offset = offsets.top();
            }
            symbol_table *new_scope = new symbol_table(is_global, parent);
            if(!new_scope){
                assert(false);//bad allocation
            }
            this->tables.push(new_scope);
            this->offsets.push(offset);
        }
        void setInWhile(bool value)
        {
            tables.top()->set_is_in_while(value);
        }
        bool notInWhile()
        {
            return (tables.top()->get_in_while()==false);
        }
        void pop_scope(){
            output::endScope();
            this->tables.top()->printTable();
            // unless you want to make it super complicated, this stays this way!
            symbol_table *top_table = this->tables.top();
            this->tables.pop();
            this->offsets.pop();
            delete top_table;
            return;
        }
        symbol_table* top_scope()
        {
            return tables.top();
        }
        symbol_table* get_global_scope(){
            //returns the global_scope to find functions and global variables and such
            symbol_table* curr_table = tables.top();
            assert(curr_table != nullptr);
            while (curr_table->parent!=nullptr)
            {
                curr_table = curr_table->parent;
            }
            assert(curr_table != nullptr);
            return curr_table;
        }
        bool is_curr_scope_global(){
            assert(!this->tables.empty());
            return (this->tables.top()->parent == nullptr);
        }

        /*bool check_before_insert(string name, string ret_type, bool is_func, bool is_override,int yylineno)
        {
            symbol_table* table = this->top_scope();
            if( (!is_func) && (table->contains(name) == true) )
            {
                return output::errorDef(yylineno,name);
            }
            else if(table->contains(name))//if is_func == true
            {
                table_entry* identical_name_func_in_table = table->findByName(name);
                if (identical_name_func_in_table->isOverride()==false)
                {
                   output::errorFuncNoOverride(yylineno, name);
                }
                else if(!is_override)
                {
                     output::errorOverrideWithoutDeclaration(yylineno, name);
                }
            }
        }*/

        void insert(string name, string type, bool is_func, bool is_override,int yylineno)
        {
            assert(this->top_scope() != nullptr);
            if(is_override && !is_func){
                assert(false);
            }

            symbol_table* table = this->top_scope();
            if(!is_func)
            {
                if (table->contains(name) == false)
                {   
                    int offset = this->offsets.top();
                    this->offsets.pop();
                    table->insert(name, type, offset, is_func,is_override);
                    this->offsets.push(offset+1);
                }
                else{
                    output::errorDef(yylineno,name);
                }
            }
            else{ //if is_func == true
                if(table->contains(name)){
                    table_entry* identical_name_func_in_table = table->findByName(name);
                    if (identical_name_func_in_table->isOverride()==false)
                    {
                        output::errorFuncNoOverride(yylineno, name);
                    }
                    else if(!is_override)
                    {
                        output::errorOverrideWithoutDeclaration(yylineno, name);
                    }
                    else if(there exist a fucntion with the same name, return type & params)
                    {
                        output::errorDef(yylineno, name);
                    }
                    else {
                        table->insert(name, type, 0 ,is_func ,is_override);
                    }
                    }
                }
                else
                {
                    table->insert(name, type, 0, is_func ,is_override);
                }
            }
           // printf("insert end\n"); 
        }
        void symbol_table_add_function_parameter_entries(std::string parameter_names, std::string type, int yylineno)
        {
            std::stringstream param_names_stream(parameter_names);
            std::stringstream param_types_stream(type);
            std::string segment;
            
            
            std::vector<std::string> names_vec;
            while(std::getline(param_names_stream, segment, ','))
            {
                names_vec.push_back(segment);
            }
            std::vector<std::string> types_vec;
            
            while(std::getline(param_types_stream, segment, ','))
            {
                types_vec.push_back(segment);
            }
            
            for(int param = 0; param < names_vec.size(); param++)
            {
                if(this->top_scope()->contains(names_vec.at(param))) output::errorDef(yylineno, names_vec.at(param));
                this->top_scope()->insert(names_vec.at(param), types_vec.at(param), -(param+1), false, false);
            }
            
        }
        
        string getType(string name)
        {
            table_entry* var_entry = this->top_scope()->findByName(name);
            return var_entry->getType();
        }
        bool nameExists(string name)
        {
            return this->top_scope()->contains(name);
        }
        bool isFunc(string name)
        {
            table_entry* var_entry = this->top_scope()->findByName(name);
            return var_entry->isFunc();
        }
        bool inWhileLoop()
        {
            if(is_in_while) 
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        void updateInWhile(bool in_while)
        {
            this->is_in_while = in_while;
        }

        table_entry* findLastDefinedFunc(string name)
        {
            return this->get_global_scope()->getLastDefinedInScope(name);
        }
        table_entry* findCurrentFunc()
        {
            return this->get_global_scope()->getLastEntry();
        }
        int numOfFuncExist(const string& name,const string& returnType,const string& parameters,bool exactly_the_same)
        {
            int func_count = 0;
            //bool exists = false;
            std::vector<table_entry *> &entries = this->get_global_scope()->entries;
            if(!entries.empty())
            {
                for(table_entry* entry : entries){
                    bool same_parameters = is_comparable_parameter_list(entry->get_function_parameters_types(), parameters, exactly_the_same);
                    bool same_retType = is_desired_return_type(entry->get_return_type(), returnType, exactly_the_same);
                    bool same_name = (entry->name == name);
                    if(same_parameters&&same_retType&&same_name)
                    {
                        func_count++;
                    }
                }
            }
            return func_count;
        }
        bool funcExist(const string& name,const string& returnType,const string& parameters,bool exactly_the_same)
        {
            return numOfFuncExist(name, returnType, parameters, exactly_the_same) >= 1;
        }
        bool is_comparable_parameter_list(const string& parameters1, const string& parameters2, bool exactly_the_same)
        {
            //if exactly_the_same=false
            //byte and int are comparable, but string and bool aren't.
            //bool and int aren't, but bool and bool are.
            //exactly_the_same =true, compare int to int only.

            if(exactly_the_same==true){
                return parameters1 == parameters2;
            }
            else
            {
                
                if(parameters1==parameters2){
                    return true;
                }
                int num_of_parameters1 = _count_parameters(parameters1);
                int num_of_parameters2 = _count_parameters(parameters2);
                if(num_of_parameters1!=num_of_parameters2)
                {
                    return false;
                }

            std::stringstream params_1_iterator(parameters1);//csv'd
            std::stringstream params_2_iterator(parameters2);
            bool is_the_same = true;
            string curr_type_parameters1;
            string curr_type_parameters2;
            while (std::getline(params_1_iterator, curr_type_parameters1, ',') 
            && std::getline(params_2_iterator, curr_type_parameters2, ','))
            {
               is_the_same =is_the_same && is_same_type(curr_type_parameters1, curr_type_parameters2);
            }
            return is_the_same;
            }
        }
        int _count_parameters(const string& parameters){
            int count = 0;
            if (parameters == "")
            {
                return count;
            }
            else{
                count = 1;
                for (char c : parameters)
                {
                    if(c==',')
                    {
                        count++;
                    }
                }
            }
            return count;
        }
        bool is_same_type(const string& type1,const string& type2)
        {
            return (type1 == type2) ? true : (type1 == "INT" && type2 == "BYTE");
        }
        bool is_desired_return_type(const string& return_type1,const string& return_type2,bool exactly_the_same){
            return exactly_the_same ? return_type1 == return_type2 : is_same_type(return_type1, return_type2);
        }
        /**these functions assume the function does exist**/
        string getFunctionreturnType(string name)
        {
            table_entry* func_entry = this->findLastDefinedFunc(name);
            return func_entry->get_return_type();
        }
        string getCurrentfunctionreturnType()
        {
            table_entry* last_func = this->findCurrentFunc();
            assert(last_func);
            return last_func->get_return_type();
            //return last_func->get_return_type();
        }
        string getFunctionParamsTypes(string name)
        {
            table_entry* func_entry = this->findLastDefinedFunc(name);
            return func_entry->get_function_parameters_types();
        }
        table_entry* findFunc(const string& name,const string& returnType,const string& parameters,bool exactly_the_same){
            table_entry *func = nullptr;
            std::vector<table_entry *> &entries = this->get_global_scope()->entries;
            if(!entries.empty())
            {
                for(table_entry* entry : entries){
                    bool same_parameters = is_comparable_parameter_list(entry->get_function_parameters_types(), parameters, exactly_the_same);
                    bool same_retType = is_desired_return_type(entry->get_return_type(), returnType, exactly_the_same);
                    bool same_name = (entry->name == name);
                    if(same_parameters&&same_retType&&same_name)
                    {
                        func = entry;
                        break;
                    }
                }
            }
            return func;
        }
        void validateMainFunction()
        {
            
            string name = "main";
            table_entry* main_func = this->findLastDefinedFunc(name);
            /*cout<<"hello leviiii"<<endl;
            cout<<"getFunctionParamsTypes(name) : "<<getFunctionParamsTypes(name)<<endl;
            cout<<"getFunctionreturnType(name) : "<<getFunctionreturnType(name)<<endl;
            cout<<"main_func equals nullptr?: "<< (main_func==nullptr) <<endl;
            cout<<"hello leviiii"<<endl;*/
            if(main_func == nullptr || (getFunctionParamsTypes(name)!= "")
            || getFunctionreturnType(name) != "VOID")
            {
                output::errorMainMissing();
            }
        }
};