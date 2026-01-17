//
// Created by Эвелина on 17.01.2026.
//

#ifndef LAB0_WORDCLEANER_H
#define LAB0_WORDCLEANER_H

#include <string>
#include <map>

class WordCleaner {
private:
    static void list(const std::wstring& word, std::map<std::wstring, int>& Words);

public:
    static std::wstring clean(std::wstring word, std::map<std::wstring, int>& Words);
};

#endif //LAB0_WORDCLEANER_H
