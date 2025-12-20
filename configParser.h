//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_CONFIGPARSER_H
#define SOUNDPROCESSOR_CONFIGPARSER_H

#include "converter.h"
#include <vector>
#include <string>
#include <memory>

struct ConfigLine {
    std::string converterName;
    std::vector<std::string> parameters;
};

class ConfigParser {
public:
    static std::vector<std::unique_ptr<Converter>> parse(
        const std::string& filename,
        const std::vector<AudioBuffer>& inputStreams);

private:
    static std::string trim(const std::string& str);

    static ConfigLine parseLine(const std::string& line);
};


#endif //SOUNDPROCESSOR_CONFIGPARSER_H