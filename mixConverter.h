//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_MIXCONVERTER_H
#define SOUNDPROCESSOR_MIXCONVERTER_H

#include "converter.h"
#include "wavFile.h"
#include <vector>
#include <memory>
#include <string>

class MixConverter : public Converter {
private:
    int streamNumber_;
    double offsetSeconds_;

    mutable std::vector<int16_t> cachedExtraSamples_;
    mutable size_t cachedExtraStart_ = 0;
    mutable bool extraStreamPrepared_ = false;

    static const size_t CACHE_SIZE = 44100;

public:
    MixConverter(int streamNumber, double offsetSeconds);

    AudioBuffer process(const AudioBuffer& input,
                     const std::vector<AudioBuffer>& extraStreams) const override;

    AudioBuffer processChunk(const AudioBuffer& chunk,
                           size_t globalSampleOffset,
                           const std::vector<std::unique_ptr<WavFile>>& extraStreams) const override;

    bool requiresFullStreamAccess() const override { return false; }

    void prepareForStreaming(const std::vector<std::unique_ptr<WavFile>>& streams) override;

    std::string getName() const override;
    std::string getDescription() const override;

private:
    int16_t mixTwoSamples(int16_t sample1, int16_t sample2) const;

    void clearCache() const {
        cachedExtraSamples_.clear();
        cachedExtraStart_ = 0;
        extraStreamPrepared_ = false;
    }
};

#endif //SOUNDPROCESSOR_MIXCONVERTER_H