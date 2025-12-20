//
// Created by Эвелина on 16.12.2025.
//

#include "soundProcessor.h"
#include "wavFile.h"
#include "configParser.h"
#include "converterFactory.h"
#include "streamAudioBuffer.h"
#include <iostream>
#include <cstring>
#include <stdexcept>
#include <memory>

struct Arguments {
    bool showHelp = false;
    std::string configFile;
    std::string outputFile;
    std::vector<std::string> inputFiles;
};

SoundProcessor::SoundProcessor() = default;

int SoundProcessor::run(int argc, char* argv[]) {
    try {
        Arguments args = parseArguments(argc, argv);

        process(args.configFile, args.outputFile, args.inputFiles);

        return 0;

    } catch (const std::invalid_argument& e) {
        std::cerr << "Ошибка аргументов: " << e.what() << std::endl;
        return 1;
    } catch (const std::runtime_error& e) {
        std::cerr << "Ошибка выполнения: " << e.what() << std::endl;
        return 2;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 3;
    }
}

Arguments SoundProcessor::parseArguments(int argc, char* argv[]) const {
    Arguments args;

    if (argc == 1) {
        throw std::invalid_argument("Не указаны аргументы");
    }

    if (argc == 2 && std::strcmp(argv[1], "-h") == 0) {
        args.showHelp = true;
        return args;
    }

    if (argc < 5) {
        throw std::invalid_argument("Недостаточно аргументов");
    }

    if (std::strcmp(argv[1], "-c") != 0) {
        throw std::invalid_argument("Первый аргумент должен быть -c");
    }

    args.configFile = argv[2];
    args.outputFile = argv[3];

    for (int i = 4; i < argc; ++i) {
        args.inputFiles.push_back(argv[i]);
    }

    return args;
}

void SoundProcessor::process(const std::string& configFile,
                            const std::string& outputFile,
                            const std::vector<std::string>& inputFiles) const {

    if (inputFiles.empty()) {
        throw std::invalid_argument("Не указаны входные файлы");
    }

    std::vector<std::unique_ptr<WavFile>> inputStreams;
    for (const auto& filename : inputFiles) {
        auto wavFile = std::make_unique<WavFile>();
        wavFile->openForReading(filename);
        inputStreams.push_back(std::move(wavFile));
    }

    StreamAudioBuffer mainStream(inputFiles[0], 44100); // Чанки по 1 секунде

    std::vector<AudioBuffer> dummyStreams;
    auto converters = ConfigParser::parse(configFile, dummyStreams);

    for (const auto& converter : converters) {
        converter->prepareForStreaming(inputStreams);
    }

    bool requiresFullAccess = false;
    for (const auto& converter : converters) {
        if (converter->requiresFullStreamAccess()) {
            requiresFullAccess = true;
            break;
        }
    }

    if (requiresFullAccess) {
        processWithFullAccess(configFile, outputFile, inputFiles);
    } else {
        processStreaming(mainStream, inputStreams, converters, outputFile);
    }
}

void SoundProcessor::processStreaming(
    StreamAudioBuffer& mainStream,
    const std::vector<std::unique_ptr<WavFile>>& inputStreams,
    const std::vector<std::unique_ptr<Converter>>& converters,
    const std::string& outputFile) const {

    WavFile output;
    output.openForWriting(outputFile, 44100);

    size_t globalSampleOffset = 0;
    const size_t chunkSize = 44100;


    while (mainStream.hasNextChunk()) {
        AudioBuffer chunk = mainStream.getNextChunk();

        AudioBuffer processedChunk = chunk;

        for (const auto& converter : converters) {
            processedChunk = converter->processChunk(
                processedChunk,
                globalSampleOffset,
                inputStreams
            );
        }

        std::vector<int16_t> temp(
            processedChunk.data(),
            processedChunk.data() + processedChunk.size()
        );
        output.writeChunk(temp);

        globalSampleOffset += processedChunk.size();
    }

    for (const auto& stream : inputStreams) {
        stream->close();
    }
    output.close();

}

void SoundProcessor::processWithFullAccess(
    const std::string& configFile,
    const std::string& outputFile,
    const std::vector<std::string>& inputFiles) const {

    std::vector<AudioBuffer> streams;
    for (const auto& file : inputFiles) {
        WavFile wav;
        wav.openForReading(file);

        AudioBuffer data;
        std::vector<int16_t> buffer;

        while (wav.readNextChunk(buffer, 44100)) {
            data.appendSamples(buffer.data(), buffer.size());
        }

        wav.close();
        streams.push_back(data);
    }

    auto converters = ConfigParser::parse(configFile, streams);

    AudioBuffer result = streams[0];
    for (const auto& converter : converters) {
        result = converter->process(result, streams);
    }

    WavFile output;
    output.openForWriting(outputFile, 44100);

    const size_t CHUNK_SIZE = 44100;
    for (size_t i = 0; i < result.size(); i += CHUNK_SIZE) {
        size_t size = std::min(CHUNK_SIZE, result.size() - i);
        std::vector<int16_t> chunk(result.begin() + i, result.begin() + i + size);
        output.writeChunk(chunk);
    }

    output.close();

}

