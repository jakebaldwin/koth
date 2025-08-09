# Create the library
add_library(trade_history
    STATIC
    src/trade_history.cpp
)

# Set up include directories
target_include_directories(trade_history
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../common_types/include>
)

# Alias for consistent naming
add_library(shared_libraries::trade_history ALIAS trade_history)

# Tests (only if testing enabled)
if(BUILD_TESTING)
    find_package(GTest REQUIRED)

    add_executable(trade_history_tests
        tests/test_trade_history.cpp
    )

    target_link_libraries(trade_history_tests
        trade_history
        GTest::gtest_main
    )

    add_test(NAME trade_history_tests COMMAND trade_history_tests)
endif()
