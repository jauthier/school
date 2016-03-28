Assignment 3
~~~~~~~~~~~~~~~~~~~
CIS 3110
Mar. 25, 2016
Author: Jessica Authier, 0849720

~~~~~~~~~~~~~~~~~~~
References:
	
	In dine.c
		lines: 58-64, 93-100, 105, 107, 112, 114
		were insiped by https://docs.oracle.com/cd/E19205-01/820-0619/gepji/index.html
		
	In holes.c
		lines: 201-213
		was taken from my assignment 2, main.c
		
		lines: 268-287
		was taken from my assignment 2, getThreads.c
		
~~~~~~~~~~~~~~~~~~~
Compiling and Running:

	Dining Philosophers:
		to complie type 'make'.
		the make file looks like 'gcc -pthread -Wall -pedantic -std=c99 dine.c -o dine'
		
		to run type './dine <number of philosophers> <number of times they eat>'
		
	Memory Management:
		to compile type 'make'
		the make file looks like 'gcc -Wall -pedantic -std=c99 holes.c -o holes'
		
		to run type './holes <fileName>'
	
~~~~~~~~~~~~~~~~~~~
Limitations:

	The memory management program only impliments first fit 


