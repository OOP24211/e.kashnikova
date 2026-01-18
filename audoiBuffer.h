//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_AUDOIBUFFER_H
#define SOUNDPROCESSOR_AUDOIBUFFER_H


#include <vector>
#include <cstdint>
#include <memory>
#include "exceptions.h"

class AudioBuffer {
public:
    class BufferError : public std::runtime_error {
    public:
        explicit BufferError(const std::string& message)
            : std::runtime_error("AudioBuffer error: " + message) {}
    };

    class IndexError : public BufferError {
    public:
        explicit IndexError(size_t index, size_t size)
            : BufferError("Index " + std::to_string(index) +
                         " out of bounds for buffer size " + std::to_string(size)) {}
    };

public:
    AudioBuffer();
    explicit AudioBuffer(size_t initialCapacity);
    AudioBuffer(const std::vector<int16_t>& samples);
    AudioBuffer(const int16_t* data, size_t size);

    size_t size() const;
    bool empty() const;
    void clear();

    int16_t getSample(size_t index) const;
    const int16_t* data() const;

    void appendSample(int16_t sample);
    void appendSamples(const int16_t* samples, size_t count);
    void appendBuffer(const AudioBuffer& other);

    void ensureCapacity(size_t capacity);

    void applyGain(double gain);
    AudioBuffer mixWith(const AudioBuffer& other, size_t offset = 0) const;

    static int16_t mixSamples(int16_t sample1, int16_t sample2);
    static int16_t applyGainToSample(int16_t sample, double gain);

    int16_t& operator[](size_t index) {
        if (index >= samples_.size()) {
            throw IndexError(index, samples_.size());
        }
        return samples_[index];
    }

    const int16_t& operator[](size_t index) const {
        if (index >= samples_.size()) {
            throw IndexError(index, samples_.size());
        }
        return samples_[index];
    }
    using iterator = std::vector<int16_t>::iterator;
    using const_iterator = std::vector<int16_t>::const_iterator;

    iterator begin() { return samples_.begin(); }
    iterator end() { return samples_.end(); }
    const_iterator begin() const { return samples_.begin(); }
    const_iterator end() const { return samples_.end(); }
    const_iterator cbegin() const { return samples_.cbegin(); }
    const_iterator cend() const { return samples_.cend(); }

private:
    std::vector<int16_t> samples_;
    size_t currentPos_;
    friend class MixConverter;
    friend class MuteConverter;
    friend class VolumeConverter;
};

#endif //SOUNDPROCESSOR_AUDOIBUFFER_H