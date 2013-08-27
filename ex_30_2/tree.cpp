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
		cerr << "mutex_lock_error";
	}

	if (k == (*node)->key) {
		(*node)->value = v;
		int s = pthread_mutex_unlock(&((*node)->mutex));
		if (s != 0) {
			cerr << "mutex_lock_error";
		}
		return;
	}

	if (k > (*node)->key) {
		int s = pthread_mutex_unlock(&((*node)->mutex));
		if (s != 0) {
			cerr << "mutex_lock_error";
		}
		add(&((*node)->right), k, v);
	}

	if (k < (*node)->key) {
		int s = pthread_mutex_unlock(&((*node)->mutex));
		if (s != 0) {
			cerr << "mutex_lock_error";
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
		cerr << "mutex_lock_error\n";
	}

	if (k == node->key) {
		int s = pthread_mutex_unlock(&(node->mutex));
		if (s != 0) {
			cerr << "mutex_unlock_error\n";
		}
		return node->value; }
	else {
		if (k < node->key) {
			int s = pthread_mutex_unlock(&(node->mutex));
			if (s != 0) {
				cerr << "mutex_unlock_error\n";
			}
			return search(node->left, k);
		}
		else {
			int s = pthread_mutex_unlock(&(node->mutex));
			if (s != 0) {
				cerr << "mutex_unlock_error\n";
			}
			return search(node->right, k);
		}
	}
}

void Tree::del(const string& k) {
    //Locate the element
    bool found = false;
    if( isEmpty() ) {
        cout << " This Tree is empty! " << endl;
        return;
    }

    Node* curr;
    Node* parent;
    curr = root;
    while(curr != NULL) {
     if(curr->key == k) {
     found = true;
     break;
     } else {
     parent = curr;
     if ( k > curr->key ) {
     curr = curr->right;
     } else {
     curr = curr->left;
     }
     }
    }
    if(!found) {
        //cout << "Deleting... key "<< k << " is not found! "<<endl;
        return;
    }

// 3 cases :
    // 1. We're removing a leaf node
    // 2. We're removing a node with a single child
    // 3. we're removing a node with 2 children

    // Node with single child
    if ( (curr->left == NULL && curr->right != NULL) || (curr->left != NULL && curr->right == NULL) ) {
     if(curr->left == NULL && curr->right != NULL) {
     if(parent->left == curr) {
     parent->left = curr->right;
     delete curr;
     } else {
     parent->right = curr->right;
     delete curr;
     }
     }
     else // left child present, no right child
     {
     if( parent->left == curr ) {
     parent->left = curr->left;
     delete curr;
     } else {
     parent->right = curr->left;
     delete curr;
     }
     }
     return;
    }

//We're looking at a leaf node
    if( curr->left == NULL && curr->right == NULL) {
     if(parent->left == curr) {
     parent->left = NULL;
     } else {
     parent->right = NULL;
     }
     delete curr;
     return;
    }

    //Node with 2 children
    // replace node with smallest value in right subtree
    if (curr->left != NULL && curr->right != NULL) {
     Node* chkr;
     chkr = curr->right;
     if ( (chkr->left == NULL ) && ( chkr->right == NULL) ) {
     curr = chkr;
     delete chkr;
     curr->right = NULL;
     } else {	// right child has children
            //if the node's right child has a left child
            // Move all the way down left to locate smallest element

     if ( (curr->right)->left != NULL ) {
     Node* lcurr;
                Node* lcurrp;
                lcurrp = curr->right;
                lcurr = (curr->right)->left;
                while( lcurr->left != NULL ) {
                 lcurrp = lcurr;
                 lcurr = lcurr->left;
                }
curr->key = lcurr->key;
curr->value = lcurr->value;
                delete lcurr;
                lcurrp->left = NULL;
           } else {
         Node* tmp;
               tmp = curr->right;
               curr->key = tmp->key;
               curr->value = tmp->value;
curr->right = tmp->right;
               delete tmp;
           }
     }
     return;
    }
}

//---------------------------------General functions-------------------------------
void initialize(Tree& tree) {
	tree.root = NULL;
}

void add(Tree& tree, const std::string& key, void* value) {
	tree.add(&(tree.root), key, value);
}

void del(Tree& tree, const std::string& key) {
	tree.del(key);
}

void* search(Tree& tree, const std::string& k) {
	return tree.search(tree.root, k);
}


