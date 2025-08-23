#!/bin/bash

# FreeFire Max 2.115 64-bit Advanced Cheat Build Script
# Optimized specifically for com.dts.freefiremax

echo "=========================================="
echo "FreeFire Max 2.115 64-bit Advanced Cheat"
echo "Build Script - Optimized Version"
echo "=========================================="

# Check for Android NDK
if [ -z "$ANDROID_NDK_ROOT" ]; then
    echo "Error: ANDROID_NDK_ROOT environment variable not set"
    echo "Please set ANDROID_NDK_ROOT to your Android NDK installation path"
    echo "Example: export ANDROID_NDK_ROOT=/path/to/android-ndk"
    exit 1
fi

if [ ! -f "$ANDROID_NDK_ROOT/ndk-build" ]; then
    echo "Error: ndk-build not found in $ANDROID_NDK_ROOT"
    exit 1
fi

# Set build variables
PROJECT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
BUILD_DIR="$PROJECT_DIR/build"
LIBS_DIR="$PROJECT_DIR/libs"
OBJ_DIR="$PROJECT_DIR/obj"

echo "Project Directory: $PROJECT_DIR"
echo "Build Directory: $BUILD_DIR"
echo "NDK Root: $ANDROID_NDK_ROOT"

# Clean previous build
echo ""
echo "Cleaning previous build..."
rm -rf "$BUILD_DIR"
rm -rf "$LIBS_DIR"
rm -rf "$OBJ_DIR"

# Create build directory
mkdir -p "$BUILD_DIR"

# Build the library
echo ""
echo "Building FreeFire Max 2.115 64-bit Advanced Cheat..."
echo "Target: com.dts.freefiremax (ARM64 only)"

cd "$PROJECT_DIR"
"$ANDROID_NDK_ROOT/ndk-build" \
    NDK_PROJECT_PATH="$PROJECT_DIR" \
    NDK_APPLICATION_MK="$PROJECT_DIR/jni/Application.mk" \
    APP_BUILD_SCRIPT="$PROJECT_DIR/jni/Android.mk" \
    NDK_OUT="$OBJ_DIR" \
    NDK_LIBS_OUT="$LIBS_DIR" \
    -j$(nproc)

BUILD_RESULT=$?

if [ $BUILD_RESULT -eq 0 ]; then
    echo ""
    echo "=========================================="
    echo "BUILD SUCCESSFUL!"
    echo "=========================================="
    
    # Copy built library to build directory
    if [ -f "$LIBS_DIR/arm64-v8a/libFFMaxCheat_2115_64bit.so" ]; then
        cp "$LIBS_DIR/arm64-v8a/libFFMaxCheat_2115_64bit.so" "$BUILD_DIR/"
        
        # Get file info
        FILE_SIZE=$(stat -c%s "$BUILD_DIR/libFFMaxCheat_2115_64bit.so")
        FILE_SIZE_KB=$((FILE_SIZE / 1024))
        
        echo "Built library: $BUILD_DIR/libFFMaxCheat_2115_64bit.so"
        echo "Size: ${FILE_SIZE_KB} KB (${FILE_SIZE} bytes)"
        echo "Architecture: ARM64 (64-bit)"
        echo "Target: com.dts.freefiremax"
        echo "Version: FreeFire Max 2.115"
        echo ""
        echo "Features included:"
        echo "  ✓ Advanced ESP with radar and performance optimization"
        echo "  ✓ Predictive Aimbot with human-like movement"
        echo "  ✓ Anti-detection measures and randomization"
        echo "  ✓ Hardcoded offsets from dump file"
        echo "  ✓ Performance optimizations for ARM64"
        echo "  ✓ Security features and obfuscation"
        echo ""
        echo "Usage Instructions:"
        echo "1. Copy libFFMaxCheat_2115_64bit.so to your module directory"
        echo "2. Ensure target device is ARM64 (64-bit)"
        echo "3. Install as Zygisk module"
        echo "4. Restart device and launch FreeFire Max"
        echo ""
        echo "⚠️  Important Notes:"
        echo "- This build is specifically optimized for FreeFire Max 2.115"
        echo "- Only supports com.dts.freefiremax package"
        echo "- Requires ARM64 (64-bit) device architecture"
        echo "- Use responsibly and follow game terms of service"
        
    else
        echo "Error: Built library not found in expected location"
        exit 1
    fi
    
else
    echo ""
    echo "=========================================="
    echo "BUILD FAILED!"
    echo "=========================================="
    echo "Please check the error messages above and fix any issues."
    exit 1
fi

echo ""
echo "Build completed successfully!"
echo "Library location: $BUILD_DIR/libFFMaxCheat_2115_64bit.so"