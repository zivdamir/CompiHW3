#include <iostream>


#include "symbol_tables_stack.hpp"
using namespace std;
int main()
{
    table_entry entry = table_entry("ziv", 0, "int", false);
    table_entry entry2 = table_entry("ziv2", 1, "int", false);
    table_entry entry3 = table_entry("ziv3", 2, "int", false);
    table_entry entry4 = table_entry("ziv4", 0, "int->int", true);
    symbol_table *table_2 = symbol_tables_stack::Maketable(nullptr, false);
    table_2->insert(entry.name, entry.type, entry.offset, false, false,1);
    table_2->insert(entry2.name, entry2.type, entry2.offset, false, false,5);
    table_2->insert(entry3.name, entry3.type, entry3.offset, false, false,5);
    table_2->insert(entry4.name, entry4.type, entry4.offset, false, false,5);
    table_2->insert(entry.name, entry.type, entry4.offset, false, false,5);

    symbol_tables_stack s = symbol_tables_stack();
    s.push(s.tables, table_2);
    // table.get_return_type
    cout << s.top(s.tables)->contains(entry.name,entry.type) << endl;
    cout << s.top(s.offsets) << endl;
    int a = s.top(s.offsets);
    a += 2;
    cout << s.top(s.offsets) << endl;
    return 0;
}