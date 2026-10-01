#include "../include/Trie.h"

#include <algorithm>
#include <cstdlib>

using namespace std;


// =========================================================
// CONSTRUCTOR
// =========================================================

Trie::Trie()
{
    root = new TrieNode();
}


// =========================================================
// INSERT WORD
// =========================================================

void Trie::insert(
    const string& word
)
{
    if (word.empty())
    {
        return;
    }

    TrieNode* current = root;

    for (char ch : word)
    {
        if (!current->hasChild(ch))
        {
            current->setChild(
                ch,
                new TrieNode()
            );
        }

        current =
            current->getChild(ch);
    }

    current->setEndOfWord(true);

    // Store dictionary word for spelling correction.
    dictionaryWords.push_back(word);
}


// =========================================================
// SEARCH WORD
// =========================================================

bool Trie::search(
    const string& word
) const
{
    if (word.empty())
    {
        return false;
    }

    TrieNode* current = root;

    for (char ch : word)
    {
        if (!current->hasChild(ch))
        {
            return false;
        }

        current =
            current->getChild(ch);
    }

    return current->getEndOfWord();
}


// =========================================================
// COLLECT WORDS
// =========================================================

void Trie::collectWords(
    TrieNode* node,
    string currentWord,
    vector<string>& results
) const
{
    if (node == nullptr)
    {
        return;
    }

    if (node->getEndOfWord())
    {
        results.push_back(
            currentWord
        );
    }

    vector<pair<char, TrieNode*>> children =
        node->getChildren();

    for (const auto& child :
         children)
    {
        collectWords(
            child.second,
            currentWord + child.first,
            results
        );
    }
}


// =========================================================
// AUTOCOMPLETE
// =========================================================

vector<string> Trie::autocomplete(
    const string& prefix
) const
{
    vector<string> results;

    TrieNode* current = root;

    for (char ch : prefix)
    {
        if (!current->hasChild(ch))
        {
            return results;
        }

        current =
            current->getChild(ch);
    }

    collectWords(
        current,
        prefix,
        results
    );

    return results;
}


// =========================================================
// LEVENSHTEIN EDIT DISTANCE
// =========================================================

int Trie::calculateEditDistance(
    const string& first,
    const string& second
) const
{
    const int n =
        static_cast<int>(
            first.length()
        );

    const int m =
        static_cast<int>(
            second.length()
        );


    // Only two rows are used.
    // Space complexity = O(m)

    vector<int> previous(
        m + 1
    );

    vector<int> current(
        m + 1
    );


    // Base case
    for (int j = 0;
         j <= m;
         j++)
    {
        previous[j] = j;
    }


    for (int i = 1;
         i <= n;
         i++)
    {
        current[0] = i;

        for (int j = 1;
             j <= m;
             j++)
        {
            int insertion =
                current[j - 1] + 1;

            int deletion =
                previous[j] + 1;

            int replacement =
                previous[j - 1] +
                (
                    first[i - 1] !=
                    second[j - 1]
                );


            current[j] =
                min(
                    {
                        insertion,
                        deletion,
                        replacement
                    }
                );
        }

        previous.swap(current);
    }

    return previous[m];
}


// =========================================================
// FIND CLOSEST DICTIONARY WORD
// =========================================================

string Trie::findClosestWord(
    const string& word
) const
{
    if (word.empty())
    {
        return "";
    }


    // Already correct
    if (search(word))
    {
        return word;
    }


    // Limit allowed edit distance
    int maxDistance;

    if (word.length() <= 3)
    {
        maxDistance = 1;
    }
    else if (word.length() <= 6)
    {
        maxDistance = 2;
    }
    else
    {
        maxDistance = 3;
    }


    string closestWord;

    int bestDistance =
        maxDistance + 1;


    // Compare against dictionary
    for (const string& candidate :
         dictionaryWords)
    {
        // -------------------------------------------------
        // Fast filtering based on word length
        // -------------------------------------------------

        int lengthDifference =
            abs(
                static_cast<int>(
                    word.length()
                ) -
                static_cast<int>(
                    candidate.length()
                )
            );


        if (lengthDifference >
            maxDistance)
        {
            continue;
        }


        // -------------------------------------------------
        // Calculate edit distance
        // -------------------------------------------------

        int distance =
            calculateEditDistance(
                word,
                candidate
            );


        // -------------------------------------------------
        // Better candidate
        // -------------------------------------------------

        if (distance < bestDistance)
        {
            bestDistance =
                distance;

            closestWord =
                candidate;
        }


        // -------------------------------------------------
        // Tie breaker
        // -------------------------------------------------

        else if (
            distance == bestDistance &&
            !closestWord.empty() &&
            candidate.length() <
                closestWord.length()
        )
        {
            closestWord =
                candidate;
        }
    }


    // -----------------------------------------------------
    // Return valid correction
    // -----------------------------------------------------

    if (bestDistance <= maxDistance)
    {
        return closestWord;
    }

    return "";
}