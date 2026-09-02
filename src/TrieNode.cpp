#include "../include/TrieNode.h"

TrieNode::TrieNode()
{
    isEndOfWord = false;
}

bool TrieNode::hasChild(char ch)
{
    return children.find(ch) != children.end();
}

TrieNode* TrieNode::getChild(char ch)
{
    return children[ch];
}

void TrieNode::setChild(char ch, TrieNode* node)
{
    children[ch] = node;
}

vector<pair<char, TrieNode*>> TrieNode::getChildren()
{
    vector<pair<char, TrieNode*>> result;

    for (auto& child : children)
    {
        result.push_back(child);
    }

    return result;
}

bool TrieNode::getEndOfWord()
{
    return isEndOfWord;
}

void TrieNode::setEndOfWord(bool value)
{
    isEndOfWord = value;
}