#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <map>
#include <time.h>
#include <stdio.h>

#include "tree.h"

using namespace std;

const int NUMBER_OF_NODES = 30;
const int NUMBER_OF_THREADS = 10;
const int KEY_LEN = 5;
Tree myTree;

string keyArray[NUMBER_OF_NODES];
void* valueArray[NUMBER_OF_NODES];

string gen_random_key();
void* gen_random_value();
void* fill_tree(void* arg);
void* show_elements(void* arg);
void* del_element(void* arg);



int main() {
	int s = 0;
	srand (time(NULL));
	initialize(myTree);
	pthread_t thread[NUMBER_OF_THREADS];
	int argList[NUMBER_OF_NODES];
	for ( int i = 0; i < NUMBER_OF_NODES; i++ ) {
		argList[i] = i;
	}

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
    for ( int i = 0; i < NUMBER_OF_THREADS; i++ ) {
    	cout << "main_fill: " << "number of thread: " << i << endl;
    	s = pthread_create(thread + i, NULL, fill_tree, &argList[i * NUMBER_OF_NODES / NUMBER_OF_THREADS]);
    	if ( s != 0 ) {
    		cerr << "Create thread error" << endl;
    	}
    }

    for ( int i = 0; i < NUMBER_OF_THREADS; i++ ) {
    	s = pthread_join(thread[i], NULL);
    	if ( s != 0 ) {
    		cerr << "Join thread error" << endl;
    	}
    }

    //searching every value from array in a tree before deleting
    cout << "-----------tree before deleting-------------" << endl;
    for ( int i = 0; i < NUMBER_OF_THREADS; i++ ) {
    	cout << "main_show_before_del: " << "number of thread: " << i << endl;
    	s = pthread_create(thread + i, NULL, show_elements, &argList[i * NUMBER_OF_NODES / NUMBER_OF_THREADS]);
    	if ( s != 0 ) {
    		cerr << "Create thread error" << endl;
    	}
    }

    for ( int i = 0; i < NUMBER_OF_THREADS; i++ ) {
    	s = pthread_join(thread[i], NULL);
    	if ( s != 0 ) {
    		cerr << "Join thread error" << endl;
    	}
    }

//    //Delete every third value
//    for ( int i = 0; i < NUMBER_OF_NODES; i += 3 ) {
//     	del(myTree, keyArray[i]);
//     }

    //Delete all elements
    for ( int i = 0; i < NUMBER_OF_THREADS; i++ ) {
    	cout << "main_del: " << "number of thread: " << i << endl;
    	s = pthread_create(thread + i, NULL, del_element, &argList[i * NUMBER_OF_NODES / NUMBER_OF_THREADS]);
    	if ( s != 0 ) {
    		cerr << "Create thread error" << endl;
    	}
    }

    for ( int i = 0; i < NUMBER_OF_THREADS; i++ ) {
    	s = pthread_join(thread[i], NULL);
    	if ( s != 0 ) {
    		cerr << "Join thread error" << endl;
    	}
    }

    cout << "-----------tree after deleting every third element-------------" << endl;
    for ( int i = 0; i < NUMBER_OF_THREADS; i++ ) {
    	cout << "main_show_after_del: " << "number of thread: " << i << endl;
    	s = pthread_create(thread + i, NULL, show_elements, &argList[i * NUMBER_OF_NODES / NUMBER_OF_THREADS]);
    	if ( s != 0 ) {
    		cerr << "Create thread error" << endl;
    	}
    }

    for ( int i = 0; i < NUMBER_OF_THREADS; i++ ) {
    	s = pthread_join(thread[i], NULL);
    	if ( s != 0 ) {
    		cerr << "Join thread error" << endl;
    	}
    }
    cout << "End!!!" << endl;
	exit(EXIT_SUCCESS);
}


/****************Functions****************************/
/*****************************************************/

void* fill_tree(void* arg) {
	int node_number = *((int*)arg);
	//cout << "In fill function: " << endl;
	for ( int i = 0; i < (NUMBER_OF_NODES / NUMBER_OF_THREADS); i++ ) {
		//cout << "addind node #" << node_number + i << endl;
		//cout << "Adding: " << keyArray[node_number + i] << endl;
		add(myTree, keyArray[node_number + i], valueArray[node_number + i]);
	}
	return NULL;
}

void* show_elements(void* arg) {
	int node_number = *((int*)arg);
//	cout << "In show function: " << endl;
	for ( int i = 0; i < (NUMBER_OF_NODES / NUMBER_OF_THREADS); i++ ) {
//		cout << "searching node #" << node_number + i << endl;
//		cout << "searching: " << keyArray[node_number + i] << endl;
		if ( search(myTree, keyArray[i]) ) {
			cout << *((int*)search(myTree, keyArray[i])) << endl;
		}
	}
	return NULL;
}

void* del_element(void* arg) {
	int node_number = *((int*)arg);
	//cout << "In fill function: " << endl;
	for ( int i = 0; i < (NUMBER_OF_NODES / NUMBER_OF_THREADS); i++ ) {
		//cout << "Delete node #" << node_number + i << endl;
		//cout << "Deleting: " << keyArray[node_number + i] << endl;
		del(myTree, keyArray[node_number + i]);
	}
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
