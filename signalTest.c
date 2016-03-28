#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void sigHandler (int signum, siginfo_t* signalInfo, void* hold){

}

int main (){

	struct sigaction action;
	action.sa_sigaction = sigHandler;

	



	if (sigaction())


	return 0;
}
