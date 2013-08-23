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
    Node(const std::string& key, void* value);
};

struct Tree {
	Node* root;
	void add(const std::string& k, void* v);
	void del(const std::string& k);
	bool isEmpty() const { return root == NULL; }
//	bool lookup(Node* node, const std::string& k, void **v);
	void* search(Node* node, const std::string& k);
};

void initialize(Tree& tree);
void add(Tree &tree, const std::string& k, void* v);
void del(Tree &tree, const std::string& k);
//bool lookup(Tree &tree, const std::string& k, void **v);
void* search(Tree &tree, const std::string& k);

#endif
