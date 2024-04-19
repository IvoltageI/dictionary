#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int isEndOfWord;
    int count;
};

// Initializes a trie node
struct TrieNode* createTrieNode() {
    struct TrieNode *node = (struct TrieNode*)malloc(sizeof(struct TrieNode));
    if (node) {
        node->isEndOfWord = 0;
        node->count = 0;
        for (int i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Inserts a word into the trie
void insert(struct TrieNode *root, char *word) {
    struct TrieNode *current = root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            current->children[index] = createTrieNode();
        }
        current = current->children[index];
    }
    current->isEndOfWord = 1;
    current->count++;
}

// Counts the occurrences of a word in the trie
int numberOfOccurrences(struct TrieNode *root, char *word) {
    struct TrieNode *current = root;
    int length = strlen(word);
    for (int i = 0; i < length; i++) {
        int index = word[i] - 'a';
        if (!current->children[index]) {
            return 0; // Word not found
        }
        current = current->children[index];
    }
    if (current != NULL && current->isEndOfWord) {
        return current->count;
    } else {
        return 0;
    }
}

// Deallocates memory allocated for the trie
void deallocateTrie(struct TrieNode *root) {
    if (!root) return;
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (root->children[i]) {
            deallocateTrie(root->children[i]);
        }
    }
    free(root);
}

// Reads dictionary from file and stores words in array
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    int count = 0;
    char word[100]; // Assuming max word length is 100 characters
    while (fscanf(file, "%s", word) != EOF) {
        pInWords[count] = strdup(word);
        count++;
    }

    fclose(file);
    return count;
}

int main(void) {
    char *inWords[256];

    // Read the number of words in the dictionary
    int numWords = readDictionary("dictionary.txt", inWords);
    for (int i = 0; i < numWords; ++i) {
        printf("%s\n", inWords[i]);
    }

    struct TrieNode *root = createTrieNode();
    for (int i = 0; i < numWords; i++) {
        insert(root, inWords[i]);
    }

    // Test word occurrences
    char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
    for (int i = 0; i < 5; i++) {
        printf("\t%s : %d\n", pWords[i], numberOfOccurrences(root, pWords[i]));
    }

    deallocateTrie(root);

    return 0;
}

