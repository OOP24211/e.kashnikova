//
// Created by Эвелина on 17.01.2026.
//

#include "wordCleaner.h"
#include <cwctype>
#include <locale>

void WordCleaner::list(const std::wstring& word, std::map<std::wstring, int>& Words) {
        Words[word]++;
}

std::wstring WordCleaner::clean(std::wstring word, std::map<std::wstring, int>& Words) {
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

    if (!word.empty()) {
        list(word, Words);
    }

    return word;
}
