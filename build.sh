#!/bin/bash

# Enhanced build script for development and clangd debugging

set -e

BUILD_TYPE=${1:-Debug}
BUILD_TYPE_LOWER=$(echo "$BUILD_TYPE" | tr '[:upper:]' '[:lower:]')
PROJECT_ROOT=$(pwd)

# Get number of CPU cores (cross-platform)
if command -v nproc >/dev/null 2>&1; then
    CORES=$(nproc)
elif command -v sysctl >/dev/null 2>&1; then
    CORES=$(sysctl -n hw.ncpu)  # macOS
else
    CORES=4  # fallback
fi

echo "=== Building shared_libraries ==="
cd shared_libraries

mkdir -p build-${BUILD_TYPE_LOWER}
cd build-${BUILD_TYPE_LOWER}

# Generate compile_commands.json for clangd
cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
      ..

make -j${CORES}

# Copy compile_commands.json to project root for clangd
if [ -f compile_commands.json ]; then
    echo "=== Copying compile_commands.json for clangd ==="
    cp compile_commands.json "$PROJECT_ROOT/"
    echo "compile_commands.json copied to project root"
else
    echo "Warning: compile_commands.json not generated"
fi

# Run tests
echo "=== Running tests ==="
ctest --verbose

echo "=== Build complete ==="
echo "If using clangd, restart your language server to pick up new includes"
