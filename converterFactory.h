//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_CONVERTERFACTORY_H
#define SOUNDPROCESSOR_CONVERTERFACTORY_H

#include "converter.h"
#include <memory>
#include <string>
#include <vector>

class ConverterFactory {
public:
    static std::unique_ptr<Converter> createConverter(
        const std::string& name,
        const std::vector<std::string>& params);
    static std::vector<std::string> getAvailableConverters();

private:
    static std::unique_ptr<Converter> createMuteConverter(const std::vector<std::string>& params);
    static std::unique_ptr<Converter> createMixConverter(const std::vector<std::string>& params);
    static std::unique_ptr<Converter> createVolumeConverter(const std::vector<std::string>& params);

    static double parseDouble(const std::string& str, const std::string& paramName, const std::string& converterName);
    //static int parseInt(const std::string& str, const std::string& paramName, const std::string& converterName);
    static int parseStreamReference(const std::string& str, const std::string& converterName);
};

#endif //SOUNDPROCESSOR_CONVERTERFACTORY_H