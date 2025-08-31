cmake_minimum_required(VERSION 3.20.0)
project(trading_services VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Build types
set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Production" CACHE STRING "" FORCE)

# Set default build type if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose build type" FORCE)
endif()

# Testing option
option(BUILD_TESTING "Build tests" ON)

# Disable testing for production builds
if(CMAKE_BUILD_TYPE STREQUAL "Production")
    set(BUILD_TESTING OFF CACHE BOOL "" FORCE)
endif()

# Compiler-specific optimizations for HFT
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS_PRODUCTION "-O3 -DNDEBUG -march=native -mtune=native -flto -fno-rtti -fno-exceptions -DPRODUCTION_BUILD")
    set(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG -march=native -g1")
    set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g3 -Wall -Wextra -Wpedantic -Wshadow -Wconversion -fsanitize=address,undefined")

    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        add_compile_options(-Werror -Wno-unused-parameter)
    endif()
endif()

# Find required packages
find_package(Threads REQUIRED)

# Set shared libraries paths directly (simpler approach)
set(SHARED_LIBRARIES_ROOT "${CMAKE_CURRENT_SOURCE_DIR}/../shared_libraries")
string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE_LOWER)
set(SHARED_LIBRARIES_BUILD_DIR "${SHARED_LIBRARIES_ROOT}/build-${BUILD_TYPE_LOWER}")

# Check if shared libraries exist
if(EXISTS "${SHARED_LIBRARIES_ROOT}" AND EXISTS "${SHARED_LIBRARIES_BUILD_DIR}")
    set(SHARED_LIBRARIES_FOUND TRUE)
    message(STATUS "Found shared libraries at: ${SHARED_LIBRARIES_ROOT}")
    message(STATUS "Found build directory: ${SHARED_LIBRARIES_BUILD_DIR}")
else()
    set(SHARED_LIBRARIES_FOUND FALSE)
    message(WARNING "Shared libraries not found. Build them first: cd ../shared_libraries && ./build.sh ${CMAKE_BUILD_TYPE}")
endif()

# Create common interface library (no forced dependencies)
add_library(trading_common INTERFACE)
target_compile_features(trading_common INTERFACE cxx_std_23)

# Add common compile definitions
target_compile_definitions(trading_common INTERFACE
    $<$<CONFIG:Debug>:DEBUG_BUILD>
    $<$<CONFIG:Release>:RELEASE_BUILD>
    $<$<CONFIG:Production>:PRODUCTION_BUILD>
)

# Add shared libraries include directories if available
if(SHARED_LIBRARIES_FOUND)
    target_include_directories(trading_common INTERFACE
        ${SHARED_LIBRARIES_ROOT}/common_types/include
    )
endif()

# Create imported targets for your specific shared libraries
if(SHARED_LIBRARIES_FOUND)
    # Based on your actual shared_libraries structure:

    add_library(SharedLibs_common_types INTERFACE IMPORTED)
    set_target_properties(SharedLibs_common_types PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SHARED_LIBRARIES_ROOT}/common_types/include"
    )

    add_library(SharedLibs_common_utilities INTERFACE IMPORTED)
    set_target_properties(SharedLibs_common_utilities PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${SHARED_LIBRARIES_ROOT}/common_utilities/include"
    )

    add_library(SharedLibs_memory_pool STATIC IMPORTED)
    set_target_properties(SharedLibs_memory_pool PROPERTIES
        IMPORTED_LOCATION "${SHARED_LIBRARIES_BUILD_DIR}/memory_pool/libmemory_pool.a"
        INTERFACE_INCLUDE_DIRECTORIES "${SHARED_LIBRARIES_ROOT}/memory_pool/include"
    )

    add_library(SharedLibs_symbol_table STATIC IMPORTED)
    set_target_properties(SharedLibs_symbol_table PROPERTIES
        IMPORTED_LOCATION "${SHARED_LIBRARIES_BUILD_DIR}/symbol_table/libsymbol_table.a"
        INTERFACE_INCLUDE_DIRECTORIES "${SHARED_LIBRARIES_ROOT}/symbol_table/include"
    )

    message(STATUS "Configured shared libraries for selective linking")
else()
    # Create fallback interface libraries with direct paths
    message(WARNING "Shared libraries not built - creating fallback targets")
    message(STATUS "Build shared libraries first: cd ../shared_libraries && ./build.sh ${CMAKE_BUILD_TYPE}")

    add_library(SharedLibs_common_types INTERFACE)
    target_include_directories(SharedLibs_common_types INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/../shared_libraries/common_types/include"
    )

    add_library(SharedLibs_memory_pool INTERFACE)
    target_include_directories(SharedLibs_memory_pool INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/../shared_libraries/memory_pool/include"
    )

    add_library(SharedLibs_symbol_table INTERFACE)
    target_include_directories(SharedLibs_symbol_table INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/../shared_libraries/symbol_table/include"
    )

endif()

# Testing setup
if(BUILD_TESTING)
    enable_testing()
    find_package(GTest REQUIRED)

    add_library(test_utils INTERFACE)
    target_link_libraries(test_utils INTERFACE
        GTest::gtest
        GTest::gtest_main
        trading_common
    )
endif()

# Add service subdirectories
add_subdirectory(market_data_pipeline)
# Uncomment as you create these services:
# add_subdirectory(trading_engine_core)
# add_subdirectory(risk_engine)
# add_subdirectory(portfolio_management)
# add_subdirectory(fix_gateway)
# add_subdirectory(inventory_manager)
# add_subdirectory(performance_monitor)
# add_subdirectory(kill_switch)
# add_subdirectory(system_control)
