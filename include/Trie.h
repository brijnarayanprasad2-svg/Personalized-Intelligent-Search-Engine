#ifndef PERSONALIZED_SEARCH_TRIE_H
#define PERSONALIZED_SEARCH_TRIE_H

#include "TrieNode.h"

#include <vector>
#include <string>

using namespace std;

class Trie
{
private:
    TrieNode* root;

    // Dictionary words used by spelling correction
    vector<string> dictionaryWords;

    // Collect complete words from a Trie node
    void collectWords(
        TrieNode* node,
        string currentWord,
        vector<string>& results
    ) const;

    // Levenshtein edit distance
    int calculateEditDistance(
        const string& first,
        const string& second
    ) const;

public:
    Trie();

    // Insert dictionary word
    void insert(
        const string& word
    );

    // Exact word search
    bool search(
        const string& word
    ) const;

    // Prefix based autocomplete
    vector<string> autocomplete(
        const string& prefix
    ) const;

    // Find nearest dictionary word
    string findClosestWord(
        const string& word
    ) const;
};

#endif