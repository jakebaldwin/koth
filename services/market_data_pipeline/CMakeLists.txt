cmake_minimum_required(VERSION 3.20.0)
project(market_data_pipeline)

# Source files
set(MARKET_DATA_PIPELINE_SOURCES
    src/market_data_order_book.cpp
    # Add other source files here as they're created
)

set(MARKET_DATA_PIPELINE_HEADERS
    include/market_data_pipeline/market_data_order_book.h
    # Add other headers here
)

# Create library for the trading engine core
add_library(market_data_pipeline_lib STATIC
    ${MARKET_DATA_PIPELINE_SOURCES}
)

# Set target properties
target_include_directories(market_data_pipeline_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link common interface and selective shared libraries
target_link_libraries(market_data_pipeline_lib
    PUBLIC
        trading_common
        Threads::Threads
    PRIVATE
        # Selectively link only the shared libraries this service needs:
        SharedLibs_common_types
        SharedLibs_common_utilities
        SharedLibs_symbol_table
        # SharedLibs::logging        # For trading engine logging
        # SharedLibs::config         # For configuration management
        # Add other specific shared libraries as needed
)

# Create main executable (if you have a main.cpp)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_executable(market_data_pipeline
        src/main.cpp
    )

    target_link_libraries(market_data_pipeline
        PRIVATE
            market_data_pipeline_lib
    )

    # Set executable properties for HFT optimization
    if(CMAKE_BUILD_TYPE STREQUAL "Production")
        set_target_properties(market_data_pipeline PROPERTIES
            LINK_FLAGS "-Wl,--gc-sections -Wl,--strip-all"
        )
    endif()
endif()

# Testing
if(BUILD_TESTING AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    # Test executable
    add_executable(market_data_pipeline_tests
        tests/test_market_data_order_book.cpp
        # Add other test files here
    )

    target_link_libraries(market_data_pipeline_tests
        PRIVATE
            market_data_pipeline_lib
            SharedLibs_symbol_table
            test_utils
    )

    # Register tests with CTest
    add_test(NAME TradingEngineTests COMMAND market_data_pipeline_tests)

    # Set test properties for better output
    set_tests_properties(TradingEngineTests PROPERTIES
        TIMEOUT 30
        WILL_FAIL FALSE
    )
endif()

# Benchmarks (optional, for HFT performance testing)
if(BUILD_BENCHMARKS)
    find_package(benchmark QUIET)
    if(benchmark_FOUND AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/benchmarks)
        add_executable(market_data_pipeline_benchmarks
            benchmarks/benchmark_order_book.cpp
        )

        target_link_libraries(market_data_pipeline_benchmarks
            PRIVATE
                market_data_pipeline_lib
                benchmark::benchmark
        )
    endif()
endif()

# Installation
install(TARGETS market_data_pipeline_lib
    EXPORT trading_system_targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
