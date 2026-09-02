#include "../include/Trie.h"

Trie::Trie()
{
    root = new TrieNode();
}

void Trie::insert(const string& word)
{
    TrieNode* current = root;

    for (char ch : word)
    {
        if (!current->hasChild(ch))
        {
            current->setChild(ch, new TrieNode());
        }

        current = current->getChild(ch);
    }

    current->setEndOfWord(true);
}

bool Trie::search(const string& word)
{
    TrieNode* current = root;

    for (char ch : word)
    {
        if (!current->hasChild(ch))
        {
            return false;
        }

        current = current->getChild(ch);
    }

    return current->getEndOfWord();
}

void Trie::collectWords(
    TrieNode* node,
    string currentWord,
    vector<string>& results
)
{
    if (node->getEndOfWord())
    {
        results.push_back(currentWord);
    }

    vector<pair<char, TrieNode*>> children =
        node->getChildren();

    for (auto& child : children)
    {
        char ch = child.first;

        TrieNode* nextNode = child.second;

        collectWords(
            nextNode,
            currentWord + ch,
            results
        );
    }
}

vector<string> Trie::autocomplete(const string& prefix)
{
    vector<string> results;

    TrieNode* current = root;

    for (char ch : prefix)
    {
        if (!current->hasChild(ch))
        {
            return results;
        }

        current = current->getChild(ch);
    }

    collectWords(
        current,
        prefix,
        results
    );

    return results;
}