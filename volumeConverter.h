//
// Created by Эвелина on 16.12.2025.
//

#ifndef SOUNDPROCESSOR_VOLUMECONVERTER_H
#define SOUNDPROCESSOR_VOLUMECONVERTER_H

#include "converter.h"
#include "wavFile.h"
#include <vector>
#include <memory>
#include <string>
#include <cmath>

class VolumeConverter : public Converter {
private:
    double volumeFactor_;

    mutable bool factorValidated_ = false;

    static constexpr double MIN_VOLUME_FACTOR = 0.0;
    static constexpr double MAX_VOLUME_FACTOR = 10.0;
    static constexpr double EPSILON = 0.0001;

public:
    explicit VolumeConverter(double volumeFactor);

    AudioBuffer process(const AudioBuffer& input,
                     const std::vector<AudioBuffer>& extraStreams) const override;

    AudioBuffer processChunk(const AudioBuffer& chunk,
                           size_t globalSampleOffset,
                           const std::vector<std::unique_ptr<WavFile>>& extraStreams) const override;

    bool requiresFullStreamAccess() const override { return false; }

    void prepareForStreaming(const std::vector<std::unique_ptr<WavFile>>& streams) override {
        validateVolumeFactor();
    }

    std::string getName() const override;
    std::string getDescription() const override;

    double getVolumeFactor() const { return volumeFactor_; }
    bool isNoOp() const { return std::abs(volumeFactor_ - 1.0) < EPSILON; }
    bool isMute() const { return std::abs(volumeFactor_) < EPSILON; }

    static double clampVolumeFactor(double factor);
    static bool isValidVolumeFactor(double factor);

private:
    void validateVolumeFactor() const;

    AudioBuffer processWithGain(AudioBuffer& buffer) const;
    AudioBuffer processSilence(AudioBuffer& buffer) const;
    AudioBuffer processNoOp(AudioBuffer& buffer) const;

    void applyLinearGain(AudioBuffer& buffer) const;
    void applySaturationGain(AudioBuffer& buffer) const;

    static int16_t applyGainToSample(int16_t sample, double gain);
    static int16_t applyGainWithSaturation(int16_t sample, double gain);

    static bool willCauseSaturation(int16_t sample, double gain);
};
#endif //SOUNDPROCESSOR_VOLUMECONVERTER_H