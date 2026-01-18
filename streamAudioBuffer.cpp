//
// Created by Эвелина on 16.12.2025.
//

#include "streamAudioBuffer.h"
#include <functional>

StreamAudioBuffer::StreamAudioBuffer(std::unique_ptr<WavFile> wavFile, size_t chunkSize)
    : wavFile_(std::move(wavFile))
    , chunkSize_(chunkSize)
    , processedSamples_(0)
    , totalSamples_(0) {

    if (!wavFile_) {
        throw AudioBuffer::BufferError("Null WavFile pointer");
    }

    if (!wavFile_->isOpenedForReading()) {
        throw AudioBuffer::BufferError("WavFile not opened for reading");
    }

    totalSamples_ = wavFile_->getTotalSamples();
}

StreamAudioBuffer::StreamAudioBuffer(const std::string& filename, size_t chunkSize)
    : chunkSize_(chunkSize)
    , processedSamples_(0)
    , totalSamples_(0) {

    wavFile_ = std::make_unique<WavFile>();
    wavFile_->openForReading(filename);
    totalSamples_ = wavFile_->getTotalSamples();
}

bool StreamAudioBuffer::hasNextChunk() const {
    if (!wavFile_) {
        return false;
    }
    return !wavFile_->isEof();
}

AudioBuffer StreamAudioBuffer::getNextChunk() {
    if (!hasNextChunk()) {
        throw AudioBuffer::BufferError("No more chunks available");
    }

    AudioBuffer buffer(chunkSize_);
    std::vector<int16_t> tempBuffer;

    if (wavFile_->readNextChunk(tempBuffer, chunkSize_)) {
        buffer.appendSamples(tempBuffer.data(), tempBuffer.size());
        processedSamples_ += tempBuffer.size();
    }

    return buffer;
}

void StreamAudioBuffer::reset() {
    if (wavFile_) {
        wavFile_->resetReading();
        processedSamples_ = 0;
    }
}

size_t StreamAudioBuffer::getTotalSamples() const {
    return totalSamples_;
}

size_t StreamAudioBuffer::getProcessedSamples() const {
    return processedSamples_;
}

double StreamAudioBuffer::getProgress() const {
    if (totalSamples_ == 0) {
        return 0.0;
    }
    return static_cast<double>(processedSamples_) / totalSamples_;
}
