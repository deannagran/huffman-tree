#ifndef _HUFFMAN_TREE_H_
#define _HUFFMAN_TREE_H_

#include <iostream>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>
#include <stdio.h>

class huffman_tree {
	
	public:
		struct charAndFreq {
			int freq=0;
			char cha=0;
		};

		struct lookUp{
			std::string code;
			char cha;
		};

		struct huffNode {
			int weight;
			bool isLeaf = true;
			char cha;
			huffNode *leftChild = NULL;
			huffNode *rightChild = NULL;

		};

		struct PQComparator{
			int operator()(huffNode *h1, huffNode *h2)const{
        	return h1->weight > h2->weight;
    		}
		};
		

		huffman_tree(const std::string &file_name);
		~huffman_tree();

		void lookUpTable(int code[], lookUp table[], int n, huffNode *root);
		void createLookUp(huffNode *root, int *arr, int top, lookUp *table);
		void setLookUpTable(lookUp *table);
		void setHuffTreeRoot(huffNode *root);
		lookUp* getLookUpTable() const{
			return p;
		};
		huffNode* getHuffTreeRoot() const{
			return root;
		};

		std::string get_character_code(char character) const;
		std::string encode(const std::string &file_name) const;
		std::string decode(const std::string &string_to_decode) const;	
	
	private:
		lookUp* p;
		huffNode* root;
};



#endif