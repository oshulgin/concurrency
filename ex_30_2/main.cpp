#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <time.h>
#include <stdio.h>

#include "tree.h"

using namespace std;

const int NUMBER_OF_NODES = 10;
const int NUMBER_OF_THREADS = 3;
const int KEY_LEN = 5;
Tree myTree;

string keyArray[NUMBER_OF_NODES];
void* valueArray[NUMBER_OF_NODES];

string gen_random_key();
void* gen_random_value();
void* fill_tree(void* arg);



int main() {
	int s = 0;
	srand (time(NULL));
	initialize(myTree);
	pthread_t thread[NUMBER_OF_THREADS];

	 //generating arrays randomly
    for (int i = 0; i < NUMBER_OF_NODES; ++i) {
    	keyArray[i] = gen_random_key();
    	valueArray[i] = gen_random_value();
    }

    //printing arrays:  (key, value, address)
    for ( int i = 0; i < NUMBER_OF_NODES; i++ ) {
    	cout << keyArray[i] << " - " << *(int*)(valueArray[i]) << " - " << valueArray[i] << endl;
    }

    //filling tree
    for ( int i = 0; i < NUMBER_OF_NODES; i++ ) {
    	cout << "main: " << i << " - loop; " << "number of thread: " << (i % NUMBER_OF_THREADS) << endl;
    	s = pthread_create(&(thread[i % NUMBER_OF_THREADS]), NULL, fill_tree, &i);
    	if ( s != 0 ) {
    		cerr << "Create thread error" << endl;
    	}
    }

//    searching every value from array in a tree before deleting
    cout << "-----------tree before deleting-------------" << endl;
    for ( int i = 0; i < NUMBER_OF_NODES; i++ ) {
     	if ( search(myTree, keyArray[i]) ) {
     		cout << *((int*)search(myTree, keyArray[i])) << endl;
     	}
     }

    //Delete every third value
    for ( int i = 0; i < NUMBER_OF_NODES; i += 3 ) {
     	del(myTree, keyArray[i]);
     }

    cout << "-----------tree after deleting every third element-------------" << endl;
    for ( int i = 0; i < NUMBER_OF_NODES; i++ ) {
     	if ( search(myTree, keyArray[i]) ) {
     		cout << *((int*)search(myTree, keyArray[i])) << endl;
     	}
     }
    cout << "End!!!" << endl;
	exit(EXIT_SUCCESS);
}

void* fill_tree(void* arg) {
	cout << "In fill function" << endl;
	add(myTree, keyArray[*((int*)arg)], valueArray[*((int*)arg)]);
	return NULL;
}

string gen_random_key() {
	string str = "";
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    for (int i = 0; i < KEY_LEN; ++i) {
        str += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return str;
}

void* gen_random_value() {
	return new int(rand() % 1000 + 1);
}
