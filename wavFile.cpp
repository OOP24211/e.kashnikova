//
// Created by Эвелина on 15.12.2025.
//

#include "wavFile.h"
#include <cstring>
#include <iostream>

template<typename T>
T WavFile::readBinary() {
    T value;
    file_.read(reinterpret_cast<char*>(&value), sizeof(T));
    if (!file_) {
        throw WavFileError(filename_, "failed to read binary data");
    }
    return value;
}

template<typename T>
void WavFile::writeBinary(T value) {
    file_.write(reinterpret_cast<const char*>(&value), sizeof(T));
    if (!file_) {
        throw WavFileError(filename_, "failed to write binary data");
    }
}

void WavFile::readString(char* buffer, size_t length) {
    file_.read(buffer, length);
    if (!file_) {
        throw WavFileError(filename_, "failed to read string");
    }
}

void WavFile::writeString(const char* buffer, size_t length) {
    file_.write(buffer, length);
    if (!file_) {
        throw WavFileError(filename_, "failed to write string");
    }
}

WavFile::~WavFile() {
    try {
        close();
    } catch (...) {
    }
}

void WavFile::openForReading(const std::string& filename) {
    if (isReading_ || isWriting_) {
        throw WavStateError::AlreadyOpened();
    }

    filename_ = filename;
    file_.open(filename, std::ios::binary | std::ios::in);
    if (!file_.is_open()) {
        throw WavFileError(filename, "cannot open for reading");
    }

    try {
        readHeader();
        checkSupportedFormat();
        findDataChunk();

        totalSamples_ = header_.subchunk2Size / (header_.bitsPerSample / 8) / header_.numChannels;
        currentSample_ = 0;
        isReading_ = true;

    } catch (...) {
        file_.close();
        isReading_ = false;
        isWriting_ = false;
        headerRead_ = false;
        throw;
    }
}

void WavFile::openForWriting(const std::string& filename, uint32_t sampleRate) {
    if (isReading_ || isWriting_) {
        throw WavStateError::AlreadyOpened();
    }

    filename_ = filename;
    file_.open(filename, std::ios::binary | std::ios::out | std::ios::trunc);
    if (!file_.is_open()) {
        throw WavFileError(filename, "cannot create for writing");
    }

    try {
        prepareHeaderForWriting(sampleRate);
        writeHeader();

        totalSamples_ = 0;
        currentSample_ = 0;
        isWriting_ = true;
        headerRead_ = true;

    } catch (...) {
        file_.close();
        isReading_ = false;
        isWriting_ = false;
        headerRead_ = false;
        throw;
    }
}

void WavFile::close() {
    if (isWriting_ && file_.is_open()) {
        try {
            header_.chunkSize = 36 + header_.subchunk2Size;

            auto currentPos = file_.tellp();

            file_.seekp(0, std::ios::beg);
            writeHeader();

            file_.seekp(currentPos);
        } catch (...) {
            }
    }

    if (file_.is_open()) {
        file_.close();
    }

    isReading_ = false;
    isWriting_ = false;
    headerRead_ = false;
    totalSamples_ = 0;
    currentSample_ = 0;
    dataStartPos_ = 0;
    filename_.clear();
}

bool WavFile::readNextChunk(std::vector<int16_t>& buffer, size_t chunkSize) {
    if (!isReading_) {
        throw WavStateError::NotOpenedForReading();
    }

    if (!file_.is_open()) {
        throw WavFileError(filename_, "file is not open");
    }

    if (isEof()) {
        return false;
    }

    size_t samplesToRead = std::min(chunkSize, totalSamples_ - currentSample_);

    if (samplesToRead == 0) {
        return false;
    }

    buffer.resize(samplesToRead);
    file_.read(reinterpret_cast<char*>(buffer.data()),
               samplesToRead * sizeof(int16_t));

    if (!file_) {
        throw WavFileError(filename_, "failed to read audio data");
    }

    currentSample_ += samplesToRead;
    return true;
}

void WavFile::resetReading() {
    if (!isReading_) {
        throw WavStateError::NotOpenedForReading();
    }

    if (!file_.is_open()) {
        throw WavFileError(filename_, "file is not open");
    }

    file_.clear();
    file_.seekg(dataStartPos_, std::ios::beg);
    currentSample_ = 0;
}

bool WavFile::isEof() const {
    return !isReading_ || currentSample_ >= totalSamples_ || !file_.is_open();
}

void WavFile::writeChunk(const std::vector<int16_t>& buffer) {
    if (!isWriting_) {
        throw WavStateError::NotOpenedForWriting();
    }

    if (!file_.is_open()) {
        throw WavFileError(filename_, "file is not open");
    }

    if (buffer.empty()) {
        return;
    }

    file_.write(reinterpret_cast<const char*>(buffer.data()),
                buffer.size() * sizeof(int16_t));

    if (!file_) {
        throw WavFileError(filename_, "failed to write audio data");
    }

    totalSamples_ += buffer.size();
    header_.subchunk2Size = totalSamples_ * sizeof(int16_t);
}

const WavFile::WavHeader& WavFile::getHeader() const {
    if (!headerRead_) {
        throw WavStateError::HeaderNotRead();
    }
    return header_;
}

size_t WavFile::getTotalSamples() const {
    return totalSamples_;
}

size_t WavFile::getCurrentSample() const {
    return currentSample_;
}

double WavFile::getDuration() const {
    if (header_.sampleRate == 0) {
        return 0.0;
    }
    return static_cast<double>(totalSamples_) / header_.sampleRate;
}

bool WavFile::isOpenedForReading() const {
    return isReading_;
}

bool WavFile::isOpenedForWriting() const {
    return isWriting_;
}

void WavFile::readHeader() {
    if (headerRead_) {
        return;
    }

    try {
        readString(header_.chunkID, 4);
        if (std::string(header_.chunkID, 4) != "RIFF") {
            throw WavFormatError::NotRiffFile();
        }

        header_.chunkSize = readBinary<uint32_t>();
        readString(header_.format, 4);
        if (std::string(header_.format, 4) != "WAVE") {
            throw WavFormatError::NotWaveFile();
        }

       readString(header_.subchunk1ID, 4);
        if (std::string(header_.subchunk1ID, 4) != "fmt ") {
            throw WavFormatError::MissingFmtChunk();
        }

        header_.subchunk1Size = readBinary<uint32_t>();
        header_.audioFormat = readBinary<uint16_t>();
        header_.numChannels = readBinary<uint16_t>();
        header_.sampleRate = readBinary<uint32_t>();
        header_.byteRate = readBinary<uint32_t>();
        header_.blockAlign = readBinary<uint16_t>();
        header_.bitsPerSample = readBinary<uint16_t>();

        headerRead_ = true;

    } catch (const WavException&) {
        throw;
    } catch (const std::exception& e) {
        throw WavFileError(std::string("failed to read header: ") + e.what());
    }
}

void WavFile::writeHeader() {
    if (!file_.is_open()) {
        throw WavFileError(filename_, "file is not open");
    }

    try {
        writeString(header_.chunkID, 4);
        writeBinary(header_.chunkSize);
        writeString(header_.format, 4);

        writeString(header_.subchunk1ID, 4);
        writeBinary(header_.subchunk1Size);
        writeBinary(header_.audioFormat);
        writeBinary(header_.numChannels);
        writeBinary(header_.sampleRate);
        writeBinary(header_.byteRate);
        writeBinary(header_.blockAlign);
        writeBinary(header_.bitsPerSample);

        writeString(header_.subchunk2ID, 4);
        writeBinary(header_.subchunk2Size);

    } catch (const WavException&) {
        throw;
    } catch (const std::exception& e) {
        throw WavFileError(std::string("failed to write header: ") + e.what());
    }
}

void WavFile::prepareHeaderForWriting(uint32_t sampleRate) {
    std::strcpy(header_.chunkID, "RIFF");
    std::strcpy(header_.format, "WAVE");
    std::strcpy(header_.subchunk1ID, "fmt ");
    std::strcpy(header_.subchunk2ID, "data");

    header_.subchunk1Size = 16;
    header_.audioFormat = 1;
    header_.chunkSize = 36;
    header_.subchunk2Size = 0;
    header_.numChannels = 1;
    header_.bitsPerSample = 16;
    header_.sampleRate = sampleRate;
    header_.blockAlign = header_.numChannels * header_.bitsPerSample / 8;
    header_.byteRate = header_.sampleRate * header_.blockAlign;
}

void WavFile::findDataChunk() {
    if (!headerRead_) {
        throw WavStateError::HeaderNotRead();
    }

    try {
        if (header_.subchunk1Size > 16) {
            file_.seekg(header_.subchunk1Size - 16, std::ios::cur);
        }

        char chunkID[4];
        uint32_t chunkSize;

        while (!file_.eof()) {
            readString(chunkID, 4);
            chunkSize = readBinary<uint32_t>();

            if (std::string(chunkID, 4) == "data") {
                header_.subchunk2Size = chunkSize;
                dataStartPos_ = file_.tellg();
                return;
            }

            file_.seekg(chunkSize, std::ios::cur);
        }

        throw WavFormatError::DataChunkNotFound();

    } catch (const WavException&) {
        throw;
    } catch (const std::exception& e) {
        throw WavFileError(std::string("failed to find data chunk: ") + e.what());
    }
}

void WavFile::checkSupportedFormat() const {
    if (!headerRead_) {
        throw WavStateError::HeaderNotRead();
    }

    if (header_.audioFormat != 1) {
        throw WavFormatError::UnsupportedAudioFormat();
    }

    if (header_.numChannels != 1) {
        throw WavFormatError::UnsupportedChannels();
    }

    if (header_.bitsPerSample != 16) {
        throw WavFormatError::UnsupportedBitsPerSample();
    }

    if (header_.sampleRate != 44100) {
        throw WavFormatError::UnsupportedSampleRate();
    }
}
