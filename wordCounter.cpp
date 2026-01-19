//
// Created by Эвелина on 18.01.2026.
//

#include "wordCounter.h"
#include "wordCleaner.h"

void WordCounter::addWord(const std::wstring& word) {
    if (WordCleaner::isCleanWord(word)) { 
        words[word]++;
        totalWords++;
    }
}

void WordCounter::addRawWord(const std::wstring& rawWord) {
    std::wstring cleanedWord = WordCleaner::clean(rawWord);
    addWord(cleanedWord);
}

const std::map<std::wstring, int>& WordCounter::getWords() const {
    return words;
}

int WordCounter::getTotalWords() const {
    return totalWords;
}
