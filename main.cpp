#include "sortOutput.h"
#include "wordCounter.h"
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
    auto converter = std::make_unique<std::codecvt_utf8<wchar_t>>();
    file.imbue(std::locale(file.getloc(), converter.get()));
    if (!file.is_open()) {
        std::cout << "Not open input" << std::endl;
        return 1;
    }

    WordCounter counter;
    std::wstring word;

    while (file >> word) {
        counter.addRawWord(word);
    }

    const auto& words = counter.getWords();
    int totalWords = counter.getTotalWords();

    file.close();
    SortOutput::sort(words, argv[2], totalWords);

    return 0;
}
