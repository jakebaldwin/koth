cmake_minimum_required(VERSION 3.16)
project(shared_libraries)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Production" CACHE STRING "" FORCE)

# Testing option
option(BUILD_TESTING "Build tests" ON)

# Disable testing for production builds
if(CMAKE_BUILD_TYPE STREQUAL "Production")
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
endif()

# Compiler flags
if(CMAKE_BUILD_TYPE STREQUAL "Production")
    set(CMAKE_CXX_FLAGS_PRODUCTION "-O3 -DNDEBUG -march=native -flto")
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g -Wall -Wextra")
endif()

# Add subdirectories
add_subdirectory(symbol_table)
add_subdirectory(trade_history)
#add_subdirectory(memory_pool)
#add_subdirectory(lock_free_buffers)
