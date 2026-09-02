#ifndef TRIE_H
#define TRIE_H

#include "TrieNode.h"

#include <vector>
#include <string>

using namespace std;

class Trie
{
private:
    TrieNode* root;

    void collectWords(
        TrieNode* node,
        string currentWord,
        vector<string>& results
    );

public:
    Trie();

    void insert(const string& word);

    bool search(const string& word);

    vector<string> autocomplete(const string& prefix);
};

#endif