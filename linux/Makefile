all: 
	gcc linux_main.c -lm -lpthread -g -O0

clean:
	rm -rf subproc a.out
	rm -rf input*.txt output*.txt

rebuild: clean all


test: 
	gcc subproc.c -o subproc

