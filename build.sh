#!/bin/bash
# Clean build script for trading system
set -e

# Parse arguments
BUILD_TYPE=${1:-Debug}
COMMAND=${2:-""}
BUILD_TYPE_LOWER=$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')
PROJECT_ROOT=$(pwd)

# Colors
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m'

# Get CPU cores
if command -v nproc >/dev/null 2>&1; then
    CORES=$(nproc)
elif command -v sysctl >/dev/null 2>&1; then
    CORES=$(sysctl -n hw.ncpu)
else
    CORES=4
fi

# Help function
show_help() {
    echo "Usage: ./build.sh [BUILD_TYPE] [COMMAND]"
    echo ""
    echo "BUILD_TYPE:"
    echo "  Debug      - Debug build with sanitizers (default)"
    echo "  Release    - Optimized build with debug info"
    echo "  Production - Maximum optimization for HFT"
    echo ""
    echo "COMMAND:"
    echo "  clean      - Clean build directories"
    echo "  shared     - Build only shared libraries"
    echo "  services   - Build only services"
    echo "  help       - Show this help"
    echo ""
    echo "Examples:"
    echo "  ./build.sh                    # Debug build of everything"
    echo "  ./build.sh Release           # Release build of everything"
    echo "  ./build.sh Debug clean       # Clean debug build"
    echo "  ./build.sh Production shared # Build shared libs in production mode"
}

# Handle help
if [ "$BUILD_TYPE" = "help" ] || [ "$COMMAND" = "help" ]; then
    show_help
    exit 0
fi

# Validate build type
if [[ ! "$BUILD_TYPE" =~ ^(Debug|Release|Production)$ ]]; then
    echo -e "${RED}Error: Invalid build type '$BUILD_TYPE'${NC}"
    echo "Valid types: Debug, Release, Production"
    exit 1
fi

echo -e "${BLUE}=== Trading System Build (${BUILD_TYPE}) ===${NC}"
echo -e "${BLUE}Using ${CORES} cores${NC}"

# Build function
build_component() {
    local name=$1
    local path=$2
    local enable_tests=${3:-"ON"}
    
    echo -e "${YELLOW}=== Building ${name} ===${NC}"
    
    if [ ! -d "$path" ]; then
        echo -e "${RED}Error: Directory not found: $path${NC}"
        return 1
    fi
    
    cd "$path"
    
    # Clean if requested
    if [ "$COMMAND" = "clean" ]; then
        echo -e "${YELLOW}Cleaning build directory${NC}"
        rm -rf "build-${BUILD_TYPE_LOWER}"
    fi
    
    mkdir -p "build-${BUILD_TYPE_LOWER}"
    cd "build-${BUILD_TYPE_LOWER}"
    
    # Configure
    echo -e "${BLUE}Configuring ${name}${NC}"
    cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
          -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
          -DBUILD_TESTING=$enable_tests \
          ..
    
    # Build
    echo -e "${BLUE}Building ${name}${NC}"
    make -j${CORES}
    
    # Test if enabled
    if [ "$enable_tests" = "ON" ] && [ -f "CTestTestfile.cmake" ]; then
        echo -e "${BLUE}Running ${name} tests${NC}"
        ctest --output-on-failure
    fi
    
    cd "$PROJECT_ROOT"
}

# Build shared libraries
if [ "$COMMAND" != "services" ]; then
    if [ -d "shared_libraries" ]; then
        build_component "Shared Libraries" "shared_libraries" "ON"
    else
        echo -e "${RED}Error: shared_libraries directory not found${NC}"
        exit 1
    fi
fi

# Build services
if [ "$COMMAND" != "shared" ]; then
    if [ -d "services" ]; then
        build_component "Services" "services" "ON"
        
        # Setup clangd support
        SERVICES_COMPILE_COMMANDS="services/build-${BUILD_TYPE_LOWER}/compile_commands.json"
        SHARED_COMPILE_COMMANDS="shared_libraries/build-${BUILD_TYPE_LOWER}/compile_commands.json"
        
        if [ -f "$SERVICES_COMPILE_COMMANDS" ]; then
            echo -e "${GREEN}=== Setting up clangd support ===${NC}"
            
            if [ -f "$SHARED_COMPILE_COMMANDS" ] && command -v jq >/dev/null 2>&1; then
                # Merge compile commands for better clangd support
                jq -s 'add' "$SHARED_COMPILE_COMMANDS" "$SERVICES_COMPILE_COMMANDS" > "$PROJECT_ROOT/compile_commands.json"
            else
                # Just copy services compile commands
                cp "$SERVICES_COMPILE_COMMANDS" "$PROJECT_ROOT/compile_commands.json"
            fi
            
            echo -e "${GREEN}compile_commands.json updated${NC}"
        fi
    else
        echo -e "${RED}Error: services directory not found${NC}"
        exit 1
    fi
fi

echo -e "${GREEN}=== Build Complete ===${NC}"

# Show results
if [ "$COMMAND" != "shared" ] && [ -d "services/build-${BUILD_TYPE_LOWER}" ]; then
    echo -e "${BLUE}=== Available Executables ===${NC}"
    find "services/build-${BUILD_TYPE_LOWER}" -type f -executable -name "*trading*" -o -name "*engine*" -o -name "*risk*" 2>/dev/null | sort
fi

echo -e "${YELLOW}Restart your language server to pick up new compile commands${NC}"
