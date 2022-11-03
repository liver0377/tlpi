#include<signal.h>
#include<stddef.h>

int siginterrupt(int sig, int flag){
	struct sigaction sa;

	if(sigaction(sig, NULL, &sa) == -1) return -1;

	if(flag == 0)
	    sa.sa_flags |= SA_RESTART;
	else
	    sa.sa_flags &= ~SA_RESTART;

	if(sigaction(sig, &sa, NULL) == -1) return -1;

	return 0;
}

int main(int argc, char **argv){
	return 0;
}
