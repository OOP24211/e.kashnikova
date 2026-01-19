//
// Created by Эвелина on 17.01.2026.
//

#ifndef LAB0_WORDCLEANER_H
#define LAB0_WORDCLEANER_H

#include <string>

class WordCleaner {
public:
    static std::wstring clean(std::wstring word);
    static bool isCleanWord(const std::wstring& word);
};

#endif //LAB0_WORDCLEANER_H
