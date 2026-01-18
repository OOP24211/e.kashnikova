//
// Created by Эвелина on 15.12.2025.
//

#ifndef SOUNDPROCESSOR_WAVFILE_H
#define SOUNDPROCESSOR_WAVFILE_H

#include <string>
#include <vector>
#include <fstream>
#include "audoiBuffer.h"

class WavFile {
public:
    struct WavHeader {
        char chunkID[4] = {'R', 'I', 'F', 'F'};
        uint32_t chunkSize;
        char format[4] = {'W', 'A', 'V', 'E'};

        char subchunk1ID[4] = {'f', 'm', 't', ' '};
        uint32_t subchunk1Size = 16;
        uint16_t audioFormat = 1;
        uint16_t numChannels = 1;
        uint32_t sampleRate = 44100;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample = 16;

        char subchunk2ID[4] = {'d', 'a', 't', 'a'};
        uint32_t subchunk2Size;
          };

public:
    WavFile() = default;
    ~WavFile();

    WavFile(const WavFile&) = delete;
    WavFile& operator=(const WavFile&) = delete;

    void openForReading(const std::string& filename);
    void openForWriting(const std::string& filename, uint32_t sampleRate = 44100);

    void close();

    bool readNextChunk(std::vector<int16_t>& buffer, size_t chunkSize);
    void resetReading();
    bool isEof() const;

    void writeChunk(const std::vector<int16_t>& buffer);

    const WavHeader& getHeader() const;
    size_t getTotalSamples() const;
    size_t getCurrentSample() const;
    double getDuration() const;
    bool isOpenedForReading() const;
    bool isOpenedForWriting() const;

private:
    void readHeader();
    void writeHeader();
    void prepareHeaderForWriting(uint32_t sampleRate);
    void findDataChunk();
    void checkSupportedFormat() const;

    template<typename T>
    T readBinary();

    template<typename T>
    void writeBinary(T value);

    void readString(char* buffer, size_t length);
    void writeString(const char* buffer, size_t length);

private:
    std::fstream file_;
    WavHeader header_;
    size_t totalSamples_ = 0;
    size_t currentSample_ = 0;
    size_t dataStartPos_ = 0;
    bool isReading_ = false;
    bool isWriting_ = false;
    bool headerRead_ = false;
    std::string filename_;
    friend class MixConverter;
};

#endif //SOUNDPROCESSOR_WAVFILE_H