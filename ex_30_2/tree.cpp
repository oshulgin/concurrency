#include "tree.h"
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <string>


using namespace std;


//---------------------------------Node definition-------------------------------

Node::Node(const string& key, void* value) : key(key), value(value), left(NULL), right(NULL) {
	int s = pthread_mutex_init(&mutex, NULL);
	if ( s != 0 ) {
		cerr << "Mutex_init_error" << endl;
	}
}

Node::~Node() {
    int s = pthread_mutex_destroy(&mutex);
	if ( s != 0 ) {
		cerr << "Mutex_destroy_error" << endl;
	}
}
//---------------------------------Tree definition-------------------------------

void Tree::add(Node** node, const string& k, void* v) {

	if( *node == NULL ) {
		*node = new Node(k, v);
		return;
	}

	int s = pthread_mutex_lock(&((*node)->mutex));
	if (s != 0) {
		cerr << "Adding: mutex_lock_error";
	}

	if (k == (*node)->key) {
		(*node)->value = v;
		int s = pthread_mutex_unlock(&((*node)->mutex));
		if (s != 0) {
			cerr << "Adding: mutex_unlock_error";
		}
		return;
	}

	if (k > (*node)->key) {
		int s = pthread_mutex_unlock(&((*node)->mutex));
		if (s != 0) {
			cerr << "Adding: mutex_unlock_error";
		}
		add(&((*node)->right), k, v);
	}

	if (k < (*node)->key) {
		int s = pthread_mutex_unlock(&((*node)->mutex));
		if (s != 0) {
			cerr << "Adding: mutex_unlock_error";
		}
		add(&((*node)->left), k, v);
	}
}

void* Tree::search(Node* node, const std::string& k) {

	if ( node == NULL ) {
		return NULL;
	}

	int s = pthread_mutex_lock(&(node->mutex));
	if (s != 0) {
		cerr << "Searching: mutex_lock_error\n";
	}

	if (k == node->key) {
		int s = pthread_mutex_unlock(&(node->mutex));
		if (s != 0) {
			cerr << "Searching: mutex_unlock_error\n";
		}
		return node->value; }
	else {
		if (k < node->key) {
			int s = pthread_mutex_unlock(&(node->mutex));
			if (s != 0) {
				cerr << "Searching: mutex_unlock_error\n";
			}
			return search(node->left, k);
		}
		else {
			int s = pthread_mutex_unlock(&(node->mutex));
			if (s != 0) {
				cerr << "Searching: mutex_unlock_error\n";
			}
			return search(node->right, k);
		}
	}
}

void Tree::del(Node* node, const string& k) {
	if(node == NULL) { return; }

	int s = pthread_mutex_lock(&(node->mutex));
	if (s != 0) {
		cerr << "Deleting: mutex_lock_error\n";
	}

	if(k == node->key) {
		Node* tmp;
		if(node->left == NULL) {
			tmp = node->right;
			delete node;
			node = tmp;
			return;
		} else if ( node->right == NULL ) {
			tmp = node->left;
			delete node;
			node = tmp;
			return;
		}
		else {
			Node* min = getMin(node->left);
			node->key = min->key;
			del(min, min->key);
		}
	} else if(k < node->key) {
		int s = pthread_mutex_unlock(&(node->mutex));
		if (s != 0) {
			cerr << "Deleting: mutex_unlock_error\n";
		}
		del(node->left, k);
	} else {
		int s = pthread_mutex_unlock(&(node->mutex));
		if (s != 0) {
			cerr << "Deleting: mutex_unlock_error\n";
		}
		del(node->right, k);
	}
	s = pthread_mutex_unlock(&(node->mutex));
	if (s != 0) {
		cerr << "Deleting: mutex_unlock_error\n";
	}
}

Node* Tree::getMin(Node* node) {
	while(node->left != NULL)
		node = node->left;
	return node;
}

//---------------------------------General functions-------------------------------
void initialize(Tree& tree) {
	tree.root = NULL;
}

void add(Tree& tree, const std::string& key, void* value) {
	tree.add(&(tree.root), key, value);
}

void del(Tree& tree, const std::string& key) {
	tree.del(tree.root, key);
}

void* search(Tree& tree, const std::string& k) {
	return tree.search(tree.root, k);
}


