CMAKE_MINIMUM_REQUIRED(VERSION 3.16)
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
    source/FlatApproach.h
    source/InheritanceApproach.h
    source/main.cpp
)
TARGET_LINK_LIBRARIES(
    NGridTests
    gtest_main
)

INCLUDE(GoogleTest)
GTEST_DISCOVER_TESTS(NGridTests)
