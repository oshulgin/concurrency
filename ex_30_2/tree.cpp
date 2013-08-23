#include "tree.h"
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <string>


using namespace std;


//---------------------------------Node definition-------------------------------

Node::Node(const string& key, void* value) : key(key), value(value), left(NULL), right(NULL) {}


//---------------------------------Tree definition-------------------------------

void Tree::add(const string& k, void* v) {
	Node* temp = new Node(k, v);
	Node* parent;
	parent = NULL;

	//cout << "Adding: " << k << endl;
  // is this a new tree?
  if( isEmpty() ) {
	  root = temp;
  } else {
	Node* curr;
	curr = root;
	// Find the Node's parent
	while(curr)	{
		parent = curr;
		if( temp->key > curr->key ) {
			curr = curr->right;
		} else {
			curr = curr->left;
		}
	}

	if ( temp->key < parent->key ) {
	   parent->left = temp;
	} else {
		parent->right = temp;
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
        //cout << "Deleting...	key "<< k << " is not found! "<<endl;
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

//bool Tree::lookup(Node* node, const std::string& k, void **v) {
//}

void* Tree::search(Node* node, const std::string& k) {

	if ( node == NULL ) {
		return NULL;
	}
	if (k == node->key) { return node->value; }
	else {
		if (k < node->key) {
			return search(node->left, k);
		}
		else {
			return search(node->right, k);
		}
	}
}


//---------------------------------General functions-------------------------------
void initialize(Tree& tree) {
	tree.root = NULL;
}

void add(Tree& tree, const std::string& key, void* value) {
	tree.add(key, value);
}

void del(Tree& tree, const std::string& key) {
	tree.del(key);
}

//bool lookup(Tree& tree, const string& k, void **v) {
//	return tree.lookup(tree.root, k, v);
//}

void* search(Tree& tree, const std::string& k) {
	return tree.search(tree.root, k);
}


