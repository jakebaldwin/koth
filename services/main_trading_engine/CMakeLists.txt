cmake_minimum_required(VERSION, 3.16)
project(main_trading_engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include Directories
include_directories(include)

# Source Files
set(SOURCES
    src/order_book.cpp
)

# Create the main executable 
# add_executable(main_trading_engine ${SOURCES} src/main.cpp)

# Google Test setup
find_package(GTest REQUIRED)
enable_testing()

# Test executable
add_executable(order_book_tests
    tests/test_order_book.cpp
)

target_link_libraries(order_book_tests
    trading_engine_lib
    GTest::gtest_main
)

# Register tests with CTest
add_test(NAME OrderBookTests COMMAND order_book_tests)
