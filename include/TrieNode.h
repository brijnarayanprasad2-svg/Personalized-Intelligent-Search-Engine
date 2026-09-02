#ifndef TRIENODE_H
#define TRIENODE_H

#include <unordered_map>
#include <vector>
#include <utility>

using namespace std;

class TrieNode
{
private:
    unordered_map<char, TrieNode*> children;
    bool isEndOfWord;

public:
    TrieNode();

    bool hasChild(char ch);

    TrieNode* getChild(char ch);

    void setChild(char ch, TrieNode* node);

    vector<pair<char, TrieNode*>> getChildren();

    bool getEndOfWord();

    void setEndOfWord(bool value);
};

#endif