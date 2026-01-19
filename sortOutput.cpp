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
#include <iomanip>

bool SortOutput::compare(const std::pair<std::wstring, int>& word1,
                         const std::pair<std::wstring, int>& word2) {
    if (word1.second != word2.second) {
        return word1.second > word2.second;
    }
    return word1.first < word2.first;
}

void SortOutput::sort(const std::map<std::wstring, int>& Words,
                      const std::string& output, int count) {
    std::vector<std::pair<std::wstring, int>> vec(Words.begin(), Words.end());
    std::sort(vec.begin(), vec.end(), compare);

    std::wofstream file(output);
    auto converter = std::make_unique<std::codecvt_utf8<wchar_t>>();
    file.imbue(std::locale(file.getloc(), converter.get()));

    if (!file.is_open()) {
        std::cout << "Not open output" << std::endl;
        return;
    }

    file << L"Слово; Частота; Частота(%)\n";

    for (const auto& pair : vec) {
        double percentage = (count > 0) ? ((double)pair.second / count * 100) : 0.0;
        file << pair.first << L";" << pair.second << L";"
             << std::fixed << std::setprecision(2) << percentage << L"%\n";
    }

    file.close();
}
