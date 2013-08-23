#include <pthread.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

using namespace std;

static int glob = 0;
static ofstream output;

struct ThreadInfo {
	int loops;
};

static void* threadFunc(void *arg) {
    int loops = ((ThreadInfo *) arg)->loops;
    int loc, j;

    for (j = 0; j < loops; j++) {
    	output << "Glob: " << glob << " , thread Id: " << pthread_self() << endl;
        loc = glob;
        loc++;
        glob = loc;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    pthread_t t1, t2;
    ThreadInfo info;
    int s;
    output.open("log");

    info.loops =  (argc > 1) ? atoi(argv[1]) : 100000;

    s = pthread_create(&t1, NULL, threadFunc, &info);
    if (s != 0)
        cerr << "pthread_create";
    s = pthread_create(&t2, NULL, threadFunc, &info);
    if (s != 0)
        cerr << "pthread_create";

    s = pthread_join(t1, NULL);
    if (s != 0)
        cerr << "pthread_join";
    s = pthread_join(t2, NULL);
    if (s != 0)
        cerr << "pthread_join";

    printf("glob = %d\n", glob);

    output.close();
    exit(EXIT_SUCCESS);
}
