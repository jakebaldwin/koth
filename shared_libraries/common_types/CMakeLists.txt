cmake_minimum_required(VERSION 3.20.0)
project(common_types)

# Create an INTERFACE library (header-only)
add_library(common_types INTERFACE)

# Set the include directories for the library
target_include_directories(common_types
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

# Specify C++ standard requirements
target_compile_features(common_types INTERFACE cxx_std_23)

# Optional: Add compile definitions if needed
# target_compile_definitions(common_types INTERFACE
#     COMMON_TYPES_VERSION_MAJOR=1
#     COMMON_TYPES_VERSION_MINOR=0
# )

# Installation rules
install(TARGETS common_types
    EXPORT common_types_targets
    INCLUDES DESTINATION include
)

# Install headers
install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)

# Install export configuration
install(EXPORT common_types_targets
    FILE common_types_targets.cmake
    NAMESPACE CommonTypes::
    DESTINATION lib/cmake/common_types
)

# Testing (optional, for header validation)
if(BUILD_TESTING)
    # Create a simple test to ensure headers compile
    add_executable(common_types_compile_test
        ${CMAKE_CURRENT_SOURCE_DIR}/tests/compile_test.cpp
    )

    target_link_libraries(common_types_compile_test
        PRIVATE common_types
    )
 
    add_test(NAME CommonTypesCompileTest COMMAND common_types_compile_test)
endif()
