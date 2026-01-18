cmake_minimum_required(VERSION 3.15)
project(pacman)

set(CMAKE_CXX_STANDARD 17)

set(SFML_PATH "C:/Users/Эвелина/CLionProjects/pacman/SFML-2.6.2")

if(EXISTS "${SFML_PATH}/lib/libsfml-graphics.a")
    set(SFML_IS_MINGW TRUE)
    message(STATUS "Found MinGW version of SFML")
else()
    message(FATAL_ERROR "SFML MinGW libraries not found!")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(SFML_GRAPHICS_LIB "${SFML_PATH}/lib/libsfml-graphics-d.a")
    set(SFML_WINDOW_LIB "${SFML_PATH}/lib/libsfml-window-d.a")
    set(SFML_SYSTEM_LIB "${SFML_PATH}/lib/libsfml-system-d.a")
    set(SFML_GRAPHICS_DLL "${SFML_PATH}/bin/sfml-graphics-d-2.dll")
    set(SFML_WINDOW_DLL "${SFML_PATH}/bin/sfml-window-d-2.dll")
    set(SFML_SYSTEM_DLL "${SFML_PATH}/bin/sfml-system-d-2.dll")
else()
    set(SFML_GRAPHICS_LIB "${SFML_PATH}/lib/libsfml-graphics.a")
    set(SFML_WINDOW_LIB "${SFML_PATH}/lib/libsfml-window.a")
    set(SFML_SYSTEM_LIB "${SFML_PATH}/lib/libsfml-system.a")
    set(SFML_GRAPHICS_DLL "${SFML_PATH}/bin/sfml-graphics-2.dll")
    set(SFML_WINDOW_DLL "${SFML_PATH}/bin/sfml-window-2.dll")
    set(SFML_SYSTEM_DLL "${SFML_PATH}/bin/sfml-system-2.dll")
endif()

add_executable(pacman main.cpp GameMap.cpp
        Pacman.cpp
        Pacman.h
        Ghost.cpp
        Ghost.h
        IGhost.cpp
        IGhost.h
        Game.cpp
        Game.h)

target_include_directories(pacman PRIVATE "${SFML_PATH}/include")

find_library(FREETYPE_LIB
        NAMES freetype libfreetype freetype6 libfreetype-6
        PATHS "${SFML_PATH}/lib"
        NO_DEFAULT_PATH
)

if(FREETYPE_LIB)
    message(STATUS "Found freetype: ${FREETYPE_LIB}")
else()
    find_library(FREETYPE_LIB freetype)
    if(NOT FREETYPE_LIB)
        message(WARNING "Freetype library not found, trying without it...")
        set(FREETYPE_LIB "")
    endif()
endif()

target_link_libraries(pacman
        ${SFML_GRAPHICS_LIB}
        ${SFML_WINDOW_LIB}
        ${SFML_SYSTEM_LIB}
        opengl32
        ${FREETYPE_LIB}  
        winmm
        gdi32
)

add_custom_command(TARGET pacman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_GRAPHICS_DLL}"
        "$<TARGET_FILE_DIR:pacman>"
)

add_custom_command(TARGET pacman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_WINDOW_DLL}"
        "$<TARGET_FILE_DIR:pacman>"
)

add_custom_command(TARGET pacman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_SYSTEM_DLL}"
        "$<TARGET_FILE_DIR:pacman>"
)

add_custom_command(TARGET pacman POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${SFML_PATH}/bin/openal32.dll"
        "$<TARGET_FILE_DIR:pacman>"
)
