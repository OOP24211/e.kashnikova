#include "wordCleaner.h"
#include "sortOutput.h"
#include <iostream>
#include <fstream>
#include <locale>
#include <codecvt>
#include <memory>

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <input_file> <output_file>" << std::endl;
        return 1;
    }

    std::wifstream file(argv[1]);
    file.imbue(std::locale(file.getloc(), std::make_unique<std::codecvt_utf8<wchar_t>>().get()));
    if (!file.is_open()) {
        std::cout << "Not open input" << std::endl;
        return 1;
    }

    std::map<std::wstring, int> Words;
    int count = 0;
    std::wstring word;

    while (file >> word) {
        count++;
        if (!word.empty()) {
            std::wstring cleanWord = WordCleaner::clean(word, Words);
        }
    }

    file.close();
    SortOutput::sort(Words, argv[2], count);

    return 0;
}
    SortOutput::sort(Words, argv[2], count);

    return 0;
}
