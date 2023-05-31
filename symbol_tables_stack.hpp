#ifndef COMPIHW3symbol_table_stack__
#define COMPIHW3symbol_table_stack__
#endif
#include "symbol_table.hpp"

#include <stack>
#include <iostream>
#include <sstream>

class symbol_tables_stack{
    private:
        /*inner functions*/
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
        int numOfFuncExist(const string& name,const string& parameters,bool exactly_the_same)//,const string& returnType
        {
            int func_count = 0;
            //bool exists = false;
            std::vector<table_entry *> &entries = this->get_global_scope()->entries;
            if(!entries.empty())
            {
                for(table_entry* entry : entries){
                    bool same_parameters = is_comparable_parameter_list(entry->get_function_parameters_types(), parameters, exactly_the_same);
                    //bool same_retType = is_desired_return_type(entry->get_return_type(), returnType, exactly_the_same);
                    bool same_name = (entry->name == name);
                    if(same_parameters&&same_name)//&&same_retType
                    {
                        func_count++;
                    }
                }
            }
            return func_count;
        }
        symbol_table* get_global_scope(){
            symbol_table* curr_table = tables.top();
            assert(curr_table != nullptr);
            while (curr_table->parent!=nullptr)
            {
                curr_table = curr_table->parent;
            }
            assert(curr_table != nullptr);
            return curr_table;
        }
        bool is_desired_return_type(const string& return_type1,const string& return_type2,bool exactly_the_same){
            return exactly_the_same ? return_type1 == return_type2 : is_same_type(return_type1, return_type2);
        }

    public:
        /*Class Variables*/
        stack<symbol_table*> tables;
        stack<int> offsets;
        bool is_in_while = false;
        
        /*Init*/
        symbol_tables_stack(){
            push_scope();
        }

        /*Stack Methods*/
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
                    //cout << "not a func" << endl;
                    output::errorDef(yylineno, name);
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
                    else 
                    if(funcExists(identical_name_func_in_table->name,
                    identical_name_func_in_table->get_function_parameters_types(),
                    false
                    ))
                    {
                        //cout << "func but no override" << endl;
                        output::errorDef(yylineno, name);
                    }
                    else {
                        table->insert(name, type, 0 ,is_func ,is_override);
                    }
                    }
                
                else
                {
                    table->insert(name, type, 0, is_func ,is_override);
                }
            }
        }

        /*While Loop Declarer*/
        void updateInWhile(bool in_while)
        {
            this->is_in_while = in_while;
        }
        
        /*Adding Additional Information For Function Declarations*/
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
                if(this->top_scope()->contains(names_vec.at(param)))
                {
                    //cout << "add function parameters:" <<parameter_names<<" "<<type<< endl;
                    output::errorDef(yylineno, names_vec.at(param));
                }
                this->top_scope()->insert(names_vec.at(param), types_vec.at(param), -(param + 1), false, false);
            }
            
        }

        /*getters*/
        string getType(string name)
        {
            table_entry* var_entry = this->top_scope()->findByName(name);
            return var_entry->getType();
        }
        /**getters: these functions assume the function does exist**/
        string getFunctionreturnType(string name)
        {
            table_entry* func_entry = this->findFunc(name);
            return func_entry->get_return_type();
        }
        string getCurrentfunctionreturnType()
        {
            table_entry* last_func = this->get_global_scope()->getLastEntry();
            assert(last_func);
            return last_func->get_return_type();
            //return last_func->get_return_type();
        }
        string getFunctionParamsTypes(string name)
        {
            table_entry* func_entry = this->findFunc(name);
            return func_entry->get_function_parameters_types();
        }
        string getFuncReturnType(const string& func_name,const string& parameters)
        {
            table_entry *func = this->findFunc(func_name, parameters, false, false);
            if(func == nullptr)
            {
            assert(false);
            }
            return func->get_return_type();
        }
        

        table_entry* findFunc(const string& name,const string& parameters="",bool exactly_the_same=false,bool search_name_only=true)//,const string& returnType=""
        {

            table_entry *func = nullptr;
            std::vector<table_entry *> &entries = this->get_global_scope()->entries;
            if(search_name_only==true)
            {
                func =  this->get_global_scope()->getLastDefinedInScope(name);

            }
            else{
           // cout << "for name: " << name << "parameters are(for debugging,comment please, symbol_tables_stack.hpp line 331~): " << parameters << endl;
            if(!entries.empty())
            {
                for(table_entry* entry : entries){
                    bool same_parameters = is_comparable_parameter_list(entry->get_function_parameters_types(), parameters, exactly_the_same);
                    //bool same_retType = is_desired_return_type(entry->get_return_type(), exactly_the_same);
                    bool same_name = (entry->name == name);
                    if(same_parameters&&same_name)//&&same_retType
                    {
                        func = entry;
                        break;
                    }
                }
            }
            }
            return func;
        }
        
        /*boolean checks*/
        //function Q
        bool funcExists(const string& name,const string& parameters,bool exactly_the_same)//const string& returnType
        {
            return numOfFuncExist(name, parameters, exactly_the_same) >= 1;
        }
        bool isFunc(string name)
        {
            table_entry* var_entry = this->top_scope()->findByName(name);
            return var_entry->isFunc();
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
        
        
        //variables Q
        bool is_same_type(const string& type1,const string& type2)
        {
            return (type1 == type2) ? true : (type1 == "INT" && type2 == "BYTE");
        }
        bool nameExists(string name)
        {
            return this->top_scope()->contains(name);
        }

        //scope Q
        bool is_curr_scope_global(){
            assert(!this->tables.empty());
            return (this->tables.top()->parent == nullptr);
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

        /*other*/
        void validateCall(const string& name, const string& params,int yylineno)
        {
            if(numOfFuncExist(name, params, false) == 0)
            {
                cout << "validate call" << endl;
                output::errorUndefFunc(yylineno, name);
            }
            else if(numOfFuncExist(name, params, false) > 1)
            {
                output::errorAmbiguousCall(yylineno, name);
            }
        }
        void validateMainFunction()
        {
            
            string name = "main";
            table_entry* main_func = this->findFunc(name);
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
        void validateId(const string& name, int yylineno)
        {
            cout << "validateId" << endl;
            if ((!nameExists(name)) || isFunc(name))
                output::errorUndef(yylineno, name);
        }
};