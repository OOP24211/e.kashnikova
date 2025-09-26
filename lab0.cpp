#include <algorithm>
#include <iostream>
#include <map>
#include <fstream>
#include <string>
#include <vector>

class WordCleaner {
private:
    static void list(const std::string& word, std::map<std::string, int>& Words) {
        if (!Words[word]) {
            Words[word] = 1;
        } else {
            Words[word]++;
        }
    }

public:
    static std::string clean(std::string word, std::map<std::string, int>& Words) {
        if (word[word.length() - 1] == ',' || word[word.length() -1] == '.' || word[word.length() - 1] == '!' || word[word.length() - 1] == '?') {
            word.pop_back();
        }

        for (size_t i = 0; i < word.length(); i++) {
            if (word[i] >= 'A' && word[i] <= 'Z') {
                word[i] = word[i] + 32;
            }
        }
        list(word, Words);
        return word;
    }
};

class SortOutput {
private:
    static bool compare(const std::pair<std::string, int>& word1, const std::pair<std::string, int>& word2) {
        return word1.second > word2.second;
    }
    static  std::vector<std::pair<std::string, int>> Sorted(std::map<std::string, int>& Words) {
        std::vector<std::pair<std::string, int>> vec(Words.begin(), Words.end());
        std::sort(vec.begin(), vec.end(), compare);
        return vec;
    }

public:
    static void sort(std::map<std::string, int>& Words, const std::string& output, int count) {
        std::vector<std::pair<std::string, int>> vec = Sorted(Words);


        std::ofstream file(output);

        if (!file.is_open()) {
            return;
        }

        file << "Слово; Частота; Частота(в%)\n";

        for (const auto& pair : vec) {
            file << pair.first << ";" << pair.second << ";" << (((double)pair.second / count) * 100) << "\n";
        }

        file.close();
    }
};

int main() {
    std::string File = "input.txt";
    std::ifstream file(File);
    if (!file.is_open()) {
        return 1;
    }

    std::map<std::string, int> Words;
    int count = 0;
    std::string word;
    while (file >> word) {
        count++;
        if (!word.empty()) {
            std::string cleanWord = WordCleaner::clean(word, Words);
        }
    }

    file.close();
    std::string outputFile = "output.csv";

    SortOutput::sort(Words, outputFile, count);

    return 0;
}