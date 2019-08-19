#include "huffman_tree.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <bits/stdc++.h>
#include <stdio.h>
#include <string>
using namespace std;
/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Reads the contents of file_name and constructs a
				huffman tree based on the character frequencies of the file contents
*/
huffman_tree::huffman_tree(const std::string &file_name){

	char ch;
	//create hash table of charcters and their corresponding frequencies
    huffman_tree::charAndFreq charsAndFreqs[256];

	//read file character by character and log frequencies
    fstream fin(file_name, fstream::in);
    while (fin >> noskipws >> ch) {

        //character is not in table yet, so add it and set freq to 1.
        if(charsAndFreqs[ch % 256].freq == 0){
            charAndFreq chara;
            chara.cha = ch;
            chara.freq = 1;
            charsAndFreqs[ch % 256] = chara;
        }else{
            //character is already in table. increase frequency.
            charsAndFreqs[ch % 256].freq = charsAndFreqs[ch % 256].freq + 1;
        }

    }

	priority_queue <huffNode*, vector<huffNode*>, PQComparator> pq2;
	//create priority q of leaf nodes
    for(int i = 0; i < 256; i++){
        if(charsAndFreqs[i].freq != 0){
            huffNode *h1 = new huffNode;
            h1->cha = charsAndFreqs[i].cha;
            h1->weight = charsAndFreqs[i].freq;
            pq2.push(h1);
        }
    }

	//build tha damn thang
    huffNode *h1;
    huffNode *h2;
	
    while(pq2.size() > 1){
        h1 = pq2.top();
        pq2.pop();
        h2 = pq2.top();
        pq2.pop();

		huffNode *h3 = new huffNode();
        h3->weight = h1->weight + h2->weight;
        h3->cha = '*';
        h3->leftChild = h1;
        h3->rightChild = h2;
        h3->isLeaf = false;

        pq2.push(h3);
    }

    huffNode* root = pq2.top();
	setHuffTreeRoot(root);
	int codes[256];
    lookUp lookUpHere[256];

	//initialize lookUp table with blank codes
	for(int i = 0; i < 256; i++){
        lookUpHere[i].code = "";
    }
    createLookUp(root, codes, 0, lookUpHere);
	setLookUpTable(lookUpHere);
}

huffman_tree::~huffman_tree(){
	//delete [] root;
	delete root;
}

/*
Preconditions: Character is a character with an ASCII value
				between 0 and 127 (inclusive).
Postconditions: Returns the Huffman code for character if character is in the tree
				and an empty string otherwise.
*/
std::string huffman_tree::get_character_code(char character) const {
	lookUp* p = getLookUpTable();
	std::string code = "";
	code = p[character % 256].code;
	return code;
}

/*
Preconditions: file_name is the name of (and possibly path to) a text file
Postconditions: Returns the Huffman encoding for the contents of file_name
				if file name exists and an empty string otherwise.
				If the file contains letters not present in the huffman_tree,
				return an empty string
*/
std::string huffman_tree::encode(const std::string &file_name) const {
	lookUp* p = getLookUpTable();
	huffNode* root = getHuffTreeRoot();

	// //check for single character tree
	// if(root->isLeaf){
	// 	return "1";
	// }

	std::string code = "";
	char ch;
	int count = 0;
	//read file character by character, get character code from table and append to code string
    fstream fin(file_name, fstream::in);
    while (fin >> noskipws >> ch) {
		if((p[ch % 256].code) != ""){
			code += (p[ch % 256].code);
		}
		else if(root->isLeaf){
			count++;
		}else{
			return "";
		}
	}
	//check for single character tree
	if(root->isLeaf){
		for(int i = 0; i < count; i++){
			code =  code + to_string(1);
		}
		return code;
	}

	return code;
}

/*
Preconditions: string_to_decode is a string containing Huffman-encoded text
Postconditions: Returns the plaintext represented by the string if the string
				is a valid Huffman encoding and an empty string otherwise
*/
std::string huffman_tree::decode(const std::string &string_to_decode) const {
	//lookUp* p = getLookUpTable();
	huffNode* root = getHuffTreeRoot();
	string s = "";

	//check if code contains anything but 1s or 0s. if it does, return empty string
    for (int i = 0; (unsigned)i < string_to_decode.length(); i++){
		if(string_to_decode.at(i) != '0' && string_to_decode.at(i) != '1'){
			return "";
		}
	}

	//check for single character tree. if root is a leaf, just print out the character stored in root.
	if(root->isLeaf){
		//print root n times
		if(string_to_decode.length() > 1){
			//root character should be printed more than once
			for(int i = 0; (unsigned)i < string_to_decode.length(); i++){
				s += (root->cha);
			}
			return s;
		}else{
			//root character should only be printed once
			return s + (root->cha);
		}
	}
        
	string decoded = ""; 
	huffNode *currentNode = root;

	//0 indicates you should traverse left. 1 indicates you should traverse right.
	//do this until you reach a leaf.
    for (int i = 0; (unsigned)i < string_to_decode.size(); i++){ 
        if (string_to_decode[i] == '0'){
			currentNode = currentNode->leftChild;
		} 
        else if(string_to_decode[i] == '1'){
			currentNode = currentNode->rightChild; 
		}
        //we reached a leaf, so add to decoded string, and start traversing from the root of the tree again 
        if (currentNode->isLeaf){ 
            decoded += currentNode->cha; 
            currentNode = root; 
        } 
    } 
    return decoded; 
}

void huffman_tree::lookUpTable(int code[], lookUp table[], int n, huffNode *root)
{
	//adds character + corresponding code to lookup table
    string codeStr = "";

    for (int i = 0; i < n; i++){
        codeStr = codeStr + to_string(code[i]);
    }

    table[root->cha % 256].code = codeStr;
    table[root->cha % 256].cha = root->cha;
}


void huffman_tree::createLookUp(huffNode *root, int *code, int counter, lookUp *table){
	//do a recursive preorder traversal of the tree.
	//if you travel left, append 0.
	//if you travel right, append 1.
	//if the current node is a leaf, add the char and corresponding code to the lookup table.
    if (root->isLeaf == true) {
        lookUpTable(code, table, counter, root);
    }

	//if left child exists, append 0 to code, call recursive createLookUp on child
    if (root->leftChild != NULL) {
        //left routes == 0
        code[counter] = 0;
		//increment counter to set the next element in code array
        createLookUp(root->leftChild, code, counter + 1, table);
    }

	//if right child exists, append 1 to code, call recursive createLookUp on child
    if (root->rightChild != NULL) {
        //right routes == 1
        code[counter] = 1;
		//increment counter to set the next element in code array
        createLookUp(root->rightChild, code, counter + 1, table);
    }
}

void huffman_tree::setLookUpTable(lookUp *table){
	this->p = table;
}

void huffman_tree::setHuffTreeRoot(huffNode *rooot){
	this->root = rooot;
}

