Assignment One
-----------------------------

CIS 3110
Feb. 12, 2016

Author: Jessica Authier, 0849720

References:
	
	In main.c
		lines: 78-80
		learned siganls from 
		http://stackoverflow.com/questions/14548367/using-waitpid-to-run-process-in-background
		and signalExample.c, example code from class
	
	In execute.c
		lines: 15-20
		from signalExample.c, example code from class
		
		lines: 103,119-120
		from http://stackoverflow.com/questions/28502305/writing-a-simple-shell-in-c-using-fork-execvp
	
	In history.c
		lines: 65-67, 71
		learned strtol from
		http://www.tutorialspoint.com/c_standard_library/c_function_strtol.htm
	
	In redirection.c
		lines: 34, 55-59 and 63
		I learned how to use freopen from 
		http://www.tutorialspoint.com/c_standard_library/c_function_freopen.htm

	In piping.c
		lines: 23, 50-54, 64-69 and 76-78
		I learned how to use pipe and dup2 from
		https://www.youtube.com/watch?v=uHH7nHkgZ4w,
		https://www.youtube.com/watch?v=EqndHT606Tw,
		http://linux.die.net/man/2/pipe and 
		http://tldp.org/LDP/lpg/node11.html
		
-----------------------------

compiling and running
~~~~~~~~~~~~~~~~~~~~~

	In my makefile there are two options, 'make' which has all the flags,
	and 'make noC99' which has all the flags but -std=c99. 
	"make noC99" exists because my code will not compile with -std=c99 due
	to an error with siginfo_t "error: unknown type name 'siginfo_t'". I do 
	not get these errors when I compile with just -Wall and -pedantic.
	
	to compile type 'make noC99' 
	to run type './CIS3110sh'
	
-------------------------------

Commands That Don't Work
~~~~~~~~~~~~~~~~~~~~~~~~

	anything with PATH
	multiple operations(<,>,|)
	export
	

