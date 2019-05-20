# DIY Compiler
Cython like mini compiler for the Compilers Course Project
```makefile
compiler: final_driver.c symbol-table.o hash-helper.o ast.o lex.o llh.o extgrm.o ptree.o driver.c
	gcc final_driver.c symbol-table.o lex.o llh.o extgrm.o ptree.o ast.o hash-helper.o -o compiler

symbol-table: symbol-table.c symboltable.h hash-helper.o ast.o hash-helper.c lex.o llh.o extgrm.o ptree.o
	gcc -c symbol-table.c -o symbol-table.o
	
hash-helper.o: hash-helper.c hash-helper.h astDefs.h lex.o llh.o extgrm.o ptree.o
	gcc -c hash-helper.c -o hash-helper.o
	
ast.o: ast.c astDefs.h lex.o llh.o extgrm.o ptree.o
	gcc -c ast.c -o ast.o

compileAll :lex.o llh.o extgrm.o ptree.o driver.c
	gcc lex.o llh.o extgrm.o ptree.o  driver.c -o stage1exe

ptree.o : lexer.h lexerDef.h parser.h parserDef.h lexer.c parser.c ll-helper.c parse-tree.c extgrm.o lex.o llh.o
	gcc -c parse-tree.c -o ptree.o


extgrm.o :parser.h parserDef.h parser.c
	gcc -c parser.c -o extgrm.o

lex.o :lexer.h lexerDef.h lexer.c ll-helper.c  llh.o
	gcc -c lexer.c -o lex.o

llh.o :ll-helper.c lexer.h lexerDef.h
	gcc -c ll-helper.c -o llh.o

clean :
rm *.o compiler
```
