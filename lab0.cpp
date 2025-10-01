#include <algorithm>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>
#include <locale>
#include <codecvt>

class WordCleaner {
private:
    static void list(const std::wstring& word, std::map<std::wstring, int>& Words) {
        if (!Words[word]) {
            Words[word] = 1;
        } else {
            Words[word]++;
        }
    }

public:
    static std::wstring clean(std::wstring word, std::map<std::wstring, int>& Words) {
        if (word[word.length() - 1] == L',' || word[word.length() -1] == L'.' || word[word.length() - 1] == L'!' || word[word.length() - 1] == L'?') {
            word.pop_back();
        }

        if (word[0] == L',' || word[0] == L'.' || word[0] == L'!' || word[0] == L'?' || word[0] == L'"') {
            word.erase(0, 1);
        }

        for (wchar_t& c : word) {
            c = static_cast<wchar_t>(towlower(c));
        }

        list(word, Words);
        return word;
    }
};

class SortOutput {
private:
    static bool compare(const std::pair<std::wstring, int>& word1, const std::pair<std::wstring, int>& word2) {
        return word1.second > word2.second;
    }
public:
    static void sort(std::map<std::wstring, int>& Words, const std::string& output, int count) {
        std::vector<std::pair<std::wstring, int>> vec(Words.begin(), Words.end());
        std::sort(vec.begin(), vec.end(), compare);

        std::wofstream file(output);
        file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
        if (!file.is_open()) {
            std::cout << "Not open output" << std::endl;
            return;
        }

        file << L"Слово; Частота; Частота(в%)\n";

        for (const auto& pair : vec) {
            file << pair.first << L";" << pair.second << L";" << (((double)pair.second / count) * 100) << L"%\n";
        }

        file.close();
    }
};

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    std::wifstream file(argv[1]);
    file.imbue(std::locale(file.getloc(), new std::codecvt_utf8<wchar_t>));
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
