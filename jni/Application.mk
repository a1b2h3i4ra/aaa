# FreeFire Max 2.115 64-bit Optimized Build Configuration
APP_ABI := arm64-v8a
APP_PLATFORM := android-21
APP_STL := c++_static
APP_OPTIM := release
APP_THIN_ARCHIVE := true
APP_PIE := true
APP_STRIP_MODE := --strip-all

# Performance and security optimizations
APP_CPPFLAGS := -std=c++17 -O3 -DNDEBUG -flto -ffast-math
APP_CPPFLAGS += -fvisibility=hidden -ffunction-sections -fdata-sections
APP_CPPFLAGS += -march=armv8-a -mtune=cortex-a76 -mcpu=cortex-a76
APP_CPPFLAGS += -fstack-protector-strong -D_FORTIFY_SOURCE=2

# FreeFire Max specific build flags
APP_CPPFLAGS += -DFFMAX_TARGET -DFFMAX_VERSION=2115 -DFFMAX_64BIT_ONLY
APP_CPPFLAGS += -DTARGET_PACKAGE=\"com.dts.freefiremax\" -DADVANCED_FEATURES
APP_CPPFLAGS += -DPERFORMANCE_OPTIMIZED -DANTI_DETECTION_ENABLED

# Linker optimizations
APP_LDFLAGS := -Wl,--gc-sections,--strip-all,--as-needed,--no-undefined
APP_LDFLAGS += -Wl,-z,relro,-z,now,-z,noexecstack -flto

# Project specific settings
APP_PROJECT_PATH := $(call my-dir)/..
APP_BUILD_SCRIPT := $(call my-dir)/Android.mk

