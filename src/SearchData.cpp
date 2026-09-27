#include "../include/SearchData.h"

#include <fstream>
#include <iostream>

using namespace std;

vector<string> SearchData::loadWords(const string& filename)
{
    vector<string> words;

    ifstream file(filename);

    if (!file.is_open())
    {
        cout << "Error: Could not open file: "
             << filename << endl;

        return words;
    }

    string word;

    while (getline(file, word))
    {
        // Remove Windows carriage return
        if (!word.empty() && word.back() == '\r')
        {
            word.pop_back();
        }

        // Ignore empty lines
        if (!word.empty())
        {
            words.push_back(word);
        }
    }

    file.close();

    return words;
}