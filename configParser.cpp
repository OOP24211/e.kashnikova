//
// Created by Эвелина on 16.12.2025.
//

#include "configParser.h"
#include "converterFactory.h"
#include <fstream>
#include <sstream>
#include <stdexcept>

std::vector<std::unique_ptr<Converter>> ConfigParser::parse(
    const std::string& filename,
    const std::vector<AudioBuffer>& inputStreams) {

    std::vector<std::unique_ptr<Converter>> converters;
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error("Не могу открыть файл конфигурации: " + filename);
    }

    std::string line;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;

        line = trim(line);
        if (line.empty() || line[0] == '#') {
            continue;
        }

        try {
            ConfigLine configLine = parseLine(line);

            auto converter = ConverterFactory::createConverter(
                configLine.converterName,
                configLine.parameters);

            converters.push_back(std::move(converter));

        } catch (const std::exception& e) {
            std::stringstream ss;
            ss << "Ошибка в строке " << lineNumber << ": " << e.what();
            throw std::runtime_error(ss.str());
        }
    }

    return converters;
}

std::string ConfigParser::trim(const std::string& str) {
    size_t first = str.find_first_not_of(" \t");
    if (first == std::string::npos) {
        return "";
    }

    size_t last = str.find_last_not_of(" \t");
    return str.substr(first, last - first + 1);
}

ConfigLine ConfigParser::parseLine(const std::string& line) {
    ConfigLine result;
    std::istringstream iss(line);

    if (!(iss >> result.converterName)) {
        throw std::runtime_error("Пустая строка конфигурации");
    }

    std::string param;
    while (iss >> param) {
        result.parameters.push_back(param);
    }

    return result;
}