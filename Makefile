.PHONY: all clean
all: clean
	flex scanner.lex
	bison  -d parser.ypp
	g++ -std=c++17 *.c *.cpp -o final.out
clean:
	rm -f lex.yy.c
	rm -f parser.tab.*pp
	rm -f final.out
	
	
	
