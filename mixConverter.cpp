//
// Created by Эвелина on 16.12.2025.
//

#include "mixConverter.h"
#include <algorithm>
#include <stdexcept>
#include <cmath>
#include <iostream>

using Sample = int16_t;

MixConverter::MixConverter(int streamNumber, double offsetSeconds)
    : streamNumber_(streamNumber), offsetSeconds_(offsetSeconds) {

    if (streamNumber_ <= 0) {
        throw std::invalid_argument("Номер потока должен быть положительным числом");
    }

    if (offsetSeconds_ < 0) {
        throw std::invalid_argument("Время смещения не может быть отрицательным");
    }
}

void MixConverter::prepareForStreaming(const std::vector<std::unique_ptr<WavFile>>& streams) {
    clearCache();

    if (streamNumber_ <= 0 || static_cast<size_t>(streamNumber_) > streams.size()) {
        extraStreamPrepared_ = false;
        return;
    }

    const auto& extraStream = streams[streamNumber_ - 1];
    if (!extraStream || !extraStream->isOpenedForReading()) {
        extraStreamPrepared_ = false;
        return;
    }

   try {
        cachedExtraStart_ = 0;
        cachedExtraSamples_.resize(CACHE_SIZE, 0);

        std::vector<int16_t> temp;
        if (extraStream->readNextChunk(temp, CACHE_SIZE)) {
            if (temp.size() < CACHE_SIZE) {
                cachedExtraSamples_.resize(temp.size());
            }
            std::copy(temp.begin(), temp.end(), cachedExtraSamples_.begin());
            extraStreamPrepared_ = true;
        }

        extraStream->resetReading();

    } catch (const std::exception& e) {
        std::cerr << "Предупреждение: не удалось кэшировать поток для микширования: "
                  << e.what() << std::endl;
        clearCache();
    }
}

AudioBuffer MixConverter::process(const AudioBuffer& input,
                                const std::vector<AudioBuffer>& extraStreams) const {
    if (streamNumber_ <= 0 || static_cast<size_t>(streamNumber_) > extraStreams.size()) {
        return input;
    }

    const AudioBuffer& otherStream = extraStreams[streamNumber_ - 1];

    size_t offset = static_cast<size_t>(offsetSeconds_ * 44100);

    AudioBuffer result = input;

    for (size_t i = 0; i < otherStream.size(); ++i) {
        size_t pos = offset + i;
        if (pos >= result.size()) break;
        result[pos] = mixTwoSamples(result[pos], otherStream[i]);
    }

    return result;
}

AudioBuffer MixConverter::processChunk(const AudioBuffer& chunk,
                                     size_t globalSampleOffset,
                                     const std::vector<std::unique_ptr<WavFile>>& extraStreams) const {

    if (streamNumber_ <= 0 || static_cast<size_t>(streamNumber_) > extraStreams.size()) {
        return chunk;
    }

    WavFile* extraStream = extraStreams[streamNumber_ - 1].get();
    if (!extraStream || !extraStream->isOpenedForReading()) {
        return chunk;
    }

    size_t mixStartSample = static_cast<size_t>(offsetSeconds_ * 44100);

    size_t chunkStart = globalSampleOffset;
    size_t chunkEnd = globalSampleOffset + chunk.size();

    if (chunkEnd <= mixStartSample ||
        chunkStart >= mixStartSample + extraStream->getTotalSamples()) {
        return chunk;
    }

    size_t extraReadStart = 0;
    size_t extraReadCount = 0;
    size_t chunkMixStart = 0;

    if (chunkStart < mixStartSample) {
        chunkMixStart = mixStartSample - chunkStart;
        extraReadStart = 0;
    } else {
        chunkMixStart = 0;
        extraReadStart = chunkStart - mixStartSample;
    }

    extraReadCount = std::min(
        chunk.size() - chunkMixStart,
        extraStream->getTotalSamples() - extraReadStart
    );

    if (extraReadCount == 0) {
        return chunk;
    }

    std::vector<int16_t> extraSamples(extraReadCount, 0);
    bool samplesLoaded = false;

    if (extraStreamPrepared_ &&
        extraReadStart >= cachedExtraStart_ &&
        extraReadStart + extraReadCount <= cachedExtraStart_ + cachedExtraSamples_.size()) {

        size_t cacheOffset = extraReadStart - cachedExtraStart_;
        std::copy(cachedExtraSamples_.begin() + cacheOffset,
                 cachedExtraSamples_.begin() + cacheOffset + extraReadCount,
                 extraSamples.begin());
        samplesLoaded = true;
    }

    if (!samplesLoaded) {
        try {
            auto originalPos = extraStream->file_.tellg();

            size_t byteOffset = extraStream->dataStartPos_ +
                               extraReadStart * sizeof(int16_t);
            extraStream->file_.seekg(byteOffset);

            extraStream->file_.read(reinterpret_cast<char*>(extraSamples.data()),
                                   extraReadCount * sizeof(int16_t));

            extraStream->file_.seekg(originalPos);
            samplesLoaded = true;

        } catch (const std::exception& e) {
            std::cerr << "Ошибка чтения дополнительного потока: "
                      << e.what() << std::endl;
            return chunk;
        }
    }

    AudioBuffer result = chunk;

    for (size_t i = 0; i < extraReadCount; ++i) {
        size_t posInChunk = chunkMixStart + i;
        if (posInChunk >= result.size()) {
            break;
        }

        int16_t mainSample = result.getSample(posInChunk);
        int16_t extraSample = extraSamples[i];

        int32_t mixed = static_cast<int32_t>(mainSample) +
                       static_cast<int32_t>(extraSample);
        mixed = mixed / 2;

        if (mixed > 32767) {
            mixed = 32767;
        } else if (mixed < -32768) {
            mixed = -32768;
        }

        if (posInChunk < result.samples_.size()) {
            result.samples_[posInChunk] = static_cast<int16_t>(mixed);
        }
    }

    return result;
}

int16_t MixConverter::mixTwoSamples(int16_t sample1, int16_t sample2) const {
    int32_t mixed = static_cast<int32_t>(sample1) +
                   static_cast<int32_t>(sample2);

    mixed = mixed / 2;

    if (mixed > 32767) {
        mixed = 32767;
    } else if (mixed < -32768) {
        mixed = -32768;
    }

    return static_cast<Sample>(mixed);
}

std::string MixConverter::getName() const {
    return "mix";
}

std::string MixConverter::getDescription() const {
    return "Смешать с другим аудиопотоком (нужен дополнительный файл)";
}
