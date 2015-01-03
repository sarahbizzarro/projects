typedef struct TrieNodeStruct {
    char key;
    struct TrieNodeStruct *next_sibling;
    struct TrieNodeStruct *first_child;
    int is_end;		/* 1 if this node is the end of a word*/
    int occurrences;
    int superwords;
    int prefixes;
} TrieNode;

typedef struct {
	TrieNode *root;
} Dictionary;

TrieNode *createNode(char letter);
void readDict(FILE *dict_file);
TrieNode *get_child(TrieNode *parent, char letter, int create);
void process_word(char *word);
void increment_prefixes(TrieNode *first_child);
void dict_destroy (Dictionary *dict);
void trie_destroy(TrieNode *root);
void scanData(FILE *data_file);
void printDict(Dictionary *dictionary);
void printDictHelper(TrieNode *root, char *buffer, int index);
