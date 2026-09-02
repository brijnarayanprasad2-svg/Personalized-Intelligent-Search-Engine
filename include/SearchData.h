#ifndef SEARCHDATA_H
#define SEARCHDATA_H

#include <string>
#include <vector>

using namespace std;

class SearchData
{
public:
    vector<string> loadWords(const string& filename);
};

#endif