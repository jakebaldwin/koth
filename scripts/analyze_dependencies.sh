#!/bin/bash
# Analyze and suggest dependencies for your services
# Usage: ./scripts/analyze_dependencies.sh [service_name]

set -e

SERVICE_NAME=$1
BUILD_TYPE=${2:-debug}
PROJECT_ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"

echo "=== Trading System Dependency Analysis ==="
echo ""

# Analyze shared libraries
if [ -d "$PROJECT_ROOT/shared_libraries" ]; then
    echo "📚 Available Shared Libraries:"
    echo ""
    
    for lib_dir in "$PROJECT_ROOT/shared_libraries"/*; do
        if [ -d "$lib_dir" ]; then
            lib_name=$(basename "$lib_dir")
            echo "  SharedLibs::$lib_name"
            
            # Show what's in the library
            if [ -d "$lib_dir/include" ]; then
                echo "    📁 Headers:"
                find "$lib_dir/include" -name "*.h" | sed 's|.*/||' | sed 's/^/      /'
            fi
            
            # Check if library is built
            build_dir="$lib_dir/build-$BUILD_TYPE"
            if [ -f "$build_dir/lib${lib_name}.a" ] || [ -f "$PROJECT_ROOT/shared_libraries/build-$BUILD_TYPE/lib${lib_name}.a" ]; then
                echo "    ✅ Built"
            else
                echo "    ❌ Not built (run: cd shared_libraries && ./build.sh)"
            fi
            echo ""
        fi
    done
fi

# Analyze specific service if provided
if [ -n "$SERVICE_NAME" ]; then
    service_dir="$PROJECT_ROOT/services/$SERVICE_NAME"
    
    if [ ! -d "$service_dir" ]; then
        echo "❌ Service '$SERVICE_NAME' not found"
        exit 1
    fi
    
    echo "🔍 Analyzing service: $SERVICE_NAME"
    echo ""
    
    # Check current dependencies
    cmake_file="$service_dir/CMakeLists.txt"
    if [ -f "$cmake_file" ]; then
        echo "📄 Current CMakeLists.txt dependencies:"
        grep -E "SharedLibs::|trading_engine_lib|risk_engine_lib" "$cmake_file" | sed 's/^/  /'
        echo ""
    fi
    
    # Analyze source files for includes
    echo "🔍 Source file analysis:"
    for src_file in "$service_dir/src"/*.cpp "$service_dir/include"/*/*.h; do
        if [ -f "$src_file" ]; then
            filename=$(basename "$src_file")
            echo "  📄 $filename:"
            
            # Check for shared library includes
            grep -E "#include.*common_types|#include.*memory_pool|#include.*symbol_table" "$src_file" | sed 's/^/    /' || echo "    (no shared library includes)"
        fi
    done
    echo ""
    
    # Suggest dependencies based on service type
    echo "💡 Suggested dependencies for $SERVICE_NAME:"
    case "$SERVICE_NAME" in
        *trading*|*engine*)
            echo "  ✅ SharedLibs::common_types    # For Order, Trade, MarketData"
            echo "  ✅ SharedLibs::memory_pool     # For fast order allocation"
            echo "  ✅ SharedLibs::symbol_table    # For symbol lookups"
            echo "  🤔 SharedLibs::trade_history   # If tracking trades"
            ;;
        *risk*)
            echo "  ✅ SharedLibs::common_types    # For position data"
            echo "  🤔 SharedLibs::trade_history   # For risk calculations"
            echo "  🤔 trading_engine_lib         # For order book access"
            ;;
        *market*|*feed*)
            echo "
