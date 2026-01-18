//
// Created by Эвелина on 16.12.2025.
//

#include "converterFactory.h"
#include "muteConverter.h"
#include "mixConverter.h"
#include "volumeConverter.h"
#include <sstream>
#include <stdexcept>

std::unique_ptr<Converter> ConverterFactory::createConverter(
    const std::string& name,
    const std::vector<std::string>& params) {

    if (name == "mute") {
        return createMuteConverter(params);
    } else if (name == "mix") {
        return createMixConverter(params);
    } else if (name == "volume") {
        return createVolumeConverter(params);
    }

    throw std::invalid_argument("Неизвестный конвертер: " + name);
}

std::unique_ptr<Converter> ConverterFactory::createMuteConverter(
    const std::vector<std::string>& params) {

    if (params.size() < 2) {
        throw std::invalid_argument("mute требует 2 параметра: начало и конец");
    }

    double start = parseDouble(params[0], "начало", "mute");
    double end = parseDouble(params[1], "конец", "mute");

    return std::make_unique<MuteConverter>(start, end);
}

std::unique_ptr<Converter> ConverterFactory::createMixConverter(
    const std::vector<std::string>& params) {

    if (params.empty()) {
        throw std::invalid_argument("mix требует хотя бы 1 параметр");
    }

    std::string streamParam = params[0];
    int streamNumber = parseStreamReference(streamParam, "mix");

    double offset = 0.0;
    if (params.size() >= 2) {
        offset = parseDouble(params[1], "смещение", "mix");
    }

    return std::make_unique<MixConverter>(streamNumber, offset);
}

std::unique_ptr<Converter> ConverterFactory::createVolumeConverter(
    const std::vector<std::string>& params) {

    if (params.empty()) {
        throw std::invalid_argument("volume требует 1 параметр");
    }

    double volumeFactor = parseDouble(params[0], "коэффициент", "volume");

    if (volumeFactor < 0) {
        throw std::invalid_argument("volume: коэффициент не может быть отрицательным");
    }

    if (volumeFactor > 10.0) {
        throw std::invalid_argument("volume: коэффициент слишком большой (макс 10.0)");
    }

    return std::make_unique<VolumeConverter>(volumeFactor);
}

double ConverterFactory::parseDouble(const std::string& str,
                                    const std::string& paramName,
                                    const std::string& converterName) {
    try {
        return std::stod(str);
    } catch (const std::exception&) {
        std::stringstream ss;
        ss << converterName << ": параметр '" << paramName
           << "' должен быть числом, получено '" << str << "'";
        throw std::invalid_argument(ss.str());
    }
}

int ConverterFactory::parseStreamReference(const std::string& str,
                                          const std::string& converterName) {
    if (str.empty() || str[0] != '$') {
        std::stringstream ss;
        ss << converterName << ": ссылка на поток должна начинаться с '$', получено '" << str << "'";
        throw std::invalid_argument(ss.str());
    }

    try {
        int streamNumber = std::stoi(str.substr(1));

        if (streamNumber <= 0) {
            std::stringstream ss;
            ss << converterName << ": номер потока должен быть положительным, получено $" << streamNumber;
            throw std::invalid_argument(ss.str());
        }

        return streamNumber;

    } catch (const std::exception&) {
        std::stringstream ss;
        ss << converterName << ": неверная ссылка на поток '" << str << "'";
        throw std::invalid_argument(ss.str());
    }
}

std::vector<std::string> ConverterFactory::getAvailableConverters() {
    return {"mute", "mix", "volume"};
}