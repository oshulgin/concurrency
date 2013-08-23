#include <pthread.h>
#include <stdio.h>
#include <string.h>


void* threadFunc(void *arg) {
	char* message = (char*)arg;
	printf("%s", message);

	return (void*)strlen(message);
}

int main(int argc, char *argv[]) {
	pthread_t t1, t2;
	void* res;
	int s;

	printf("Message from main() #1\n");

	//pthread_join(pthread_self(), NULL);

	s = pthread_create(&t1, NULL, threadFunc, (void*)"Message #1\n");
	s = pthread_create(&t2, NULL, threadFunc, (void*)"Message #2\n");

	printf("Message from main() #2\n");

	if ( t1 != pthread_self() ) {
		s = pthread_join(t1, &res);
	}
	if ( t2 != pthread_self() ) {
		s = pthread_join(t2, &res);
	}

	printf("Thread returned %ld\n", (long)res);

	return 0;
}
