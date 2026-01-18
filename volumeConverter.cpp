//
// Created by Эвелина on 16.12.2025.
//

#include "volumeConverter.h"
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <iostream>

VolumeConverter::VolumeConverter(double volumeFactor)
    : volumeFactor_(volumeFactor) {

    validateVolumeFactor();

    volumeFactor_ = clampVolumeFactor(volumeFactor_);
}

void VolumeConverter::validateVolumeFactor() const {
    if (!isValidVolumeFactor(volumeFactor_)) {
        throw std::invalid_argument(
            "Коэффициент громкости должен быть в диапазоне [" +
            std::to_string(MIN_VOLUME_FACTOR) + ", " +
            std::to_string(MAX_VOLUME_FACTOR) + "], получено: " +
            std::to_string(volumeFactor_)
        );
    }
    factorValidated_ = true;
}

AudioBuffer VolumeConverter::process(const AudioBuffer& input,
                                  const std::vector<AudioBuffer>& extraStreams) const {

    if (!factorValidated_) {
        validateVolumeFactor();
    }

    if (isNoOp()) {
        return input;
    }

    if (isMute()) {
        AudioBuffer result(input.size());
        return result;
    }

    AudioBuffer result(input.size());

    for (size_t i = 0; i < input.size(); ++i) {
        int16_t sample = input.getSample(i);
        int16_t processed = applyGainToSample(sample, volumeFactor_);
        result.appendSample(processed);
    }

    return result;
}

AudioBuffer VolumeConverter::processChunk(const AudioBuffer& chunk,
                                        size_t globalSampleOffset,
                                        const std::vector<std::unique_ptr<WavFile>>& extraStreams) const {

    if (!factorValidated_) {
        validateVolumeFactor();
    }

    AudioBuffer result = chunk;

    if (isNoOp()) {
        return processNoOp(result);
    } else if (isMute()) {
        return processSilence(result);
    } else {
        return processWithGain(result);
    }
}

AudioBuffer VolumeConverter::processNoOp(AudioBuffer& buffer) const {
    return buffer;
}

AudioBuffer VolumeConverter::processSilence(AudioBuffer& buffer) const {
    for (size_t i = 0; i < buffer.size(); ++i) {
        if (i < buffer.samples_.size()) {
            buffer.samples_[i] = 0;
        }
    }
    return buffer;
}

AudioBuffer VolumeConverter::processWithGain(AudioBuffer& buffer) const {
    bool potentialSaturation = false;

    const size_t CHECK_COUNT = std::min<size_t>(10, buffer.size());
    for (size_t i = 0; i < CHECK_COUNT; ++i) {
        if (i < buffer.samples_.size() &&
            willCauseSaturation(buffer.samples_[i], volumeFactor_)) {
            potentialSaturation = true;
            break;
        }
    }

    if (potentialSaturation || volumeFactor_ > 2.0) {
        applySaturationGain(buffer);
    } else {
        applyLinearGain(buffer);
    }

    return buffer;
}

void VolumeConverter::applyLinearGain(AudioBuffer& buffer) const {
    for (size_t i = 0; i < buffer.size(); ++i) {
        if (i < buffer.samples_.size()) {
            int16_t sample = buffer.samples_[i];
            double amplified = static_cast<double>(sample) * volumeFactor_;

            if (amplified > 32767.0) {
                buffer.samples_[i] = 32767;
            } else if (amplified < -32768.0) {
                buffer.samples_[i] = -32768;
            } else {
                buffer.samples_[i] = static_cast<int16_t>(amplified + 0.5);
            }
        }
    }
}

void VolumeConverter::applySaturationGain(AudioBuffer& buffer) const {
    for (size_t i = 0; i < buffer.size(); ++i) {
        if (i < buffer.samples_.size()) {
            buffer.samples_[i] = applyGainWithSaturation(
                buffer.samples_[i],
                volumeFactor_
            );
        }
    }
}

int16_t VolumeConverter::applyGainToSample(int16_t sample, double gain) {
    double amplified = static_cast<double>(sample) * gain;

    if (amplified > 32767.0) {
        return 32767;
    }
    if (amplified < -32768.0) {
        return -32768;
    }

    return static_cast<int16_t>(amplified + 0.5);
}

int16_t VolumeConverter::applyGainWithSaturation(int16_t sample, double gain) {
    int32_t result = static_cast<int32_t>(sample) * static_cast<int32_t>(gain * 1000) / 1000;

    if (result > 32767) {
        return 32767;
    }
    if (result < -32768) {
        return -32768;
    }

    return static_cast<int16_t>(result);
}

bool VolumeConverter::willCauseSaturation(int16_t sample, double gain) {
    const double ABS_MAX = 32767.0;
    double absSample = std::abs(static_cast<double>(sample));

    return absSample * gain > ABS_MAX * 0.9;
}

double VolumeConverter::clampVolumeFactor(double factor) {
    if (factor < MIN_VOLUME_FACTOR) {
        std::cerr << "Предупреждение: коэффициент громкости "
                  << factor << " ограничен до " << MIN_VOLUME_FACTOR << std::endl;
        return MIN_VOLUME_FACTOR;
    }

    if (factor > MAX_VOLUME_FACTOR) {
        std::cerr << "Предупреждение: коэффициент громкости "
                  << factor << " ограничен до " << MAX_VOLUME_FACTOR << std::endl;
        return MAX_VOLUME_FACTOR;
    }

    return factor;
}

bool VolumeConverter::isValidVolumeFactor(double factor) {
    if (std::isnan(factor) || std::isinf(factor)) {
        return false;
    }

    return factor >= MIN_VOLUME_FACTOR && factor <= MAX_VOLUME_FACTOR;
}

std::string VolumeConverter::getName() const {
    return "volume";
}

std::string VolumeConverter::getDescription() const {
    std::string desc = "Изменить громкость с коэффициентом " +
                      std::to_string(volumeFactor_);

    if (isNoOp()) {
        desc += " (без изменений)";
    } else if (isMute()) {
        desc += " (тишина)";
    } else if (volumeFactor_ > 1.0) {
        desc += " (усиление)";
    } else {
        desc += " (ослабление)";
    }

    return desc;
}