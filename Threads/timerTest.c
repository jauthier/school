#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>

int main(){

	clock_t start, current;
	time_t start1, end;
	start = clock();
	time(&start1);
	int count=0;





	while (clock() < 20000){
		wait();
	}
	current = clock();
	time(&end);
	double total1 = difftime(end, start1);
	double total = difftime(current, start);
	printf("%f, %f\n",total,total1);

	return 0;
}
