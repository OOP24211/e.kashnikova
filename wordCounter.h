//
// Created by Эвелина on 18.01.2026.
//

#ifndef LAB0_WORDCOUNTER_H
#define LAB0_WORDCOUNTER_H

#include <string>
#include <map>

class WordCounter {
private:
    std::map<std::wstring, int> words;
    int totalWords = 0;
public:
    void addWord(const std::wstring& word);
    void addRawWord(const std::wstring& rawWord);
    const std::map<std::wstring, int>& getWords() const;
    int getTotalWords() const;
};


#endif //LAB0_WORDCOUNTER_H
