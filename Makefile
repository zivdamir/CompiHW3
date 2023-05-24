.PHONY: all clean
C = /home/zivdamir/compihw3/CompiHW3
all: clean
	flex scanner.lex
	bison -Wcounterexamples -d parser.ypp
	g++ -std=c++17 -o hw3 *.c *.cpp
clean:
	rm -f lex.yy.c
	rm -f parser.tab.*pp
	rm -f hw3
test:
	g++ -std=c++17 table_entry.hpp hw3_output.cpp symbol_table.hpp symbol_tables_stack.hpp main.cpp -o prog
	
	
	