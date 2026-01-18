//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_MUTECONVERTER_H
#define SOUNDPROCESSOR_MUTECONVERTER_H

#include "converter.h"
#include "wavFile.h"
#include <vector>
#include <memory>
#include <string>

class MuteConverter : public Converter {
private:
    double startSeconds_;
    double endSeconds_;

    mutable size_t startSamples_ = 0;
    mutable size_t endSamples_ = 0;
    mutable bool samplesCalculated_ = false;

    static const uint32_t SAMPLE_RATE = 44100;

public:
    MuteConverter(double startSeconds, double endSeconds);

    AudioBuffer process(const AudioBuffer& input,
                     const std::vector<AudioBuffer>& extraStreams) const override;

    AudioBuffer processChunk(const AudioBuffer& chunk,
                           size_t globalSampleOffset,
                           const std::vector<std::unique_ptr<WavFile>>& extraStreams) const override;

    bool requiresFullStreamAccess() const override { return false; }

    void prepareForStreaming(const std::vector<std::unique_ptr<WavFile>>& streams) override {
        if (!samplesCalculated_) {
            startSamples_ = static_cast<size_t>(startSeconds_ * SAMPLE_RATE);
            endSamples_ = static_cast<size_t>(endSeconds_ * SAMPLE_RATE);
            samplesCalculated_ = true;
        }
    }

    std::string getName() const override;
    std::string getDescription() const override;

    double getStartSeconds() const { return startSeconds_; }
    double getEndSeconds() const { return endSeconds_; }
    size_t getStartSamples() const {
        if (!samplesCalculated_) {
            startSamples_ = static_cast<size_t>(startSeconds_ * SAMPLE_RATE);
            samplesCalculated_ = true;
        }
        return startSamples_;
    }
    size_t getEndSamples() const {
        if (!samplesCalculated_) {
            endSamples_ = static_cast<size_t>(endSeconds_ * SAMPLE_RATE);
            samplesCalculated_ = true;
        }
        return endSamples_;
    }

private:
    void calculateSamples() const {
        if (!samplesCalculated_) {
            startSamples_ = static_cast<size_t>(startSeconds_ * SAMPLE_RATE);
            endSamples_ = static_cast<size_t>(endSeconds_ * SAMPLE_RATE);
            samplesCalculated_ = true;
        }
    }

    bool shouldMuteChunk(size_t chunkStart, size_t chunkSize) const;

    void getChunkMuteBounds(size_t chunkStart, size_t chunkSize,
                           size_t& muteStart, size_t& muteEnd) const;

    void applyMuteToBuffer(AudioBuffer& buffer, size_t muteStart, size_t muteEnd) const;
};
#endif //SOUNDPROCESSOR_MUTECONVERTER_H