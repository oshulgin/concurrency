#ifndef TREE_H
#define TREE_H

#include <pthread.h>
#include <stdio.h>
#include <string>

struct Node {
    std::string key;
    void* value;
    Node *left;
    Node *right;
    pthread_mutex_t mutex;
    Node(const std::string& key, void* value);
    ~Node();
};

struct Tree {
	Node* root;
	void add(Node** node, const std::string& k, void* v);
	void del(Node* node, const std::string& k);
	Node* getMin(Node* node);
	void* search(Node* node, const std::string& k);
};

void initialize(Tree& tree);
void add(Tree &tree, const std::string& k, void* v);
void del(Tree &tree, const std::string& k);
void* search(Tree &tree, const std::string& k);

#endif
