#!/bin/bash
# Helper script to create a new service with proper CMakeLists.txt
# Usage: ./scripts/create_service.sh service_name

set -e

SERVICE_NAME=$1
if [ -z "$SERVICE_NAME" ]; then
    echo "Usage: $0 <service_name>"
    echo "Example: $0 risk_engine"
    exit 1
fi

SERVICE_DIR="services/${SERVICE_NAME}"
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

echo "Creating service: $SERVICE_NAME"
echo "Service directory: $SERVICE_DIR"

# Create directory structure
mkdir -p "$SERVICE_DIR"/{include/${SERVICE_NAME},src,tests}

# Create CMakeLists.txt
cat > "$SERVICE_DIR/CMakeLists.txt" << EOF
cmake_minimum_required(VERSION 3.20.0)
project(${SERVICE_NAME})

# Source files
set(${SERVICE_NAME^^}_SOURCES
    src/main.cpp
    # Add other source files here
)

set(${SERVICE_NAME^^}_HEADERS
    include/${SERVICE_NAME}/${SERVICE_NAME}.h
    # Add other headers here
)

# Create library for the ${SERVICE_NAME} core
add_library(${SERVICE_NAME}_lib STATIC
    \${${SERVICE_NAME^^}_SOURCES}
)

# Set target properties
target_include_directories(${SERVICE_NAME}_lib
    PUBLIC
        \$<BUILD_INTERFACE:\${CMAKE_CURRENT_SOURCE_DIR}/include>
        \$<INSTALL_INTERFACE:include>
    PRIVATE
        \${CMAKE_CURRENT_SOURCE_DIR}/src
)

# Link libraries - select only what ${SERVICE_NAME} needs:
target_link_libraries(${SERVICE_NAME}_lib
    PUBLIC
        trading_common
        Threads::Threads
    PRIVATE
        # Select shared libraries this service needs:
        SharedLibs::common_types    # For basic types (Order, Trade, etc.)
        # SharedLibs::memory_pool     # Uncomment if you need fast allocation
        # SharedLibs::symbol_table    # Uncomment if you need symbol lookups
        # SharedLibs::trade_history   # Uncomment if you need trade tracking
        
        # Link to other services if needed:
        # trading_engine_lib          # Uncomment if you need order book access
)

# Create main executable
add_executable(${SERVICE_NAME}
    src/main.cpp
)

target_link_libraries(${SERVICE_NAME}
    PRIVATE
        ${SERVICE_NAME}_lib
)

# Production optimizations
if(CMAKE_BUILD_TYPE STREQUAL "Production")
    set_target_properties(${SERVICE_NAME} PROPERTIES
        LINK_FLAGS "-Wl,--gc-sections -Wl,--strip-all"
    )
endif()

# Testing
if(BUILD_TESTING AND EXISTS \${CMAKE_CURRENT_SOURCE_DIR}/tests)
    file(GLOB TEST_SOURCES tests/*.cpp)
    
    if(TEST_SOURCES)
        add_executable(${SERVICE_NAME}_tests
            \${TEST_SOURCES}
        )
        
        target_link_libraries(${SERVICE_NAME}_tests
            PRIVATE
                ${SERVICE_NAME}_lib
                test_utils
        )
        
        add_test(NAME ${SERVICE_NAME^}Tests COMMAND ${SERVICE_NAME}_tests)
        
        set_tests_properties(${SERVICE_NAME^}Tests PROPERTIES
            TIMEOUT 30
            WILL_FAIL FALSE
        )
    endif()
endif()

# Installation
install(TARGETS ${SERVICE_NAME}_lib
    EXPORT trading_system_targets
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
)

install(DIRECTORY include/
    DESTINATION include
    FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp"
)
EOF

# Create basic header file
cat > "$SERVICE_DIR/include/${SERVICE_NAME}/${SERVICE_NAME}.h" << EOF
#pragma once

#include "common_types/constants.h"
#include "common_types/enums.h"

namespace trading::${SERVICE_NAME} {

class ${SERVICE_NAME^} {
public:
    ${SERVICE_NAME^}();
    ~${SERVICE_NAME^}();
    
    bool initialize();
    void shutdown();
    
private:
    bool initialized_{false};
};

} // namespace trading::${SERVICE_NAME}
EOF

# Create basic source file
cat > "$SERVICE_DIR/src/main.cpp" << EOF
#include "${SERVICE_NAME}/${SERVICE_NAME}.h"
#include <iostream>
#include <memory>

int main() {
    try {
        auto service = std::make_unique<trading::${SERVICE_NAME}::${SERVICE_NAME^}>();
        
        if (!service->initialize()) {
            std::cerr << "Failed to initialize ${SERVICE_NAME}" << std::endl;
            return 1;
        }
        
        std::cout << "${SERVICE_NAME^} started successfully" << std::endl;
        
        // Main service loop would go here
        
        service->shutdown();
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "${SERVICE_NAME^} error: " << e.what() << std::endl;
        return 1;
    }
}
EOF

# Create basic test file
cat > "$SERVICE_DIR/tests/test_${SERVICE_NAME}.cpp" << EOF
#include <gtest/gtest.h>
#include "${SERVICE_NAME}/${SERVICE_NAME}.h"

using namespace trading::${SERVICE_NAME};

class ${SERVICE_NAME^}Test : public ::testing::Test {
protected:
    void SetUp() override {
        service_ = std::make_unique<${SERVICE_NAME^}>();
    }
    
    void TearDown() override {
        if (service_) {
            service_->shutdown();
        }
    }
    
    std::unique_ptr<${SERVICE_NAME^}> service_;
};

TEST_F(${SERVICE_NAME^}Test, InitializeSuccess) {
    EXPECT_TRUE(service_->initialize());
}

TEST_F(${SERVICE_NAME^}Test, ShutdownSuccess) {
    service_->initialize();
    EXPECT_NO_THROW(service_->shutdown());
}
EOF

# Add to services CMakeLists.txt
SERVICES_CMAKE="services/CMakeLists.txt"
if ! grep -q "add_subdirectory(${SERVICE_NAME})" "$SERVICES_CMAKE"; then
    # Find the line with "add_subdirectory(main_trading_engine)" and add after it
    sed -i "/add_subdirectory(main_trading_engine)/a add_subdirectory(${SERVICE_NAME})" "$SERVICES_CMAKE"
    echo "Added ${SERVICE_NAME} to services/CMakeLists.txt"
fi

echo ""
echo "✅ Service '${SERVICE_NAME}' created successfully!"
echo ""
echo "Created files:"
echo "  📁 $SERVICE_DIR/"
echo "  📄 $SERVICE_DIR/CMakeLists.txt"
echo "  📄 $SERVICE_DIR/include/${SERVICE_NAME}/${SERVICE_NAME}.h"
echo "  📄 $SERVICE_DIR/src/main.cpp"
echo "  📄 $SERVICE_DIR/tests/test_${SERVICE_NAME}.cpp"
echo ""
echo "Next steps:"
echo "1. Customize the CMakeLists.txt to link only needed shared libraries"
echo "2. Implement your service logic in the header and source files"
echo "3. Build with: ./build.sh"
echo ""
echo "The service has been added to services/CMakeLists.txt automatically."
EOF

chmod +x "scripts/create_service.sh"
