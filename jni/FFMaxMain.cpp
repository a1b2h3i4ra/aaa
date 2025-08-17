#include <android/log.h>
#include <unistd.h>
#include <thread>
#include <limits>
#include <vector>
#include <memory>

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

// Global variables
namespace FFMaxGlobals {
    using namespace FFMaxStructs;
    
    CheatConfig g_Config;
    std::vector<Player> g_PlayerList;
    Player g_LocalPlayer(nullptr);
    Camera g_MainCamera(nullptr);
    
    uintptr_t il2cpp_base = 0;
    bool g_IsInitialized = false;
    bool g_IsGameActive = false;
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

void hack();

class FFMaxModule : public zygisk::ModuleBase {
public:
    void onLoad(Api *api, JNIEnv *env) override {
        this->api_ = api;
        this->env_ = env;
        genv = env;
    }

    void preAppSpecialize(AppSpecializeArgs *args) override {
        // Only target FreeFire Max specifically
        static constexpr const char *TARGET_PACKAGE = "com.dts.freefiremax";
        
        const char *process = env_->GetStringUTFChars(args->nice_name, nullptr);
        is_target_game_ = (strcmp(process, TARGET_PACKAGE) == 0);
        env_->ReleaseStringUTFChars(args->nice_name, process);
        
        if (is_target_game_) {
            LOGD("FreeFire Max detected - initializing advanced cheat system");
        }
    }

    void postAppSpecialize(const AppSpecializeArgs *args) override {
        if (is_target_game_) {
            genv->GetJavaVM(&jvm);
            std::thread{hack}.detach();
        }
    }

private:
    Api *api_ = nullptr;
    JNIEnv *env_ = nullptr;
    bool is_target_game_ = false;
};

// Advanced UI Theme
void SetFFMaxTheme() {
    ImGuiStyle* style = &ImGui::GetStyle();
    ImVec4* colors = style->Colors;

    // FreeFire Max themed colors
    colors[ImGuiCol_Text]                   = ImVec4(1.000f, 1.000f, 1.000f, 1.000f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.500f, 0.500f, 0.500f, 1.000f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.110f, 0.110f, 0.110f, 0.950f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.150f, 0.150f, 0.150f, 0.000f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.110f, 0.110f, 0.110f, 0.940f);
    colors[ImGuiCol_Border]                 = ImVec4(0.430f, 0.430f, 0.500f, 0.500f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.000f, 0.000f, 0.000f, 0.000f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.200f, 0.200f, 0.200f, 0.540f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.400f, 0.400f, 0.400f, 0.400f);
    colors[ImGuiCol_FrameBgActive]          = ImVec4(0.600f, 0.600f, 0.600f, 0.670f);
    colors[ImGuiCol_TitleBg]                = ImVec4(0.040f, 0.040f, 0.040f, 1.000f);
    colors[ImGuiCol_TitleBgActive]          = ImVec4(0.160f, 0.290f, 0.480f, 1.000f);
    colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.000f, 0.000f, 0.000f, 0.510f);
    colors[ImGuiCol_MenuBarBg]              = ImVec4(0.140f, 0.140f, 0.140f, 1.000f);
    colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.020f, 0.020f, 0.020f, 0.530f);
    colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.310f, 0.310f, 0.310f, 1.000f);
    colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.410f, 0.410f, 0.410f, 1.000f);
    colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.510f, 0.510f, 0.510f, 1.000f);
    colors[ImGuiCol_CheckMark]              = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
    colors[ImGuiCol_SliderGrab]             = ImVec4(0.240f, 0.520f, 0.880f, 1.000f);
    colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
    colors[ImGuiCol_Button]                 = ImVec4(0.260f, 0.590f, 0.980f, 0.400f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.060f, 0.530f, 0.980f, 1.000f);
    colors[ImGuiCol_Header]                 = ImVec4(0.260f, 0.590f, 0.980f, 0.310f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.260f, 0.590f, 0.980f, 0.800f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.260f, 0.590f, 0.980f, 1.000f);

    style->WindowRounding = 7.0f;
    style->FrameRounding = 5.0f;
    style->PopupRounding = 4.0f;
    style->ScrollbarRounding = 9.0f;
    style->GrabRounding = 3.0f;
    style->TabRounding = 4.0f;
    style->WindowBorderSize = 1.0f;
    style->FrameBorderSize = 1.0f;
}

// Game Data Management
void UpdateGameData() {
    using namespace FFMaxGlobals;
    using namespace FFMaxStructs;
    
    if (!g_IsInitialized) return;
    
    // Update main camera
    void* mainCameraPtr = nullptr; // This would be obtained from the game
    g_MainCamera = Camera(mainCameraPtr);
    
    // Update local player
    void* localPlayerPtr = nullptr; // This would be obtained from the game
    g_LocalPlayer = Player(localPlayerPtr);
    
    // Update player list
    g_PlayerList.clear();
    
    // This would iterate through all players in the game
    // For demonstration, we'll leave this as a placeholder
    // In a real implementation, you would:
    // 1. Find the player manager/list in memory
    // 2. Iterate through all player instances
    // 3. Add valid players to g_PlayerList
    
    /*
    void* playerManagerPtr = GetPlayerManager();
    if (playerManagerPtr) {
        auto playerArray = GetPlayerArray(playerManagerPtr);
        for (auto playerPtr : playerArray) {
            if (playerPtr && IsValidPlayer(playerPtr)) {
                g_PlayerList.emplace_back(playerPtr);
            }
        }
    }
    */
}

// Advanced GUI
void RenderAdvancedGUI() {
    using namespace FFMaxGlobals;
    
    static bool showMainWindow = true;
    static int currentTab = 0;
    
    if (!showMainWindow) return;
    
    SetFFMaxTheme();
    
    ImGui::SetNextWindowSize(ImVec2(600, 500), ImGuiCond_FirstUseEver);
    
    if (ImGui::Begin("FreeFire Max Advanced Cheat v2.115", &showMainWindow, ImGuiWindowFlags_NoCollapse)) {
        
        // Tab bar
        if (ImGui::BeginTabBar("CheatTabs")) {
            
            // ESP Tab
            if (ImGui::BeginTabItem("ESP")) {
                ImGui::Spacing();
                
                Toggle("Enable ESP", &g_Config.esp.enabled);
                ImGui::Separator();
                
                if (g_Config.esp.enabled) {
                    ImGui::Text("ESP Features:");
                    Toggle("Show Box", &g_Config.esp.showBox);
                    Toggle("Show Line", &g_Config.esp.showLine);
                    Toggle("Show Health", &g_Config.esp.showHealth);
                    Toggle("Show Distance", &g_Config.esp.showDistance);
                    Toggle("Show Team", &g_Config.esp.showTeam);
                    Toggle("Show Bots", &g_Config.esp.showBots);
                    Toggle("Show Knocked", &g_Config.esp.showKnocked);
                    Toggle("Show In Vehicle", &g_Config.esp.showInVehicle);
                    
                    ImGui::Separator();
                    ImGui::SliderFloat("Max Distance", &g_Config.esp.maxDistance, 50.0f, 1000.0f, "%.0fm");
                    
                    ImGui::Separator();
                    ImGui::Text("Colors:");
                    ImGui::ColorEdit4("Enemy", (float*)&g_Config.esp.colors.enemy);
                    ImGui::ColorEdit4("Team", (float*)&g_Config.esp.colors.team);
                    ImGui::ColorEdit4("Bot", (float*)&g_Config.esp.colors.bot);
                    ImGui::ColorEdit4("Knocked", (float*)&g_Config.esp.colors.knocked);
                }
                
                ImGui::EndTabItem();
            }
            
            // Aimbot Tab
            if (ImGui::BeginTabItem("Aimbot")) {
                ImGui::Spacing();
                
                Toggle("Enable Aimbot", &g_Config.aimbot.enabled);
                ImGui::Separator();
                
                if (g_Config.aimbot.enabled) {
                    ImGui::Text("Aimbot Settings:");
                    Toggle("Aim Key", &g_Config.aimbot.aimKey);
                    Toggle("Auto Shoot", &g_Config.aimbot.autoShoot);
                    Toggle("Visibility Check", &g_Config.aimbot.visibilityCheck);
                    Toggle("Team Check", &g_Config.aimbot.teamCheck);
                    Toggle("Bot Check", &g_Config.aimbot.botCheck);
                    Toggle("Knocked Check", &g_Config.aimbot.knockedCheck);
                    
                    ImGui::Separator();
                    ImGui::SliderFloat("FOV", &g_Config.aimbot.fov, 10.0f, 180.0f, "%.0f°");
                    ImGui::SliderFloat("Smoothing", &g_Config.aimbot.smoothing, 0.1f, 10.0f, "%.1f");
                    ImGui::SliderFloat("Max Distance", &g_Config.aimbot.maxDistance, 50.0f, 500.0f, "%.0fm");
                    
                    ImGui::Separator();
                    ImGui::Text("Target Bone:");
                    const char* bones[] = { "Head", "Chest", "Body" };
                    ImGui::Combo("##TargetBone", &g_Config.aimbot.targetBone, bones, IM_ARRAYSIZE(bones));
                }
                
                ImGui::EndTabItem();
            }
            
            // Features Tab
            if (ImGui::BeginTabItem("Features")) {
                ImGui::Spacing();
                
                ImGui::Text("Player Features:");
                Toggle("Wall Hack", &g_Config.wallhack);
                Toggle("Speed Hack", &g_Config.speedHack);
                Toggle("Jump Hack", &g_Config.jumpHack);
                Toggle("Fly Hack", &g_Config.flyHack);
                
                if (g_Config.speedHack) {
                    ImGui::SliderFloat("Speed Multiplier", &g_Config.speedMultiplier, 1.0f, 5.0f, "%.1fx");
                }
                
                if (g_Config.jumpHack) {
                    ImGui::SliderFloat("Jump Height", &g_Config.jumpHeight, 1.0f, 5.0f, "%.1fx");
                }
                
                ImGui::Separator();
                ImGui::Text("Weapon Features:");
                Toggle("No Recoil", &g_Config.noRecoil);
                Toggle("No Spread", &g_Config.noSpread);
                Toggle("Infinite Ammo", &g_Config.infiniteAmmo);
                Toggle("Rapid Fire", &g_Config.rapidFire);
                
                if (g_Config.rapidFire) {
                    ImGui::SliderFloat("Fire Rate Multiplier", &g_Config.fireRateMultiplier, 1.0f, 5.0f, "%.1fx");
                }
                
                ImGui::EndTabItem();
            }
            
            // Info Tab
            if (ImGui::BeginTabItem("Info")) {
                ImGui::Spacing();
                
                ImGui::Text("FreeFire Max Advanced Cheat");
                ImGui::Text("Version: 2.115 [64Bit]");
                ImGui::Text("Target: com.dts.freefiremax");
                ImGui::Separator();
                
                ImGui::Text("Status:");
                ImGui::Text("Game Active: %s", g_IsGameActive ? "Yes" : "No");
                ImGui::Text("Players Found: %zu", g_PlayerList.size());
                ImGui::Text("Local Player: %s", g_LocalPlayer.IsValid() ? "Valid" : "Invalid");
                ImGui::Text("Main Camera: %s", g_MainCamera.IsValid() ? "Valid" : "Invalid");
                
                ImGui::Separator();
                ImGui::Text("Performance:");
                ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
                ImGui::Text("Frame Time: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
                
                ImGui::EndTabItem();
            }
            
            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}

// Hook eglSwapBuffers for rendering
inline EGLBoolean (*old_eglSwapBuffers)(EGLDisplay dpy, EGLSurface surface);
inline EGLBoolean hook_eglSwapBuffers(EGLDisplay dpy, EGLSurface surface) {
    using namespace FFMaxGlobals;
    
    eglQuerySurface(dpy, surface, EGL_WIDTH, &g_GlWidth);
    eglQuerySurface(dpy, surface, EGL_HEIGHT, &g_GlHeight);

    if (!g_IsSetup) {
        prevWidth = g_GlWidth;
        prevHeight = g_GlHeight;
        SetupImgui();
        g_IsSetup = true;
    }

    // Update game data
    UpdateGameData();

    ImGuiIO &io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(g_GlWidth, g_GlHeight);
    ImGui::NewFrame();

    // Handle touch input
    int touchCount = (((int (*)())(Class_Input__get_touchCount))());
    if (touchCount > 0) {
        UnityEngine_Touch_Fields touch = ((UnityEngine_Touch_Fields(*)(int))(Class_Input__GetTouch))(0);
        float reverseY = io.DisplaySize.y - touch.m_Position.fields.y;
        switch (touch.m_Phase) {
            case TouchPhase::Began:
            case TouchPhase::Stationary:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                io.MouseDown[0] = true;
                break;
            case TouchPhase::Ended:
            case TouchPhase::Canceled:
                io.MouseDown[0] = false;
                break;
            case TouchPhase::Moved:
                io.MousePos = ImVec2(touch.m_Position.fields.x, reverseY);
                break;
        }
    }

    // Render ESP
    FFMaxESP::RenderESP();
    
    // Update Aimbot
    FFMaxAimbot::UpdateAimbot();
    
    // Render Aimbot Visuals
    FFMaxAimbot::RenderAimbotVisuals();
    
    // Render GUI
    RenderAdvancedGUI();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    
    return old_eglSwapBuffers(dpy, surface);
}

// Initialize GUI
inline void StartAdvancedGUI() {
    void *ptr_eglSwapBuffer = DobbySymbolResolver("/system/lib/libEGL.so", "eglSwapBuffers");
    if (ptr_eglSwapBuffer != nullptr) {
        DobbyHook((void *)ptr_eglSwapBuffer, (void *)hook_eglSwapBuffers, (void **)&old_eglSwapBuffers);
        LOGD("Advanced FreeFire Max GUI started successfully");
    }
}

// Game initialization and hooks
void InitializeGameHooks() {
    using namespace FFMaxGlobals;
    
    LOGD("Initializing FreeFire Max specific hooks...");
    
    // Initialize cheat configuration with default values
    g_Config.esp.enabled = false;
    g_Config.aimbot.enabled = false;
    
    // Set default ESP colors
    g_Config.esp.colors.enemy = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
    g_Config.esp.colors.team = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
    g_Config.esp.colors.bot = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
    g_Config.esp.colors.knocked = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    g_Config.esp.colors.vehicle = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
    
    // Set default aimbot settings
    g_Config.aimbot.fov = 90.0f;
    g_Config.aimbot.smoothing = 2.0f;
    g_Config.aimbot.maxDistance = 300.0f;
    g_Config.aimbot.targetBone = 0; // Head
    
    g_IsInitialized = true;
    g_IsGameActive = true;
    
    LOGD("FreeFire Max hooks initialized successfully");
}

// Main hack thread
void hack_thread(pid_t pid) {
    using namespace FFMaxGlobals;
    
    LOGD("FreeFire Max Advanced Cheat Thread initiated for PID: %i", pid);

    // Wait for IL2CPP to load
    for (int i = 0; i < 10; i++) {
        il2cpp_base = get_module_base(pid, "libil2cpp.so");
        if (il2cpp_base != 0) break;
        sleep(10);
    }

    if (il2cpp_base == 0) {
        LOGE("libil2cpp.so not found in thread %d", pid);
        return;
    }

    LOGD("IL2CPP base address: 0x%" PRIxPTR, il2cpp_base);
    sleep(5);

    // Initialize IL2CPP
    Il2CppAttach();
    
    // Initialize game-specific hooks
    InitializeGameHooks();
    
    // Start advanced GUI
    StartAdvancedGUI();
    
    LOGD("FreeFire Max Advanced Cheat System fully initialized!");
}

void hack() {
    LOGD("FreeFire Max Advanced Cheat Injection Successful");
    std::thread thread_hack(hack_thread, getpid());
    thread_hack.detach();
}

REGISTER_ZYGISK_MODULE(FFMaxModule)