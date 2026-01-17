//
// Created by Эвелина on 17.01.2026.
//

#include "sortOutput.h"
#include <algorithm>
#include <vector>
#include <fstream>
#include <iostream>
#include <locale>
#include <codecvt>
#include <memory>

bool SortOutput::compare(const std::pair<std::wstring, int>& word1,
                         const std::pair<std::wstring, int>& word2) {
    return word1.second > word2.second;
}

void SortOutput::sort(std::map<std::wstring, int>& Words,
                      const std::string& output, int count) {
    std::vector<std::pair<std::wstring, int>> vec(Words.begin(), Words.end());
    std::sort(vec.begin(), vec.end(), compare);

    std::wofstream file(output);
    file.imbue(std::locale(file.getloc(), std::make_unique<std::codecvt_utf8<wchar_t>>().get()));
    if (!file.is_open()) {
        std::cout << "Not open output" << std::endl;
        return;
    }

    file << L"Слово; Частота; Частота(в%)\n";

    for (const auto& pair : vec) {
        file << pair.first << L";" << pair.second << L";"
             << (((double)pair.second / count) * 100) << L"%\n";
    }

    file.close();
}
