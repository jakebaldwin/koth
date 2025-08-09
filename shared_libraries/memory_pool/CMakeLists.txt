# Create the library
add_library(memory_pool
    STATIC
    src/memory_pool.cpp
)

# Set up include directories
target_include_directories(memory_pool
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common_types/include>
)

# Alias for consistent naming
add_library(shared_libraries::memory_pool ALIAS memory_pool)

# Tests (only if testing enabled)
if(BUILD_TESTING)
    find_package(GTest REQUIRED)

    add_executable(memory_pool_tests
        tests/test_memory_pool.cpp
    )

    target_link_libraries(memory_pool_tests
        memory_pool
        GTest::gtest_main
    )

    add_test(NAME memory_pool_tests COMMAND memory_pool_tests)
endif()
