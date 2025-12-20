cmake_minimum_required(VERSION 4.0)
project(SoundProcessor)

set(CMAKE_CXX_STANDARD 20)

add_executable(SoundProcessor main.cpp
        wavFile.cpp
        wavFile.h
        exceptions.h
        audoiBuffer.cpp
        audoiBuffer.h
        streamAudioBuffer.cpp
        streamAudioBuffer.h
        converter.h
        muteConverter.cpp
        muteConverter.h
        mixConverter.cpp
        mixConverter.h
        converterFactory.cpp
        converterFactory.h
        configParser.cpp
        configParser.h
        volumeConverter.cpp
        volumeConverter.h
        soundProcessor.cpp
        soundProcessor.h
)
