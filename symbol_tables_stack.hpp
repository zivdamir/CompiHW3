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
        symbol_tables_stack():tables(),offsets(){
            push_scope();//init global scope
            //1.do we want to init a global scope with te construction
            //2. levi needs to undetrstands pop deletes the object!
            // inWhile = false;
        }//init tables and offset

        void push_scope(){
            symbol_table *parent= nullptr;
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
            return;
        }
        void pop_scope(){
            symbol_table *top_table = this->tables.top();
            this->tables.pop();
            this->offsets.pop();
            delete top_table;
            return;
        }
        symbol_table* top_scope()
        {
            return this->tables.top();
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
        void insert(string name,string type,bool is_func)
        {
            
        }
};