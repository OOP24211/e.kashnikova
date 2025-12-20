//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_SOUNDPROCESSOR_H
#define SOUNDPROCESSOR_SOUNDPROCESSOR_H

#include <string>
#include <vector>
#include <memory>
#include "wavFile.h"
#include "streamAudioBuffer.h"
#include "converter.h"

class SoundProcessor {
public:
    SoundProcessor();

    int run(int argc, char* argv[]);
    void processWithFullAccess(
        const std::string& configFile,
        const std::string& outputFile,
        const std::vector<std::string>& inputFiles) const;
    void processStreaming(
        StreamAudioBuffer& mainStream,
        const std::vector<std::unique_ptr<WavFile>>& inputStreams,
        const std::vector<std::unique_ptr<Converter>>& converters,
        const std::string& outputFile) const;


private:
    struct Arguments parseArguments(int argc, char* argv[]) const;
    void process(const std::string& configFile,
                 const std::string& outputFile,
                 const std::vector<std::string>& inputFiles) const;
};


#endif //SOUNDPROCESSOR_SOUNDPROCESSOR_H