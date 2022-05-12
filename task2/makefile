all: exec

c-files: myshell.o LineParser.o MyPipeLine.o
	gcc -g -c -o myshell.o myshell.c
	gcc -g -c -o LineParser.o LineParser.c
	gcc -g -c -o MyPipeLine.o MyPipeLine.c

exec: c-files myshell.o LineParser.o MyPipeLine.o
	gcc -g myshell.o LineParser.o MyPipeLine.o -o myshell
	rm -f myshell.o LineParser.o MyPipeLine.o
	
.PHONY: clean
clean:
	rm -rf ./*.o main
