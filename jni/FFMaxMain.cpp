#include <android/log.h>
#include <unistd.h>
#include <thread>
#include <limits>
#include <vector>
#include <memory>
#include <chrono>
#include <atomic>

#include <xdl.h>
#include <KittyUtils.h>
#include <KittyMemory.h>
#include <Il2Cpp.h>
#include <SubstrateHook.h>
#include <CydiaSubstrate.h>

#include "Struct/FFMaxOffsets.h"
#include "Struct/FFMaxStructs.h"
#include "Struct/FFMaxESP.h"
#include "Struct/FFMaxAimbot.h"
#include "Struct/Gui.hpp"
#include "fonts/FontAwesome6_solid.h"
#include "ImGui/Toggle.h"
#include "zygisk.hpp"

using zygisk::Api;
using zygisk::AppSpecializeArgs;
using zygisk::ServerSpecializeArgs;

// Advanced logging system
#define FFMAX_LOG_TAG "FFMax_2115_64bit"
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG, FFMAX_LOG_TAG, __VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, FFMAX_LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, FFMAX_LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, FFMAX_LOG_TAG, __VA_ARGS__)

// Global state management
namespace FFMaxGlobals {
    using namespace FFMaxStructs;
    
    // Core game state
    CheatConfig g_Config;
    std::vector<Player> g_PlayerList;
    Player g_LocalPlayer(nullptr);
    Camera g_MainCamera(nullptr);
    
    // Memory and IL2CPP
    uintptr_t il2cpp_base = 0;
    uintptr_t unity_base = 0;
    
    // State flags
    std::atomic<bool> g_IsInitialized{false};
    std::atomic<bool> g_IsGameActive{false};
    std::atomic<bool> g_IsInMatch{false};
    std::atomic<bool> g_IsMenuOpen{false};
    
    // Performance tracking
    std::chrono::high_resolution_clock::time_point last_update;
    float fps_counter = 0.0f;
    int frame_count = 0;
    
    // Anti-detection
    std::atomic<bool> g_AntiDetectionEnabled{true};
    std::chrono::high_resolution_clock::time_point last_detection_check;
    
    // Feature toggles with improved defaults
    struct {
        // ESP Features
        bool esp_enabled = true;
        bool esp_box = true;
        bool esp_health = true;
        bool esp_distance = true;
        bool esp_name = true;
        bool esp_weapon = true;
        bool esp_bone = false;
        bool esp_traceline = true;
        
        // Aimbot Features
        bool aimbot_enabled = true;
        bool aimbot_smooth = true;
        bool aimbot_fov_check = true;
        bool aimbot_visible_check = true;
        bool aimbot_team_check = true;
        float aimbot_fov = 60.0f;
        float aimbot_smooth_factor = 5.0f;
        int aimbot_target_bone = 0; // 0=head, 1=chest, 2=pelvis
        
        // Advanced Features
        bool no_recoil = false;
        bool no_spread = false;
        bool rapid_fire = false;
        bool infinite_ammo = false;
        bool speed_hack = false;
        bool fly_hack = false;
        bool wall_hack = false;
        bool god_mode = false;
        
        // Visual Enhancements
        bool remove_fog = false;
        bool bright_mode = false;
        bool crosshair = true;
        bool radar = true;
        
        // Anti-Detection
        bool randomize_delays = true;
        bool human_like_movement = true;
        bool detection_bypass = true;
        
    } features;
}

// Export globals for other modules
namespace FFMaxESP {
    CheatConfig& g_Config = FFMaxGlobals::g_Config;
    std::vector<Player>& g_PlayerList = FFMaxGlobals::g_PlayerList;
    Player& g_LocalPlayer = FFMaxGlobals::g_LocalPlayer;
    Camera& g_MainCamera = FFMaxGlobals::g_MainCamera;
}

namespace FFMaxAimbot {
    CheatConfig& g_Config = FFMaxGlobals::g_Config;
    std::vector<Player>& g_PlayerList = FFMaxGlobals::g_PlayerList;
    Player& g_LocalPlayer = FFMaxGlobals::g_LocalPlayer;
    Camera& g_MainCamera = FFMaxGlobals::g_MainCamera;
}

// Forward declarations
void InitializeFFMaxCheat();
void UpdateGameState();
void UpdatePlayerList();
void ProcessFeatures();
void AntiDetectionRoutine();

class FFMaxModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api_ = api;
        this->env_ = env;
        genv = env;
        
        LOGI("FFMax Module loaded - Version 2.115 64-bit Optimized");
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        // Specifically target FreeFire Max only
        static constexpr const char *TARGET_PACKAGE = "com.dts.freefiremax";
        
        const char *process = env_->GetStringUTFChars(args->nice_name, nullptr);
        is_target_game_ = (strcmp(process, TARGET_PACKAGE) == 0);
        env_->ReleaseStringUTFChars(args->nice_name, process);
        
        if (is_target_game_) {
            LOGI("FreeFire Max 2.115 detected - Initializing advanced cheat system");
            LOGI("Target: %s", TARGET_PACKAGE);
        } else {
            LOGD("Non-target process detected: %s", process);
        }
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {
        if (is_target_game_) {
            LOGI("Post-specialization: Starting FFMax cheat thread");
            genv->GetJavaVM(&jvm);
            
            // Start the main cheat thread
            std::thread cheat_thread([]() {
                // Wait for game to fully load
                std::this_thread::sleep_for(std::chrono::seconds(3));
                InitializeFFMaxCheat();
            });
            cheat_thread.detach();
            
            // Start anti-detection thread
            std::thread anti_detection_thread([]() {
                while (true) {
                    std::this_thread::sleep_for(std::chrono::milliseconds(500));
                    AntiDetectionRoutine();
                }
            });
            anti_detection_thread.detach();
        }
    }

private:
    Api *api_ = nullptr;
    JNIEnv *env_ = nullptr;
    bool is_target_game_ = false;
};

// Advanced UI Theme specifically for FreeFire Max
void SetFFMaxAdvancedTheme() {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    // FreeFire Max Orange/Black theme
    colors[ImGuiCol_Text]                   = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.060f, 0.060f, 0.070f, 0.950f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.110f, 0.110f, 0.140f, 0.920f);
    colors[ImGuiCol_Border]                 = ImVec4(0.500f, 0.500f, 0.500f, 0.500f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.430f, 0.430f, 0.430f, 0.390f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.470f, 0.470f, 0.470f, 0.400f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.420f, 0.410f, 0.640f, 0.690f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.270f, 0.270f, 0.540f, 0.830f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.320f, 0.320f, 0.630f, 0.870f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.400f, 0.400f, 0.800f, 0.200f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.400f, 0.400f, 0.550f, 0.800f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.200f, 0.250f, 0.300f, 0.600f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.400f, 0.400f, 0.800f, 0.300f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.400f, 0.400f, 0.800f, 0.400f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.410f, 0.390f, 0.800f, 0.600f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.900f, 0.900f, 0.900f, 0.500f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(1.000f, 1.000f, 1.000f, 0.300f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.410f, 0.390f, 0.800f, 0.600f);
    colors[ImGuiCol_Button]                 = ImVec4(0.350f, 0.400f, 0.610f, 0.620f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.400f, 0.480f, 0.710f, 0.790f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.460f, 0.540f, 0.800f, 1.000f);
    colors[ImGuiCol_Header]                 = ImVec4(0.400f, 0.400f, 0.900f, 0.450f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.450f, 0.450f, 0.900f, 0.800f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.530f, 0.530f, 0.870f, 0.800f);
    colors[ImGuiCol_Separator]              = ImVec4(0.500f, 0.500f, 0.500f, 0.600f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.600f, 0.600f, 0.700f, 1.000f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.700f, 0.700f, 0.900f, 1.000f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(1.000f, 1.000f, 1.000f, 0.100f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.780f, 0.820f, 1.000f, 0.600f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.780f, 0.820f, 1.000f, 0.900f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.340f, 0.340f, 0.680f, 0.790f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.450f, 0.450f, 0.900f, 0.800f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.400f, 0.400f, 0.730f, 0.840f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.280f, 0.280f, 0.570f, 0.820f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.350f, 0.350f, 0.650f, 0.840f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(0.900f, 0.700f, 0.000f, 1.000f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(0.900f, 0.700f, 0.000f, 1.000f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.000f, 0.600f, 0.000f, 1.000f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.000f, 0.000f, 1.000f, 0.350f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(1.000f, 1.000f, 0.000f, 0.900f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(0.450f, 0.450f, 0.900f, 0.800f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.000f, 1.000f, 1.000f, 0.700f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.800f, 0.800f, 0.800f, 0.200f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.200f, 0.200f, 0.200f, 0.350f);

    // Style adjustments
    style->WindowRounding    = 2.0f;
    style->FrameRounding     = 2.0f;
    style->PopupRounding     = 2.0f;
    style->ScrollbarRounding = 3.0f;
    style->GrabRounding      = 2.0f;
    style->TabRounding       = 2.0f;
    style->WindowBorderSize  = 1.0f;
    style->FrameBorderSize   = 0.0f;
    style->PopupBorderSize   = 1.0f;
    style->TabBorderSize     = 0.0f;
}

// Initialize the cheat system
void InitializeFFMaxCheat() {
    LOGI("Initializing FFMax cheat system...");
    
    try {
        // Get IL2CPP base address
        FFMaxGlobals::il2cpp_base = (uintptr_t)xdl_open("libil2cpp.so", 0);
        if (!FFMaxGlobals::il2cpp_base) {
            LOGE("Failed to get IL2CPP base address");
            return;
        }
        LOGI("IL2CPP base: 0x%lx", FFMaxGlobals::il2cpp_base);
        
        // Get Unity base address  
        FFMaxGlobals::unity_base = (uintptr_t)xdl_open("libunity.so", 0);
        if (!FFMaxGlobals::unity_base) {
            LOGW("Unity base not found, using IL2CPP base");
            FFMaxGlobals::unity_base = FFMaxGlobals::il2cpp_base;
        }
        LOGI("Unity base: 0x%lx", FFMaxGlobals::unity_base);
        
        // Initialize IL2CPP
        if (!Il2Cpp::Initialize()) {
            LOGE("Failed to initialize IL2CPP");
            return;
        }
        LOGI("IL2CPP initialized successfully");
        
        // Set up ImGui with advanced theme
        SetFFMaxAdvancedTheme();
        
        // Mark as initialized
        FFMaxGlobals::g_IsInitialized = true;
        FFMaxGlobals::last_update = std::chrono::high_resolution_clock::now();
        
        LOGI("FFMax cheat system initialized successfully!");
        
        // Main game loop
        while (true) {
            if (FFMaxGlobals::g_IsInitialized && FFMaxGlobals::g_IsGameActive) {
                UpdateGameState();
                UpdatePlayerList();
                ProcessFeatures();
                
                // Performance tracking
                FFMaxGlobals::frame_count++;
                auto now = std::chrono::high_resolution_clock::now();
                auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - FFMaxGlobals::last_update);
                
                if (duration.count() >= 1000) { // Update FPS every second
                    FFMaxGlobals::fps_counter = FFMaxGlobals::frame_count;
                    FFMaxGlobals::frame_count = 0;
                    FFMaxGlobals::last_update = now;
                }
            }
            
            // Adaptive sleep based on game state
            if (FFMaxGlobals::g_IsInMatch) {
                std::this_thread::sleep_for(std::chrono::milliseconds(16)); // ~60 FPS
            } else {
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // Lower frequency when not in match
            }
        }
        
    } catch (const std::exception& e) {
        LOGE("Exception in InitializeFFMaxCheat: %s", e.what());
    } catch (...) {
        LOGE("Unknown exception in InitializeFFMaxCheat");
    }
}

// Update game state
void UpdateGameState() {
    // Check if we're in a match
    // This would typically check game manager or scene state
    FFMaxGlobals::g_IsInMatch = true; // Placeholder - implement actual detection
    
    // Get main camera
    if (!FFMaxGlobals::g_MainCamera.instance) {
        // Try to get main camera using IL2CPP
        auto camera_class = Il2Cpp::Class::Find("UnityEngine.Camera");
        if (camera_class) {
            auto main_method = Il2Cpp::Class::GetMethodFromName(camera_class, "get_main", 0);
            if (main_method) {
                auto main_camera = Il2Cpp::Runtime::Invoke(main_method, nullptr, nullptr);
                if (main_camera) {
                    FFMaxGlobals::g_MainCamera = Camera(main_camera);
                    LOGD("Main camera acquired");
                }
            }
        }
    }
}

// Update player list
void UpdatePlayerList() {
    if (!FFMaxGlobals::g_IsInMatch) return;
    
    // Clear previous player list
    FFMaxGlobals::g_PlayerList.clear();
    
    // Find all player objects
    // This is a simplified version - actual implementation would scan for player objects
    try {
        auto player_class = Il2Cpp::Class::Find("Player");
        if (player_class) {
            // Get all instances of Player class
            // Implementation would depend on how the game stores player references
            LOGD("Player class found, updating player list");
        }
    } catch (...) {
        LOGD("Error updating player list");
    }
}

// Process all enabled features
void ProcessFeatures() {
    using namespace FFMaxGlobals;
    
    if (!g_IsInitialized || !g_IsInMatch) return;
    
    // ESP Processing
    if (features.esp_enabled && !g_PlayerList.empty()) {
        // Process ESP for all players
        // Implementation would render ESP elements
    }
    
    // Aimbot Processing
    if (features.aimbot_enabled && g_LocalPlayer.instance) {
        // Process aimbot logic
        // Implementation would handle target selection and aim adjustment
    }
    
    // Other features
    if (features.no_recoil) {
        // Implement no recoil
    }
    
    if (features.no_spread) {
        // Implement no spread
    }
    
    // Add more feature processing as needed
}

// Anti-detection routine
void AntiDetectionRoutine() {
    using namespace FFMaxGlobals;
    
    if (!g_AntiDetectionEnabled) return;
    
    auto now = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::seconds>(now - last_detection_check);
    
    if (duration.count() >= 5) { // Check every 5 seconds
        // Randomize some behaviors
        if (features.randomize_delays) {
            // Add small random delays to actions
        }
        
        // Check for detection patterns
        // Implementation would monitor for anti-cheat signatures
        
        last_detection_check = now;
    }
}

// Advanced menu rendering
void RenderAdvancedMenu() {
    using namespace FFMaxGlobals;
    
    if (!g_IsInitialized || !g_IsMenuOpen) return;
    
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    ImGui::Begin("FFMax 2.115 Advanced Cheat", &g_IsMenuOpen, ImGuiWindowFlags_NoCollapse);
    
    // Performance info
    ImGui::Text("FPS: %.1f | Players: %zu", fps_counter, g_PlayerList.size());
    ImGui::Text("Status: %s", g_IsInMatch ? "In Match" : "In Lobby");
    ImGui::Separator();
    
    if (ImGui::BeginTabBar("MainTabs")) {
        if (ImGui::BeginTabItem("ESP")) {
            ImGui::Checkbox("Enable ESP", &features.esp_enabled);
            ImGui::Checkbox("Box ESP", &features.esp_box);
            ImGui::Checkbox("Health ESP", &features.esp_health);
            ImGui::Checkbox("Distance ESP", &features.esp_distance);
            ImGui::Checkbox("Name ESP", &features.esp_name);
            ImGui::Checkbox("Weapon ESP", &features.esp_weapon);
            ImGui::Checkbox("Bone ESP", &features.esp_bone);
            ImGui::Checkbox("Traceline ESP", &features.esp_traceline);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Aimbot")) {
            ImGui::Checkbox("Enable Aimbot", &features.aimbot_enabled);
            ImGui::Checkbox("Smooth Aimbot", &features.aimbot_smooth);
            ImGui::Checkbox("FOV Check", &features.aimbot_fov_check);
            ImGui::Checkbox("Visibility Check", &features.aimbot_visible_check);
            ImGui::Checkbox("Team Check", &features.aimbot_team_check);
            
            ImGui::SliderFloat("FOV", &features.aimbot_fov, 10.0f, 180.0f);
            ImGui::SliderFloat("Smooth Factor", &features.aimbot_smooth_factor, 1.0f, 20.0f);
            
            const char* bones[] = { "Head", "Chest", "Pelvis" };
            ImGui::Combo("Target Bone", &features.aimbot_target_bone, bones, 3);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Advanced")) {
            ImGui::Checkbox("No Recoil", &features.no_recoil);
            ImGui::Checkbox("No Spread", &features.no_spread);
            ImGui::Checkbox("Rapid Fire", &features.rapid_fire);
            ImGui::Checkbox("Infinite Ammo", &features.infinite_ammo);
            ImGui::Checkbox("Speed Hack", &features.speed_hack);
            ImGui::Checkbox("Fly Hack", &features.fly_hack);
            ImGui::Checkbox("Wall Hack", &features.wall_hack);
            ImGui::Checkbox("God Mode", &features.god_mode);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Visual")) {
            ImGui::Checkbox("Remove Fog", &features.remove_fog);
            ImGui::Checkbox("Bright Mode", &features.bright_mode);
            ImGui::Checkbox("Crosshair", &features.crosshair);
            ImGui::Checkbox("Radar", &features.radar);
            ImGui::EndTabItem();
        }
        
        if (ImGui::BeginTabItem("Anti-Detection")) {
            ImGui::Checkbox("Enable Anti-Detection", &g_AntiDetectionEnabled);
            ImGui::Checkbox("Randomize Delays", &features.randomize_delays);
            ImGui::Checkbox("Human-like Movement", &features.human_like_movement);
            ImGui::Checkbox("Detection Bypass", &features.detection_bypass);
            ImGui::EndTabItem();
        }
        
        ImGui::EndTabBar();
    }
    
    ImGui::End();
}

// Hook for rendering
void RenderHook() {
    if (FFMaxGlobals::g_IsInitialized) {
        RenderAdvancedMenu();
        
        // Render ESP and other visual features
        if (FFMaxGlobals::features.esp_enabled) {
            // ESP rendering code would go here
        }
        
        if (FFMaxGlobals::features.crosshair) {
            // Crosshair rendering code would go here
        }
    }
}

// Main hack function - entry point
void hack() {
    LOGI("FFMax hack thread started");
    
    // Wait for game initialization
    while (!FFMaxGlobals::g_IsGameActive) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    // Initialize the cheat system
    InitializeFFMaxCheat();
}

REGISTER_ZYGISK_MODULE(FFMaxModule)