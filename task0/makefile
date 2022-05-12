all: exec

c-files: mypipe.o
	gcc -g -c -o mypipe.o mypipe.c

exec: c-files mypipe.o
	gcc -g mypipe.o -o mypipe
	rm -f mypipe.o
	
.PHONY: clean
clean:
	rm -rf ./*.o main
