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
        void insert(string name, string type, bool is_func, bool is_override,int yylineno)
        {
            //cout<<"insert input type:" <<type<<endl;
            //printf("insert begining\n");
            assert(this->top_scope() != nullptr);
            symbol_table* table = this->top_scope();
            if(is_override && !is_func){
                assert(false);
            }
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
                    else
                    {
                        if(!is_override)
                        {
                            output::errorOverrideWithoutDeclaration(yylineno, name);
                        }
                        else{
                            table->insert(name, type, 0 ,is_func ,is_override);
                        }
                    }
                }
                else
                {
                    table->insert(name, type, 0,is_func ,is_override);
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