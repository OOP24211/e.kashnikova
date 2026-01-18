//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_CONVERTER_H
#define SOUNDPROCESSOR_CONVERTER_H

#include "audoiBuffer.h"
#include <vector>
#include <memory>
#include "wavFile.h"

class Converter {
public:
    virtual ~Converter() = default;

    virtual AudioBuffer process(const AudioBuffer& input,
                             const std::vector<AudioBuffer>& extraStreams) const = 0;

    virtual AudioBuffer processChunk(const AudioBuffer& chunk,
                                    size_t globalSampleOffset,
                                    const std::vector<std::unique_ptr<WavFile>>& extraStreams) const = 0;

    virtual std::string getName() const = 0;
    virtual std::string getDescription() const = 0;

    virtual bool requiresFullStreamAccess() const { return false; }
    virtual void prepareForStreaming(const std::vector<std::unique_ptr<WavFile>>& streams) {}
};

#endif //SOUNDPROCESSOR_CONVERTER_H