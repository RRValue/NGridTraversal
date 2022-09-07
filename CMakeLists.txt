CMAKE_MINIMUM_REQUIRED(VERSION 3.16)

SET(PROJECT_BIN_PATH ${CMAKE_CURRENT_SOURCE_DIR}/bin)

PROJECT(NGridTraversal)

# Set C++ Standard
SET(CMAKE_CXX_STANDARD 20)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

INCLUDE(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/release-1.11.0.zip
)

SET(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

ENABLE_TESTING()

ADD_EXECUTABLE(
    NGridTests
    source/Grid.h
    source/GridIterator.h
    tests/NGridTests.cpp
)
TARGET_LINK_LIBRARIES(
    NGridTests
    gtest_main
)

TARGET_INCLUDE_DIRECTORIES(NGridTests PUBLIC source)

INCLUDE(GoogleTest)

GTEST_DISCOVER_TESTS(NGridTests)

IF(MSVC)
    SET_TARGET_PROPERTIES(NGridTests PROPERTIES PDB_OUTPUT_DIRECTORY ${PROJECT_BIN_PATH})
    SET_TARGET_PROPERTIES(NGridTests PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BIN_PATH})
ENDIF(MSVC)