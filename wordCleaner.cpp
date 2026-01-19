//
// Created by Эвелина on 17.01.2026.
//

#include "wordCleaner.h"
#include <cwctype>
#include <locale>

std::wstring WordCleaner::clean(std::wstring word) {
    if (word[word.length() - 1] == L',' || word[word.length() - 1] == L'.' ||
        word[word.length() - 1] == L'!' || word[word.length() - 1] == L'?') {
        word.pop_back();
        }

    if (word[0] == L',' || word[0] == L'.' || word[0] == L'!' ||
        word[0] == L'?' || word[0] == L'"') {
        word.erase(0, 1);
        }

    for (wchar_t& c : word) {
        c = static_cast<wchar_t>(towlower(c));
    }

    return word;
}

bool WordCleaner::isCleanWord(const std::wstring& word) {
    return !word.empty();
}
