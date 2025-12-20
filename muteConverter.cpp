//
// Created by Эвелина on 16.12.2025.
//

#include "muteConverter.h"
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <iostream>

MuteConverter::MuteConverter(double startSeconds, double endSeconds)
    : startSeconds_(startSeconds), endSeconds_(endSeconds) {

    if (startSeconds_ < 0) {
        throw std::invalid_argument("Время начала не может быть отрицательным");
    }

    if (endSeconds_ < 0) {
        throw std::invalid_argument("Время конца не может быть отрицательным");
    }

    if (startSeconds_ > endSeconds_) {
        std::swap(startSeconds_, endSeconds_);
        std::cout << "Предупреждение: начало интервала заглушения ("
                  << endSeconds_ << ") больше конца (" << startSeconds_
                  << "). Параметры поменяны местами.\n";
    }
    calculateSamples();
}

AudioBuffer MuteConverter::process(const AudioBuffer& input,
                                const std::vector<AudioBuffer>& extraStreams) const {

    calculateSamples();

    AudioBuffer result = input;

    if (startSamples_ >= result.size()) {
        return result;
    }

    size_t actualEnd = std::min(endSamples_, result.size());

    if (startSamples_ < actualEnd) {
        std::fill(result.begin() + startSamples_,
                 result.begin() + actualEnd,
                 0);
    }

    return result;
}

AudioBuffer MuteConverter::processChunk(const AudioBuffer& chunk,
                                      size_t globalSampleOffset,
                                      const std::vector<std::unique_ptr<WavFile>>& extraStreams) const {

    calculateSamples();

    AudioBuffer result = chunk;

    if (!shouldMuteChunk(globalSampleOffset, chunk.size())) {
        return result;
    }

    size_t muteStart = 0;
    size_t muteEnd = 0;
    getChunkMuteBounds(globalSampleOffset, chunk.size(), muteStart, muteEnd);

    if (muteStart < muteEnd) {
        applyMuteToBuffer(result, muteStart, muteEnd);
    }

    return result;
}

bool MuteConverter::shouldMuteChunk(size_t chunkStart, size_t chunkSize) const {

    size_t chunkEnd = chunkStart + chunkSize;
    if (chunkEnd <= startSamples_) {
        return false;
    }

    if (chunkStart >= endSamples_) {
        return false;
    }
    return true;
}

void MuteConverter::getChunkMuteBounds(size_t chunkStart, size_t chunkSize,
                                      size_t& muteStart, size_t& muteEnd) const {

    size_t chunkEnd = chunkStart + chunkSize;

    if (chunkStart < startSamples_) {
        muteStart = startSamples_ - chunkStart;
    } else {
        muteStart = 0;
    }

    if (chunkEnd > endSamples_) {
        muteEnd = endSamples_ - chunkStart;
    } else {
        muteEnd = chunkSize;
    }

    if (muteStart > muteEnd) {
        muteStart = 0;
        muteEnd = 0;
    }

    muteStart = std::min(muteStart, chunkSize);
    muteEnd = std::min(muteEnd, chunkSize);
}

void MuteConverter::applyMuteToBuffer(AudioBuffer& buffer,
                                     size_t muteStart,
                                     size_t muteEnd) const {

    if (muteStart >= muteEnd || muteStart >= buffer.size()) {
        return;
    }

    muteEnd = std::min(muteEnd, buffer.size());

    for (size_t i = muteStart; i < muteEnd; ++i) {
        if (i < buffer.samples_.size()) {
            buffer.samples_[i] = 0;
        }
    }
}

std::string MuteConverter::getName() const {
    return "mute";
}

std::string MuteConverter::getDescription() const {
    std::string desc = "Заглушить звук с " +
                      std::to_string(startSeconds_) +
                      " по " +
                      std::to_string(endSeconds_) +
                      " секунд";
    return desc;
}