# Create the library
add_library(common_utilities
    STATIC
    src/time_utils.cpp
)

# Set up include directories
target_include_directories(common_utilities
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

add_library(shared_libraries::common_utilities ALIAS common_utilities)

target_link_libraries(common_utilities
    PUBLIC
        common_types
)

# Tests (only if testing enabled)
if(BUILD_TESTING)
    find_package(GTest REQUIRED)

    add_executable(common_utilities_tests
        tests/test_common_utilities.cpp
    )

    target_link_libraries(common_utilities_tests
        common_utilities
        GTest::gtest_main
    )

    add_test(NAME common_utilities_tests COMMAND common_utilities_tests)
endif()
