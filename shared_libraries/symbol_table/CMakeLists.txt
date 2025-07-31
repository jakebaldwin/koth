# Create the library
add_library(symbol_table 
    STATIC
    src/symbol_table.cpp
)

# Set up include directories
target_include_directories(symbol_table
    PUBLIC 
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

# Alias for consistent naming
add_library(shared_libraries::symbol_table ALIAS symbol_table)

# Tests (only if testing enabled)
if(BUILD_TESTING)
    find_package(GTest REQUIRED)
    
    add_executable(symbol_table_tests
        tests/test_symbol_table.cpp
    )
    
    target_link_libraries(symbol_table_tests
        symbol_table
        GTest::gtest_main
    )
    
    add_test(NAME symbol_table_tests COMMAND symbol_table_tests)
endif()
