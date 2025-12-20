//
// Created by Эвелина on 15.12.2025.
//

#ifndef SOUNDPROCESSOR_EXCEPTIONS_H
#define SOUNDPROCESSOR_EXCEPTIONS_H

#include <stdexcept>
#include <string>

class WavException : public std::runtime_error {
public:
    explicit WavException(const std::string& message)
        : std::runtime_error(message) {}
};

class WavFileError : public WavException {
public:
    explicit WavFileError(const std::string& filename, const std::string& operation)
        : WavException("File error: " + operation + " for file '" + filename + "'") {}

    explicit WavFileError(const std::string& message)
        : WavException("File error: " + message) {}
};

class WavFormatError : public WavException {
public:
    explicit WavFormatError(const std::string& message)
        : WavException("Format error: " + message) {}

    static WavFormatError NotRiffFile() {
        return WavFormatError("Not a RIFF file");
    }

    static WavFormatError NotWaveFile() {
        return WavFormatError("Not a WAVE file");
    }

    static WavFormatError MissingFmtChunk() {
        return WavFormatError("Missing 'fmt ' chunk");
    }

    static WavFormatError DataChunkNotFound() {
        return WavFormatError("Data chunk not found");
    }

    static WavFormatError UnsupportedAudioFormat() {
        return WavFormatError("Only PCM format is supported (audioFormat = 1)");
    }

    static WavFormatError UnsupportedChannels() {
        return WavFormatError("Only mono audio is supported (numChannels = 1)");
    }

    static WavFormatError UnsupportedBitsPerSample() {
        return WavFormatError("Only 16-bit audio is supported");
    }

    static WavFormatError UnsupportedSampleRate() {
        return WavFormatError("Only 44100 Hz sample rate is supported");
    }
};

class WavStateError : public WavException {
public:
    explicit WavStateError(const std::string& message)
        : WavException("State error: " + message) {}

    static WavStateError NotOpenedForReading() {
        return WavStateError("File not opened for reading");
    }

    static WavStateError NotOpenedForWriting() {
        return WavStateError("File not opened for writing");
    }

    static WavStateError AlreadyOpened() {
        return WavStateError("File already opened");
    }

    static WavStateError HeaderNotRead() {
        return WavStateError("Header not read");
    }
};
#endif //SOUNDPROCESSOR_EXCEPTIONS_H