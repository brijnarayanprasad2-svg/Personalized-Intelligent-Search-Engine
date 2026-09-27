#ifndef SEARCHRESULT_H
#define SEARCHRESULT_H

#include <string>

using namespace std;

class SearchResult
{
private:
    int pageID;
    string title;
    string url;
    string content;

    double score;
    double personalizationScore;

    // Personalization score components
    double queryFrequencyScore;
    double pageFrequencyScore;
    double queryPageScore;
    double recencyBonus;


public:
    // Default constructor
    SearchResult();


    // Parameterized constructor
    SearchResult(
        int id,
        const string& pageTitle,
        const string& pageURL,
        const string& pageContent,
        double relevanceScore
    );


    // Basic getters
    int getPageID() const;

    string getTitle() const;

    string getURL() const;

    string getContent() const;

    double getScore() const;


    // Personalization getter/setter
    double getPersonalizationScore() const;

    void setPersonalizationScore(
        double value
    );


    // Personalization component getters
    double getQueryFrequencyScore() const;

    double getPageFrequencyScore() const;

    double getQueryPageScore() const;

    double getRecencyBonus() const;


    // Personalization component setters
    void setQueryFrequencyScore(
        double value
    );

    void setPageFrequencyScore(
        double value
    );

    void setQueryPageScore(
        double value
    );

    void setRecencyBonus(
        double value
    );
};

#endif