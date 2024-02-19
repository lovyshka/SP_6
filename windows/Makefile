all : subproc
	gcc windows_main.c -lm

subproc:
	gcc subproc.c -o subproc

clean:
	del input*.txt
	del output*.txt
	del .\a.exe .\subproc.exe