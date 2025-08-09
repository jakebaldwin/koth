cmake_minimum_required(VERSION 3.20.0)
project(main_trading_engine)

# Source files
set(TRADING_ENGINE_SOURCES
    src/order_book.cpp
    # Add other source files here as they're created
)

set(TRADING_ENGINE_HEADERS
    include/main_trading_engine/order_book.h
    # Add other headers here
)

# Create library for the trading engine core
add_library(trading_engine_lib STATIC
    ${TRADING_ENGINE_SOURCES}
)

# Set target properties
target_include_directories(trading_engine_lib
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link common interface and selective shared libraries
target_link_libraries(trading_engine_lib
    PUBLIC
        trading_common
        Threads::Threads
    PRIVATE
        # Selectively link only the shared libraries this service needs:
        # SharedLibs::market_data    # For order book market data structures
        SharedLibs_memory_pool
        # SharedLibs::logging        # For trading engine logging
        # SharedLibs::config         # For configuration management
        # Add other specific shared libraries as needed
)

# Create main executable (if you have a main.cpp)
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)
    add_executable(main_trading_engine
        src/main.cpp
    )

    target_link_libraries(main_trading_engine
        PRIVATE
            trading_engine_lib
    )

    # Set executable properties for HFT optimization
    if(CMAKE_BUILD_TYPE STREQUAL "Production")
        set_target_properties(main_trading_engine PROPERTIES
            LINK_FLAGS "-Wl,--gc-sections -Wl,--strip-all"
        )
    endif()
endif()

# Testing
if(BUILD_TESTING AND EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests)
    # Test executable
    add_executable(trading_engine_tests
        tests/test_order_book.cpp
        # Add other test files here
    )

    target_link_libraries(trading_engine_tests
        PRIVATE
            trading_engine_lib
            test_utils
    )

    # Register tests with CTest
    add_test(NAME TradingEngineTests COMMAND trading_engine_tests)

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
        add_executable(trading_engine_benchmarks
            benchmarks/benchmark_order_book.cpp
        )

        target_link_libraries(trading_engine_benchmarks
            PRIVATE
                trading_engine_lib
                benchmark::benchmark
        )
    endif()
endif()

# Installation
install(TARGETS trading_engine_lib
    EXPORT trading_system_targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
