#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "dictstat.h"

Dictionary *dictionary;

int main(int argc, char const *argv[])
{

	FILE *dict_file;
	FILE *data_file;

	if (argc < 3) {
		fprintf(stderr, "invalid input\n");
		return -1;
	}

	dict_file = fopen(argv[1], "r");
	readDict(dict_file);
	fclose(dict_file);

	data_file = fopen(argv[2], "r");
	scanData(data_file);
	fclose(data_file);

	/*
	 * note: remember to close the files when finished reading them using fclose(file_name)
	 */

	printDict(dictionary);

	/* another note: make a function to free all nodes in the trie (use LRV recursion) */

	dict_destroy(dictionary);

	return 0;
}

void dict_destroy (Dictionary *dict){

	if (dict == NULL) return;

	trie_destroy(dict->root);

	free(dict);

}

void trie_destroy(TrieNode *root)
{
	if (root == NULL){
		return;
	}

	trie_destroy(root->first_child);
	trie_destroy(root->next_sibling);

	free(root);
}

 
TrieNode *createNode(char letter)
{
	TrieNode *newNode = (TrieNode *)malloc(sizeof(TrieNode));
	newNode->key = tolower(letter);
	newNode->next_sibling = NULL;
	newNode->first_child = NULL;
	newNode->is_end = 0;
	newNode->occurrences = 0;
	newNode->superwords = 0;
	newNode->prefixes = 0;

	return newNode;
}


/*
 * This function takes in a file pointer to the dictionary file and
 * reads and stores words from the Dictionary file into an appropriate data structure.
 */

/*if a prefix of a Dictionary word is in the data file, that’s the prefix count*/
void readDict(FILE *dict_file)
{

	char buffer[128];
	int i;
	TrieNode *temp;
	TrieNode *root = createNode('*');

	dictionary = malloc(sizeof(Dictionary));
	dictionary->root = root;

	fscanf(dict_file, "%*[^a-zA-Z]");

	while(fscanf(dict_file, "%[a-zA-Z]%*[^a-zA-Z]", buffer) == 1){
		temp = root;
		for(i = 0; buffer[i]; i++){

			temp = get_child(temp, tolower(buffer[i]), 1);


		}
		temp->is_end = 1;
	}
}

/*
 * Takes in a file pointer to the data file and, for every Dictionary word stored in
 * the data structure, counts occurrences, prefixes and superwords in the data file.
 */
void scanData(FILE *data_file)
{
	char buffer[128];

	fscanf(data_file, "%*[^a-zA-Z]");

	while(fscanf(data_file, "%[a-zA-Z]%*[^a-zA-Z]", buffer) == 1){

		process_word(buffer);

	}
}

/*
 * Attempts to return a pointer to the parent node's child that contains the letter.
 * If the target node does not exist, it is created if @create is non-zero, otherwise
 * it returns NULL.
 */
TrieNode *get_child(TrieNode *parent, char letter, int create)
{
	TrieNode *prev, *curr, *temp;
	letter = tolower(letter);
	if (parent->first_child == NULL) {
		if (create){
			parent->first_child = createNode(letter);
			return parent->first_child;
		} else {
			return NULL;
		}	
	}

	if (parent->first_child->key > letter){
		if (create){
			temp = parent->first_child;
			parent->first_child = createNode(letter);
			parent->first_child->next_sibling = temp;
			return parent->first_child;
		} else {
			return NULL;
		}
	}

	prev = NULL;
	curr = parent->first_child;

	while (curr != NULL){

		if (curr->key == letter){
			return curr;
		} else if (curr->key > letter){
			if (create){
				prev->next_sibling = createNode(letter);
				prev->next_sibling->next_sibling = curr;
				return prev->next_sibling;
			} else {
				return NULL;
			}
		}
		prev = curr;
		curr = curr->next_sibling;
	}

	if (create){
		prev->next_sibling = createNode(letter);
		return prev->next_sibling;
	} else {
		return NULL;
	}
}

/*
 * Processes a word by updating the dictionary.
 */
void process_word(char *word)
{
	int i;
	TrieNode *temp = dictionary->root;

	for (i = 0; word[i]; i++){
		temp = get_child(temp, word[i], 0);
		if (temp == NULL){
			/* node doesn't have that child (so the word is not in the trie) */
			return;
		} else if (temp->is_end == 1){
			if (word[i + 1] == '\0'){
				temp->occurrences++;
			} else {
				temp->superwords++;
			}
		}
	}
	increment_prefixes(temp->first_child);
}

/*
 * Recursively increments the prefix counts of any siblings/descendants of this node.
 * Note: use a helper method, don't
 * increase the prefix count of the actual root.
 */
void increment_prefixes(TrieNode *first_child)
{
	if (first_child == NULL) return;

	if (first_child->is_end){
		first_child->prefixes++;
	}

	increment_prefixes(first_child->first_child);
	increment_prefixes(first_child->next_sibling);
}

void printDict(Dictionary *dictionary)
{
	char buffer[128];
	printDictHelper(dictionary->root->first_child, buffer, 0);
}

/*
 * Helper function that recursively (VLR) prints the trie.
 * @index is the index of the next empty spot in buffer.
 */
void printDictHelper(TrieNode *root, char *buffer, int index)
{
	if (root == NULL) return;

	buffer[index] = root->key;

	if (root->is_end){
		buffer[index + 1] = '\0';
		printf("%s %d %d %d\n", buffer, root->occurrences, root->prefixes, root->superwords);
	}

	printDictHelper(root->first_child, buffer, index + 1);
	
	buffer[index] = '\0'; /* not really necessary, will get overwritten anyway */

	printDictHelper(root->next_sibling, buffer, index);
}
