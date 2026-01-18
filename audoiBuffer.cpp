//
// Created by Эвелина on 16.12.2025.
//

#include "audoiBuffer.h"
#include <cmath>

AudioBuffer::AudioBuffer()
    : samples_()
    , currentPos_(0) {}

AudioBuffer::AudioBuffer(size_t initialCapacity)
    : samples_()
    , currentPos_(0) {
    ensureCapacity(initialCapacity);
}

AudioBuffer::AudioBuffer(const std::vector<int16_t>& samples)
    : samples_(samples)
    , currentPos_(0) {}

AudioBuffer::AudioBuffer(const int16_t* data, size_t size)
    : samples_(data, data + size)
    , currentPos_(0) {}

size_t AudioBuffer::size() const {
    return samples_.size();
}

bool AudioBuffer::empty() const {
    return samples_.empty();
}

void AudioBuffer::clear() {
    samples_.clear();
    currentPos_ = 0;
}

int16_t AudioBuffer::getSample(size_t index) const {
    if (index >= samples_.size()) {
        throw IndexError(index, samples_.size());
    }
    return samples_[index];
}

const int16_t* AudioBuffer::data() const {
    return samples_.data();
}

void AudioBuffer::appendSample(int16_t sample) {
    samples_.push_back(sample);
}

void AudioBuffer::appendSamples(const int16_t* samples, size_t count) {
    if (samples == nullptr && count > 0) {
        throw BufferError("Null pointer passed to appendSamples");
    }
    samples_.insert(samples_.end(), samples, samples + count);
}

void AudioBuffer::appendBuffer(const AudioBuffer& other) {
    samples_.insert(samples_.end(), other.samples_.begin(), other.samples_.end());
}

void AudioBuffer::ensureCapacity(size_t capacity) {
    if (capacity > samples_.capacity()) {
        samples_.reserve(capacity);
    }
}

void AudioBuffer::applyGain(double gain) {
    if (std::abs(gain - 1.0) < 0.0001) {
        return;
    }

    for (auto& sample : samples_) {
        sample = applyGainToSample(sample, gain);
    }
}

AudioBuffer AudioBuffer::mixWith(const AudioBuffer& other, size_t offset) const {
    size_t outputSize = std::max(samples_.size(), other.samples_.size() + offset);
    AudioBuffer result(outputSize);

    result.samples_.resize(outputSize, 0);
    std::copy(samples_.begin(), samples_.end(), result.samples_.begin());

    size_t mixStart = offset;
    size_t mixEnd = std::min(offset + other.samples_.size(), outputSize);

    for (size_t i = mixStart, j = 0; i < mixEnd; ++i, ++j) {
        result.samples_[i] = mixSamples(result.samples_[i], other.samples_[j]);
    }

    return result;
}

int16_t AudioBuffer::mixSamples(int16_t sample1, int16_t sample2) {
    int32_t mixed = static_cast<int32_t>(sample1) + static_cast<int32_t>(sample2);

    if (mixed > 32767) {
        return 32767;
    }
    if (mixed < -32768) {
        return -32768;
    }

    return static_cast<int16_t>(mixed / 2);
}

int16_t AudioBuffer::applyGainToSample(int16_t sample, double gain) {
    double amplified = static_cast<double>(sample) * gain;

    if (amplified > 32767.0) {
        return 32767;
    }
    if (amplified < -32768.0) {
        return -32768;
    }

    return static_cast<int16_t>(amplified + 0.5);
}