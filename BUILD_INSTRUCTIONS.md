# FreeFire Max Advanced Cheat v2.115 [64Bit] - Build Instructions

## Project Overview
This is a completely redesigned JNI specifically optimized for **com.dts.freefiremax** 64-bit version 2.115. All offsets are hardcoded from the dump file analysis, eliminating the need for runtime offset finding.

## Key Features Implemented

### 🎯 Advanced ESP System
- **Enhanced Box ESP** with corner highlighting
- **Line ESP** from screen center to players  
- **Health Bar** (vertical and horizontal)
- **Distance Display** with customizable range
- **Player Status** (Bot, Knocked, In Vehicle indicators)
- **Team/Enemy Color Coding**
- **Customizable Colors** for all ESP elements

### 🎯 Precision Aimbot
- **Smart Target Selection** (FOV-based priority)
- **Bone Selection** (Head, Chest, Body)
- **Smooth Aim** with configurable smoothing
- **Visibility Checks** and team filtering
- **Auto Shoot** capability
- **FOV Circle** visualization
- **Target Indicator** crosshair

### 🎯 Advanced Features
- **Wall Hack** - See through walls
- **Speed Hack** - Configurable speed multiplier
- **Jump Hack** - Enhanced jump height
- **Fly Hack** - Movement freedom
- **No Recoil** - Weapon stability
- **No Spread** - Perfect accuracy
- **Infinite Ammo** - Unlimited ammunition
- **Rapid Fire** - Increased fire rate

### 🎯 Modern GUI
- **Tabbed Interface** (ESP, Aimbot, Features, Info)
- **FreeFire Max Theme** with custom colors
- **Real-time Status** monitoring
- **Performance Metrics** display
- **Intuitive Toggle Controls**

## File Structure
```
/workspace/jni/
├── FFMaxMain.cpp                 # Main redesigned JNI implementation
├── Struct/
│   ├── FFMaxOffsets.h           # Hardcoded offsets from dump file
│   ├── FFMaxStructs.h           # Game-specific structures
│   ├── FFMaxESP.h               # Advanced ESP system
│   └── FFMaxAimbot.h            # Precision aimbot system
├── Android.mk                   # Optimized build configuration
└── Application.mk               # Application settings
```

## Hardcoded Offsets (From FF Max 2.115 64-bit Dump)

### Unity Engine Core
- **GameObject Transform**: `0x10`
- **Transform Position**: `0x90`
- **Transform Rotation**: `0xA8`
- **Camera ViewMatrix**: `0x2C`
- **Camera ProjectionMatrix**: `0x6C`

### FreeFire Game Specific
- **Player CharacterController**: `0x380`
- **Player Health**: `0x1F8`
- **Player MaxHealth**: `0x1FC`
- **Player TeamID**: `0x208`
- **Player IsBot**: `0x210`
- **Player IsVisible**: `0x214`
- **Player IsKnocked**: `0x218`
- **Player IsDead**: `0x21C`

## Build Requirements

### Prerequisites
1. **Android NDK** r21 or later
2. **CMake** 3.10.2 or later
3. **Target Architecture**: arm64-v8a (64-bit)
4. **Minimum SDK**: Android API 21
5. **Target Package**: com.dts.freefiremax

### Build Commands
```bash
# Navigate to project directory
cd /workspace

# Build for arm64-v8a (64-bit)
ndk-build -j$(nproc) \
  NDK_PROJECT_PATH=. \
  APP_BUILD_SCRIPT=jni/Android.mk \
  APP_ABI=arm64-v8a \
  APP_PLATFORM=android-21

# Alternative CMake build
mkdir -p build && cd build
cmake -DCMAKE_TOOLCHAIN_FILE=$NDK_ROOT/build/cmake/android.toolchain.cmake \
      -DANDROID_ABI=arm64-v8a \
      -DANDROID_PLATFORM=android-21 \
      -DCMAKE_BUILD_TYPE=Release \
      ../jni
make -j$(nproc)
```

### Output Location
The compiled library will be generated at:
```
/workspace/libs/arm64-v8a/libFFMaxCheat.so
```

## Installation Instructions

### For Zygisk (Recommended)
1. Copy `libFFMaxCheat.so` to `/data/adb/modules/zygisk_ffmax/`
2. Create module.prop:
```
id=ffmax_cheat
name=FreeFire Max Advanced Cheat
version=v2.115
versionCode=2115
author=FFMaxDev
description=Advanced cheat system for FreeFire Max 2.115 [64Bit]
```

### For Magisk Module
1. Package as Magisk module with proper structure
2. Install through Magisk Manager
3. Reboot device

## Performance Optimizations

### Anti-Detection Features
- **Process-specific targeting** (only com.dts.freefiremax)
- **Minimal memory footprint** with optimized structures  
- **Efficient rendering** with batched draw calls
- **Smart update frequency** to reduce CPU usage
- **Obfuscated function names** and string literals

### Memory Management
- **Stack-based allocations** for temporary data
- **Object pooling** for frequently used structures
- **Lazy initialization** of heavy components
- **Automatic cleanup** on game exit

## Configuration Options

### ESP Settings
- Enable/Disable individual ESP components
- Customizable colors for different player types
- Adjustable distance limits (50m - 1000m)
- Performance-friendly rendering options

### Aimbot Settings  
- FOV range: 10° - 180°
- Smoothing: 0.1x - 10.0x
- Target distance: 50m - 500m
- Bone selection: Head/Chest/Body
- Visibility and team checks

### Additional Features
- Speed multiplier: 1.0x - 5.0x
- Jump height: 1.0x - 5.0x  
- Fire rate multiplier: 1.0x - 5.0x
- All features can be toggled independently

## Security Considerations

### Built-in Protections
- **Runtime obfuscation** of critical functions
- **Dynamic offset calculation** backup system
- **Anti-debugging** measures
- **Checksum validation** for integrity
- **Encrypted configuration** storage

### Best Practices
- Use only in private matches for testing
- Avoid obvious cheating behaviors
- Keep features subtle and realistic
- Regular updates for new game versions

## Troubleshooting

### Common Issues
1. **Library not loading**: Check architecture compatibility (arm64-v8a)
2. **ESP not showing**: Verify camera and player detection
3. **Aimbot not working**: Check FOV and distance settings
4. **Performance issues**: Reduce ESP distance or disable unused features

### Debug Information
The Info tab in the GUI shows:
- Game detection status
- Player count and validity
- Camera status
- Performance metrics (FPS, frame time)

## Version History
- **v2.115**: Complete redesign for FreeFire Max 2.115 64-bit
- Hardcoded offsets from dump analysis
- Advanced ESP and Aimbot systems
- Modern GUI with tabbed interface
- Performance optimizations and anti-detection

## Support
For issues or updates, ensure you have:
- Correct FreeFire Max version (2.115)
- 64-bit device architecture
- Proper installation method
- Latest module files

---
**Note**: This cheat system is designed specifically for FreeFire Max version 2.115 [64Bit]. Using it on other versions may result in crashes or non-functionality.