LOCAL_PATH := $(call my-dir)
MAIN_LOCAL_PATH := $(call my-dir)

# ============================================================================#
include $(CLEAR_VARS)
LOCAL_MODULE    := libdobby
LOCAL_SRC_FILES := Dobby/$(TARGET_ARCH_ABI)/libdobby.a
include $(PREBUILT_STATIC_LIBRARY)
# ============================================================================#

include $(CLEAR_VARS)

LOCAL_MODULE           := FFMaxCheat_2115_64bit

# Optimized build flags for performance and security
LOCAL_CFLAGS           := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w
LOCAL_CFLAGS           += -fno-rtti -fno-exceptions -fpermissive -O3 -DNDEBUG -flto -ffast-math
LOCAL_CFLAGS           += -march=armv8-a -mtune=cortex-a76 -mfpu=neon-fp-armv8
LOCAL_CPPFLAGS         := -Wno-error=format-security -fvisibility=hidden -ffunction-sections -fdata-sections -w -Werror -s -std=c++17
LOCAL_CPPFLAGS         += -Wno-error=c++11-narrowing -fms-extensions -fno-rtti -fno-exceptions -fpermissive -O3 -DNDEBUG -flto
LOCAL_CPPFLAGS         += -march=armv8-a -mtune=cortex-a76 -mfpu=neon-fp-armv8 -ffast-math
LOCAL_LDFLAGS          += -Wl,--gc-sections,--strip-all,--as-needed,--no-undefined -llog -s -flto
LOCAL_ARM_MODE         := arm
LOCAL_LDLIBS           := -llog -landroid -lEGL -lGLESv3 -lGLESv2 -lGLESv1_CM -lz -lm

LOCAL_C_INCLUDES       += $(LOCAL_PATH)
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Dobby
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/imgui
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/KittyMemory
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Unity
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/Struct
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include/And64InlineHook
LOCAL_C_INCLUDES       += $(LOCAL_PATH)/include/Substrate

# FreeFire Max 2.115 64-bit specific source files
LOCAL_SRC_FILES := FFMaxMain.cpp

# Include ImGui and other necessary files
FILE_LIST               := $(wildcard $(LOCAL_PATH)/imgui/*.c*)
FILE_LIST               += $(wildcard $(LOCAL_PATH)/xdl/*.c*)
FILE_LIST               += $(wildcard $(LOCAL_PATH)/KittyMemory/*.c*)
FILE_LIST              += $(wildcard $(LOCAL_PATH)/CheatCode/IL2CppSDKGenerator/*.c*)

LOCAL_SRC_FILES        += $(FILE_LIST:$(LOCAL_PATH)/%=%)

# Architecture specific optimizations for ARM64 only (64-bit)
ifeq ($(TARGET_ARCH_ABI), arm64-v8a)
    LOCAL_C_INCLUDES += $(LOCAL_PATH)/include/And64InlineHook
    HOOK_SRC := $(wildcard $(LOCAL_PATH)/include/And64InlineHook/*.c*)
    LOCAL_SRC_FILES += $(HOOK_SRC:$(LOCAL_PATH)/%=%)
    LOCAL_CFLAGS += -DARM64 -DFFMAX_64BIT
    LOCAL_CPPFLAGS += -DARM64 -DFFMAX_64BIT
    # ARM64 specific optimizations
    LOCAL_CFLAGS += -mcpu=cortex-a76 -mtune=cortex-a76
    LOCAL_CPPFLAGS += -mcpu=cortex-a76 -mtune=cortex-a76
else
    $(error This build is specifically for ARM64 (64-bit) architecture only)
endif

# FreeFire Max 2.115 64-bit specific definitions
LOCAL_CFLAGS += -DFFMAX_TARGET -DFFMAX_VERSION=2115 -DFFMAX_64BIT_ONLY
LOCAL_CFLAGS += -DTARGET_PACKAGE=\"com.dts.freefiremax\" -DADVANCED_FEATURES
LOCAL_CFLAGS += -DPERFORMANCE_OPTIMIZED -DANTI_DETECTION_ENABLED
LOCAL_CPPFLAGS += -DFFMAX_TARGET -DFFMAX_VERSION=2115 -DFFMAX_64BIT_ONLY
LOCAL_CPPFLAGS += -DTARGET_PACKAGE=\"com.dts.freefiremax\" -DADVANCED_FEATURES
LOCAL_CPPFLAGS += -DPERFORMANCE_OPTIMIZED -DANTI_DETECTION_ENABLED

# Security and obfuscation flags
LOCAL_CFLAGS += -fstack-protector-strong -D_FORTIFY_SOURCE=2
LOCAL_CPPFLAGS += -fstack-protector-strong -D_FORTIFY_SOURCE=2
LOCAL_LDFLAGS += -Wl,-z,relro,-z,now,-z,noexecstack

LOCAL_STATIC_LIBRARIES := libdobby
LOCAL_CPP_FEATURES     := exceptions

# Build as shared library with specific name
include $(BUILD_SHARED_LIBRARY)

# ============================================================================
# Create a stripped version for release
include $(CLEAR_VARS)
LOCAL_MODULE := FFMaxCheat_2115_64bit_stripped
LOCAL_SRC_FILES := $(TARGET_OUT_INTERMEDIATE_LIBRARIES)/libFFMaxCheat_2115_64bit.so
LOCAL_MODULE_CLASS := SHARED_LIBRARIES
LOCAL_MODULE_SUFFIX := .so
LOCAL_STRIP_MODULE := true
include $(BUILD_PREBUILT)
# ============================================================================

