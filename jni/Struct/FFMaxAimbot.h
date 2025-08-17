#pragma once

#include <imgui.h>
#include <vector>
#include <algorithm>
#include <cmath>
#include "FFMaxStructs.h"
#include "FFMaxOffsets.h"

namespace FFMaxAimbot {
    
    using namespace FFMaxStructs;
    
    // Global references
    extern CheatConfig g_Config;
    extern std::vector<Player> g_PlayerList;
    extern Player g_LocalPlayer;
    extern Camera g_MainCamera;
    
    // Bone positions for different target types
    struct BoneOffsets {
        static constexpr Vector3 HEAD = Vector3(0.0f, 1.8f, 0.0f);
        static constexpr Vector3 CHEST = Vector3(0.0f, 1.2f, 0.0f);
        static constexpr Vector3 BODY = Vector3(0.0f, 0.9f, 0.0f);
    };
    
    // Target information
    struct AimTarget {
        Player player;
        Vector3 targetPos;
        Vector2 screenPos;
        float distance;
        float fovDistance;
        bool isValid;
        
        AimTarget() : player(nullptr), isValid(false) {}
        AimTarget(const Player& p, const Vector3& pos, const Vector2& screen, float dist, float fov) 
            : player(p), targetPos(pos), screenPos(screen), distance(dist), fovDistance(fov), isValid(true) {}
    };
    
    // Utility functions
    inline float GetFOVDistance(const Vector2& screenPos, const Vector2& screenCenter) {
        float dx = screenPos.x - screenCenter.x;
        float dy = screenPos.y - screenCenter.y;
        return sqrt(dx*dx + dy*dy);
    }
    
    inline Vector3 GetTargetBonePosition(const Player& player, int boneType) {
        Vector3 basePos = player.GetPosition();
        
        switch (boneType) {
            case AimbotConfig::HEAD:
                return basePos + BoneOffsets::HEAD;
            case AimbotConfig::CHEST:
                return basePos + BoneOffsets::CHEST;
            case AimbotConfig::BODY:
                return basePos + BoneOffsets::BODY;
            default:
                return basePos + BoneOffsets::HEAD;
        }
    }
    
    inline bool IsTargetValid(const Player& target, const Player& localPlayer) {
        if (!target.IsValid() || !localPlayer.IsValid()) return false;
        if (target.instance == localPlayer.instance) return false;
        if (target.IsDead()) return false;
        
        // Team check
        if (g_Config.aimbot.teamCheck && !target.IsEnemy(localPlayer)) return false;
        
        // Bot check
        if (!g_Config.aimbot.botCheck && target.IsBot()) return false;
        
        // Knocked check
        if (!g_Config.aimbot.knockedCheck && target.IsKnocked()) return false;
        
        // Distance check
        float distance = target.GetDistance(localPlayer);
        if (distance > g_Config.aimbot.maxDistance) return false;
        
        return true;
    }
    
    inline bool IsTargetVisible(const Player& target, const Player& localPlayer) {
        if (!g_Config.aimbot.visibilityCheck) return true;
        
        // This would require ray-casting implementation
        // For now, we'll use the player's visibility flag
        return target.IsVisible();
    }
    
    // World to screen conversion for aimbot
    Vector2 WorldToScreenAim(const Vector3& worldPos, const Camera& camera) {
        if (!camera.IsValid()) return Vector2(-1, -1);
        
        Matrix4x4 viewMatrix = camera.GetViewMatrix();
        Matrix4x4 projMatrix = camera.GetProjectionMatrix();
        
        // Transform world position to clip space
        float x = worldPos.x * viewMatrix.m[0][0] + worldPos.y * viewMatrix.m[0][1] + worldPos.z * viewMatrix.m[0][2] + viewMatrix.m[0][3];
        float y = worldPos.x * viewMatrix.m[1][0] + worldPos.y * viewMatrix.m[1][1] + worldPos.z * viewMatrix.m[1][2] + viewMatrix.m[1][3];
        float z = worldPos.x * viewMatrix.m[2][0] + worldPos.y * viewMatrix.m[2][1] + worldPos.z * viewMatrix.m[2][2] + viewMatrix.m[2][3];
        float w = worldPos.x * viewMatrix.m[3][0] + worldPos.y * viewMatrix.m[3][1] + worldPos.z * viewMatrix.m[3][2] + viewMatrix.m[3][3];
        
        if (w < 0.1f) return Vector2(-1, -1);
        
        // Project to screen space
        float screenX = (x / w) * projMatrix.m[0][0];
        float screenY = (y / w) * projMatrix.m[1][1];
        
        // Convert to screen coordinates
        ImGuiIO& io = ImGui::GetIO();
        screenX = (screenX + 1.0f) * 0.5f * io.DisplaySize.x;
        screenY = (1.0f - screenY) * 0.5f * io.DisplaySize.y;
        
        return Vector2(screenX, screenY);
    }
    
    // Find the best target
    AimTarget FindBestTarget() {
        if (!g_Config.aimbot.enabled) return AimTarget();
        if (!g_MainCamera.IsValid()) return AimTarget();
        if (!g_LocalPlayer.IsValid()) return AimTarget();
        
        ImGuiIO& io = ImGui::GetIO();
        Vector2 screenCenter = Vector2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
        
        std::vector<AimTarget> validTargets;
        
        // Find all valid targets
        for (const Player& player : g_PlayerList) {
            if (!IsTargetValid(player, g_LocalPlayer)) continue;
            if (!IsTargetVisible(player, g_LocalPlayer)) continue;
            
            // Get target bone position
            Vector3 targetPos = GetTargetBonePosition(player, g_Config.aimbot.targetBone);
            Vector2 screenPos = WorldToScreenAim(targetPos, g_MainCamera);
            
            if (screenPos.x < 0 || screenPos.y < 0) continue;
            if (screenPos.x > io.DisplaySize.x || screenPos.y > io.DisplaySize.y) continue;
            
            // Calculate FOV distance
            float fovDistance = GetFOVDistance(screenPos, screenCenter);
            float maxFovDistance = (g_Config.aimbot.fov / 90.0f) * (io.DisplaySize.x * 0.5f);
            
            if (fovDistance > maxFovDistance) continue;
            
            float distance = player.GetDistance(g_LocalPlayer);
            validTargets.emplace_back(player, targetPos, screenPos, distance, fovDistance);
        }
        
        if (validTargets.empty()) return AimTarget();
        
        // Sort by FOV distance (closest to crosshair first)
        std::sort(validTargets.begin(), validTargets.end(), 
            [](const AimTarget& a, const AimTarget& b) {
                return a.fovDistance < b.fovDistance;
            });
        
        return validTargets[0];
    }
    
    // Smooth aim calculation
    Vector2 CalculateSmoothAim(const Vector2& currentPos, const Vector2& targetPos, float smoothing) {
        if (smoothing <= 0.0f) return targetPos;
        
        float deltaX = targetPos.x - currentPos.x;
        float deltaY = targetPos.y - currentPos.y;
        
        float smoothFactor = 1.0f / smoothing;
        
        return Vector2(
            currentPos.x + deltaX * smoothFactor,
            currentPos.y + deltaY * smoothFactor
        );
    }
    
    // Main aimbot update function
    void UpdateAimbot() {
        if (!g_Config.aimbot.enabled) return;
        
        // Check aim key if enabled
        if (g_Config.aimbot.aimKey) {
            // This would require input handling implementation
            // For now, we'll assume the key is always pressed when aimKey is enabled
            // You would need to implement proper key checking here
        }
        
        AimTarget target = FindBestTarget();
        if (!target.isValid) return;
        
        ImGuiIO& io = ImGui::GetIO();
        Vector2 screenCenter = Vector2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
        
        // Calculate smooth aim
        Vector2 aimPos = CalculateSmoothAim(screenCenter, target.screenPos, g_Config.aimbot.smoothing);
        
        // Apply aim (this would require mouse input injection)
        // For demonstration purposes, we'll just calculate the deltas
        float deltaX = aimPos.x - screenCenter.x;
        float deltaY = aimPos.y - screenCenter.y;
        
        // Convert screen delta to mouse movement
        // This would need to be implemented based on the game's sensitivity settings
        // ApplyMouseMovement(deltaX, deltaY);
        
        // Auto shoot if enabled and target is close to crosshair
        if (g_Config.aimbot.autoShoot) {
            float distanceToTarget = GetFOVDistance(target.screenPos, screenCenter);
            if (distanceToTarget < 10.0f) { // Within 10 pixels of crosshair
                // Trigger auto shoot
                // This would require input injection implementation
                // TriggerShoot();
            }
        }
    }
    
    // Draw aimbot FOV circle
    void DrawAimbotFOV() {
        if (!g_Config.aimbot.enabled) return;
        
        ImGuiIO& io = ImGui::GetIO();
        Vector2 screenCenter = Vector2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
        
        float fovRadius = (g_Config.aimbot.fov / 90.0f) * (io.DisplaySize.x * 0.5f);
        
        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        ImU32 fovColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 0.3f));
        
        drawList->AddCircle(ImVec2(screenCenter.x, screenCenter.y), fovRadius, fovColor, 64, 2.0f);
    }
    
    // Draw target indicator
    void DrawTargetIndicator() {
        if (!g_Config.aimbot.enabled) return;
        
        AimTarget target = FindBestTarget();
        if (!target.isValid) return;
        
        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        ImU32 targetColor = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 0.0f, 0.0f, 0.8f));
        
        // Draw crosshair on target
        float crosshairSize = 10.0f;
        drawList->AddLine(
            ImVec2(target.screenPos.x - crosshairSize, target.screenPos.y),
            ImVec2(target.screenPos.x + crosshairSize, target.screenPos.y),
            targetColor, 3.0f
        );
        drawList->AddLine(
            ImVec2(target.screenPos.x, target.screenPos.y - crosshairSize),
            ImVec2(target.screenPos.x, target.screenPos.y + crosshairSize),
            targetColor, 3.0f
        );
        
        // Draw circle around target
        drawList->AddCircle(ImVec2(target.screenPos.x, target.screenPos.y), 20.0f, targetColor, 32, 2.0f);
    }
    
    // Render aimbot visuals
    void RenderAimbotVisuals() {
        DrawAimbotFOV();
        DrawTargetIndicator();
    }
    
    // Initialize aimbot
    void InitializeAimbot();
    
    // Cleanup aimbot
    void CleanupAimbot();
    
    // Input handling functions (to be implemented)
    void ApplyMouseMovement(float deltaX, float deltaY);
    void TriggerShoot();
    bool IsAimKeyPressed();
}