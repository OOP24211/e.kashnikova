//
// Created by Эвелина on 17.01.2026.
//

#ifndef LAB0_SORTOUTPUT_H
#define LAB0_SORTOUTPUT_H

#include <string>
#include <map>

class SortOutput {
private:
    static bool compare(const std::pair<std::wstring, int>& word1,
                        const std::pair<std::wstring, int>& word2);

public:
    static void sort(const std::map<std::wstring, int>& Words,
                     const std::string& output, int count);
};

#endif //LAB0_SORTOUTPUT_H
