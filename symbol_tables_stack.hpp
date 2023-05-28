#ifndef COMPIHW3symbol_table_stack__
#define COMPIHW3symbol_table_stack__
#endif
#include "symbol_table.hpp"
#include <stack>
#include <iostream>
using namespace std;

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
        bool NameExist(string name)
        {
            symbol_table *table=tables.top();
            return table->contains(name);
        }
        void insert(string name, string type, bool is_func, bool is_override,int yylineno)
        {
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
                table->insert(name, type, offset, is_func, is_override);
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
                            table->insert(name, type, 0, is_func, is_override);
                        }
                    }
                }
                else
                {
                    table->insert(name, type, 0, is_func, is_override);
                }
            }
            
        }
        string getFuncReturnType(string name)
        {
            table_entry* func_entry = this->get_global_scope()->findByName(name);
            return func_entry->get_return_type();
        }
        string getType(string name)
        {
            table_entry* var_entry = this->top_scope()->findByName(name);
            return var_entry->getType();
        }
        bool nameExists(string name)
        {
            if(this->top_scope()->contains(name))
            {
                return true;
            }
            else{
                return false;
            }
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
        string getFunctionParamsTypes(string name)
        {
            table_entry* func_entry = this->findLastDefinedFunc(name);
            return func_entry->get_function_parameters_types();

        }
        table_entry* findLastDefinedFunc(string name)
        {
            return this->get_global_scope()->getLastDefinedInScope(name);
        }
        string getCurrentfunctionreturnType(string name)
        {
            table_entry* last_func = this->findCurrentFunc();
            assert(last_func);
            return last_func->get_return_type();
        }
        table_entry* findCurrentFunc()
        {
            return this->get_global_scope()->getLastEntry();
        }
};
