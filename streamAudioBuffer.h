//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_STREAMAUDIOBUFFER_H
#define SOUNDPROCESSOR_STREAMAUDIOBUFFER_H

#include "audoiBuffer.h"
#include "wavFile.h"
#include <memory>
#include <functional>

class StreamAudioBuffer {
public:
    StreamAudioBuffer(std::unique_ptr<WavFile> wavFile, size_t chunkSize = 44100);
    StreamAudioBuffer(const std::string& filename, size_t chunkSize = 44100);

    bool hasNextChunk() const;
    AudioBuffer getNextChunk();
    void reset();

    size_t getTotalSamples() const;
    size_t getProcessedSamples() const;
    double getProgress() const;

private:
    std::unique_ptr<WavFile> wavFile_;
    size_t chunkSize_;
    size_t processedSamples_;
    size_t totalSamples_;
};
#endif //SOUNDPROCESSOR_STREAMAUDIOBUFFER_H