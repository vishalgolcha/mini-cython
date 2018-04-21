symbol-table: symbol-table.c symboltable.h hash-helper.o ast.o 
	gcc symbol-table.c lex.o llh.o extgrm.o ptree.o ast.o hash-helper.o -o stout
	
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
	rm *.o stage1exe
test1 : compileAll
	./stage1exe testcase1.txt result1.txt
test2 : compileAll
	./stage1exe testcase2.txt result2.txt
test3 : compileAll
	./stage1exe testcase3.txt result3.txt
test4 : compileAll
	./stage1exe testcase4.txt result4.txt
test5 : compileAll
	./stage1exe testcase5.txt result5.txt

