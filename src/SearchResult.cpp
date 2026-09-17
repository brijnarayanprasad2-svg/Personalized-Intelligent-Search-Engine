#include "../include/SearchResult.h"

using namespace std;


// ==================================================
// DEFAULT CONSTRUCTOR
// ==================================================

SearchResult::SearchResult()
{
    pageID = 0;

    title = "";

    url = "";

    content = "";

    score = 0.0;

    personalizationScore = 0.0;

    queryFrequencyScore = 0.0;

    pageFrequencyScore = 0.0;

    queryPageScore = 0.0;

    recencyBonus = 0.0;
}


// ==================================================
// PARAMETERIZED CONSTRUCTOR
// ==================================================

SearchResult::SearchResult(
    int id,
    const string& pageTitle,
    const string& pageURL,
    const string& pageContent,
    double relevanceScore)
{
    pageID = id;

    title = pageTitle;

    url = pageURL;

    content = pageContent;

    score = relevanceScore;

    personalizationScore = 0.0;

    queryFrequencyScore = 0.0;

    pageFrequencyScore = 0.0;

    queryPageScore = 0.0;

    recencyBonus = 0.0;
}


// ==================================================
// BASIC GETTERS
// ==================================================

int SearchResult::getPageID() const
{
    return pageID;
}


string SearchResult::getTitle() const
{
    return title;
}


string SearchResult::getURL() const
{
    return url;
}


string SearchResult::getContent() const
{
    return content;
}


double SearchResult::getScore() const
{
    return score;
}


// ==================================================
// PERSONALIZATION GETTER
// ==================================================

double SearchResult::getPersonalizationScore() const
{
    return personalizationScore;
}


// ==================================================
// PERSONALIZATION SETTER
// ==================================================

void SearchResult::setPersonalizationScore(
    double value)
{
    personalizationScore = value;
}


// ==================================================
// PERSONALIZATION COMPONENT GETTERS
// ==================================================

double SearchResult::getQueryFrequencyScore() const
{
    return queryFrequencyScore;
}


double SearchResult::getPageFrequencyScore() const
{
    return pageFrequencyScore;
}


double SearchResult::getQueryPageScore() const
{
    return queryPageScore;
}


double SearchResult::getRecencyBonus() const
{
    return recencyBonus;
}


// ==================================================
// PERSONALIZATION COMPONENT SETTERS
// ==================================================

void SearchResult::setQueryFrequencyScore(
    double value)
{
    queryFrequencyScore = value;
}


void SearchResult::setPageFrequencyScore(
    double value)
{
    pageFrequencyScore = value;
}


void SearchResult::setQueryPageScore(
    double value)
{
    queryPageScore = value;
}


void SearchResult::setRecencyBonus(
    double value)
{
    recencyBonus = value;
}