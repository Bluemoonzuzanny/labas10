#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Trie structure
struct Trie
{	
	struct Trie *children[26];
	int count;
};

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word)
{
	struct Trie *node = pTrie;
	while (*word) {
		if (!node->children[*word - 'a'])
			node->children[*word - 'a'] = calloc(1, sizeof(struct Trie));
		node = node->children[*word - 'a'];
		word++;
	}
	node->count++;
}

// computes the number of occurances of the word
int numberOfOccurances(struct Trie *pTrie, char *word)
{
	struct TrieNode *crawler = root;
	while (*word) {
		int index = *word - 'a';
		if (!crawler->children[index])
			return 0;
		crawler = crawler->children[index];
		word++;
	}
	return crawler->count;
}

// deallocate the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie)
{
	if (!root) return;
	for (int i = 0; i < 26; i++) {
		if (root->children[i])
			deallocateTrie(root->children[i]);
	}
	free(root);
}

// Initializes a trie structure
struct Trie *createTrie()
{
	return calloc(1, sizeof(struct Trie));
}

// this function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords)
{
	FILE *file = fopen(filename, "r");
	if (!file) return -1;
	
	int numWords = 0;
	char word[100];
	while (fgets(word, sizeof(word), file)) {
		word[strcspn(word, "\n")] = 0;
		pInWords[numWords] = strdup(word);
		numWords++;
	}
	fclose(file);
	return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}
