#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <memory>
#include <cmath>
#include <chrono>
#include <unordered_map>
#include "FFMaxStructs.h"
#include "FFMaxOffsets.h"

namespace FFMaxESP {
    
    using namespace FFMaxStructs;
    
    // Global references
    extern CheatConfig g_Config;
    extern std::vector<Player> g_PlayerList;
    extern Player g_LocalPlayer;
    extern Camera g_MainCamera;
    
    // Advanced ESP Configuration
    struct AdvancedESPConfig {
        // Basic ESP
        bool enabled = true;
        bool showBox = true;
        bool showLine = true;
        bool showHealth = true;
        bool showDistance = true;
        bool showName = true;
        bool showWeapon = true;
        
        // Advanced ESP
        bool showBones = false;
        bool showHeadDot = true;
        bool showLookDirection = true;
        bool showVelocity = false;
        bool showKillCount = false;
        bool showRank = false;
        bool showPing = false;
        
        // Visual Enhancements
        bool smoothBoxes = true;
        bool glowEffect = false;
        bool fadeWithDistance = true;
        bool showThroughWalls = true;
        bool customCrosshair = true;
        bool miniRadar = true;
        
        // Performance
        float maxDistance = 500.0f;
        int maxPlayersToRender = 50;
        bool useOcclusion = true;
        bool adaptiveQuality = true;
        
        // Colors
        struct {
            ImVec4 enemy = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImVec4 team = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            ImVec4 bot = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            ImVec4 knocked = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
            ImVec4 vehicle = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);
            ImVec4 healthGreen = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            ImVec4 healthYellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            ImVec4 healthRed = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
            ImVec4 crosshair = ImVec4(1.0f, 1.0f, 1.0f, 0.8f);
            ImVec4 radar = ImVec4(0.0f, 1.0f, 0.0f, 0.7f);
        } colors;
    };
    
    // Performance tracking
    struct ESPPerformance {
        std::chrono::high_resolution_clock::time_point lastUpdate;
        float renderTime = 0.0f;
        int playersRendered = 0;
        int totalPlayers = 0;
        float avgFPS = 60.0f;
    };
    
    // Player cache for performance
    struct PlayerCache {
        Player player;
        Vector3 worldPos;
        Vector2 screenPos;
        float distance;
        bool isVisible;
        bool shouldRender;
        std::chrono::high_resolution_clock::time_point lastUpdate;
        
        PlayerCache() : player(nullptr), isVisible(false), shouldRender(false) {}
    };
    
    extern AdvancedESPConfig g_ESPConfig;
    extern ESPPerformance g_ESPPerformance;
    extern std::unordered_map<uintptr_t, PlayerCache> g_PlayerCache;
    
    // Utility functions
    inline float GetDistance2D(const Vector2& a, const Vector2& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return sqrt(dx*dx + dy*dy);
    }
    
    inline float GetDistance3D(const Vector3& a, const Vector3& b) {
        return a.Distance(b);
    }
    
    inline ImVec4 GetPlayerColor(const Player& player, const Player& localPlayer) {
        if (player.IsDead()) return g_ESPConfig.colors.knocked;
        if (player.IsKnocked()) return g_ESPConfig.colors.knocked;
        if (player.IsInVehicle()) return g_ESPConfig.colors.vehicle;
        if (player.IsBot()) return g_ESPConfig.colors.bot;
        if (player.IsEnemy(localPlayer)) return g_ESPConfig.colors.enemy;
        return g_ESPConfig.colors.team;
    }
    
    inline ImVec4 GetHealthColor(float healthPercent) {
        if (healthPercent > 0.7f) return g_ESPConfig.colors.healthGreen;
        if (healthPercent > 0.3f) return g_ESPConfig.colors.healthYellow;
        return g_ESPConfig.colors.healthRed;
    }
    
    inline ImVec4 ApplyDistanceFade(const ImVec4& color, float distance, float maxDistance) {
        if (!g_ESPConfig.fadeWithDistance) return color;
        
        float alpha = 1.0f - (distance / maxDistance);
        alpha = std::max(0.1f, std::min(1.0f, alpha));
        
        return ImVec4(color.x, color.y, color.z, color.w * alpha);
    }
    
    // Advanced World to Screen conversion with perspective correction
    Vector2 WorldToScreen(const Vector3& worldPos, const Camera& camera) {
        if (!camera.IsValid()) return Vector2(-1, -1);
        
        // Get camera matrices
        Matrix4x4 viewMatrix = camera.GetViewMatrix();
        Matrix4x4 projMatrix = camera.GetProjectionMatrix();
        
        // Transform to view space
        Vector3 viewPos;
        viewPos.x = worldPos.x * viewMatrix.m[0][0] + worldPos.y * viewMatrix.m[1][0] + worldPos.z * viewMatrix.m[2][0] + viewMatrix.m[3][0];
        viewPos.y = worldPos.x * viewMatrix.m[0][1] + worldPos.y * viewMatrix.m[1][1] + worldPos.z * viewMatrix.m[2][1] + viewMatrix.m[3][1];
        viewPos.z = worldPos.x * viewMatrix.m[0][2] + worldPos.y * viewMatrix.m[1][2] + worldPos.z * viewMatrix.m[2][2] + viewMatrix.m[3][2];
        
        // Check if behind camera
        if (viewPos.z <= 0.1f) return Vector2(-1, -1);
        
        // Transform to clip space
        Vector3 clipPos;
        clipPos.x = viewPos.x * projMatrix.m[0][0] + viewPos.y * projMatrix.m[1][0] + viewPos.z * projMatrix.m[2][0] + projMatrix.m[3][0];
        clipPos.y = viewPos.x * projMatrix.m[0][1] + viewPos.y * projMatrix.m[1][1] + viewPos.z * projMatrix.m[2][1] + projMatrix.m[3][1];
        clipPos.z = viewPos.x * projMatrix.m[0][2] + viewPos.y * projMatrix.m[1][2] + viewPos.z * projMatrix.m[2][2] + projMatrix.m[3][2];
        
        // Perspective divide
        if (clipPos.z != 0.0f) {
            clipPos.x /= clipPos.z;
            clipPos.y /= clipPos.z;
        }
        
        // Convert to screen coordinates
        ImVec2 screenSize = ImGui::GetIO().DisplaySize;
        Vector2 screenPos;
        screenPos.x = (clipPos.x * 0.5f + 0.5f) * screenSize.x;
        screenPos.y = (1.0f - (clipPos.y * 0.5f + 0.5f)) * screenSize.y;
        
        // Validate screen coordinates
        if (screenPos.x < 0 || screenPos.x > screenSize.x || screenPos.y < 0 || screenPos.y > screenSize.y) {
            return Vector2(-1, -1);
        }
        
        return screenPos;
    }
    
    // Advanced visibility check with raycasting
    bool IsPlayerVisible(const Player& player, const Player& localPlayer, const Camera& camera) {
        if (!player.IsValid() || !localPlayer.IsValid() || !camera.IsValid()) return false;
        
        if (!g_ESPConfig.useOcclusion) return true;
        
        Vector3 localPos = localPlayer.GetPosition();
        Vector3 targetPos = player.GetPosition();
        
        // Add height offset for head position
        localPos.y += 1.7f;
        targetPos.y += 1.7f;
        
        // Perform raycast (simplified - in real implementation would use physics system)
        // For now, assume visible if within reasonable distance and angle
        float distance = GetDistance3D(localPos, targetPos);
        if (distance > g_ESPConfig.maxDistance) return false;
        
        // Check if target is within camera frustum
        Vector2 screenPos = WorldToScreen(targetPos, camera);
        return screenPos.x >= 0 && screenPos.y >= 0;
    }
    
    // Update player cache for performance
    void UpdatePlayerCache() {
        if (!g_LocalPlayer.IsValid() || !g_MainCamera.IsValid()) return;
        
        auto now = std::chrono::high_resolution_clock::now();
        g_ESPPerformance.totalPlayers = g_PlayerList.size();
        g_ESPPerformance.playersRendered = 0;
        
        for (const auto& player : g_PlayerList) {
            if (!player.IsValid()) continue;
            
            uintptr_t playerId = (uintptr_t)player.instance;
            auto& cache = g_PlayerCache[playerId];
            
            // Update cache if needed (every 16ms for 60fps)
            auto timeSinceUpdate = std::chrono::duration_cast<std::chrono::milliseconds>(now - cache.lastUpdate);
            if (timeSinceUpdate.count() >= 16 || !cache.player.IsValid()) {
                cache.player = player;
                cache.worldPos = player.GetPosition();
                cache.distance = GetDistance3D(cache.worldPos, g_LocalPlayer.GetPosition());
                cache.isVisible = IsPlayerVisible(player, g_LocalPlayer, g_MainCamera);
                cache.shouldRender = cache.distance <= g_ESPConfig.maxDistance && 
                                   (g_ESPConfig.showThroughWalls || cache.isVisible);
                cache.lastUpdate = now;
                
                if (cache.shouldRender) {
                    cache.screenPos = WorldToScreen(cache.worldPos, g_MainCamera);
                    cache.shouldRender = cache.screenPos.x >= 0 && cache.screenPos.y >= 0;
                }
            }
        }
    }
    
    // Enhanced box drawing with smooth corners
    void DrawSmoothBox(const Vector2& topLeft, const Vector2& bottomRight, const ImVec4& color, float thickness = 1.0f) {
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        
        if (g_ESPConfig.smoothBoxes) {
            // Draw rounded rectangle
            float rounding = 3.0f;
            drawList->AddRect(ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), 
                            ImGui::ColorConvertFloat4ToU32(color), rounding, 0, thickness);
        } else {
            // Draw regular rectangle
            drawList->AddRect(ImVec2(topLeft.x, topLeft.y), ImVec2(bottomRight.x, bottomRight.y), 
                            ImGui::ColorConvertFloat4ToU32(color), 0.0f, 0, thickness);
        }
        
        // Add glow effect if enabled
        if (g_ESPConfig.glowEffect) {
            ImVec4 glowColor = ImVec4(color.x, color.y, color.z, color.w * 0.3f);
            drawList->AddRect(ImVec2(topLeft.x - 1, topLeft.y - 1), ImVec2(bottomRight.x + 1, bottomRight.y + 1), 
                            ImGui::ColorConvertFloat4ToU32(glowColor), g_ESPConfig.smoothBoxes ? 4.0f : 0.0f, 0, thickness + 1.0f);
        }
    }
    
    // Enhanced health bar with gradient
    void DrawHealthBar(const Vector2& position, float health, float maxHealth, const ImVec2& size) {
        if (health <= 0 || maxHealth <= 0) return;
        
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        float healthPercent = health / maxHealth;
        
        // Background
        ImVec4 bgColor = ImVec4(0.2f, 0.2f, 0.2f, 0.8f);
        drawList->AddRectFilled(ImVec2(position.x, position.y), 
                              ImVec2(position.x + size.x, position.y + size.y), 
                              ImGui::ColorConvertFloat4ToU32(bgColor));
        
        // Health bar with gradient
        ImVec4 healthColor = GetHealthColor(healthPercent);
        float healthWidth = size.x * healthPercent;
        
        if (healthWidth > 0) {
            // Create gradient effect
            ImVec4 healthColorDark = ImVec4(healthColor.x * 0.7f, healthColor.y * 0.7f, healthColor.z * 0.7f, healthColor.w);
            
            drawList->AddRectFilledMultiColor(
                ImVec2(position.x, position.y),
                ImVec2(position.x + healthWidth, position.y + size.y),
                ImGui::ColorConvertFloat4ToU32(healthColor),
                ImGui::ColorConvertFloat4ToU32(healthColorDark),
                ImGui::ColorConvertFloat4ToU32(healthColorDark),
                ImGui::ColorConvertFloat4ToU32(healthColor)
            );
        }
        
        // Border
        ImVec4 borderColor = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        drawList->AddRect(ImVec2(position.x, position.y), 
                        ImVec2(position.x + size.x, position.y + size.y), 
                        ImGui::ColorConvertFloat4ToU32(borderColor));
        
        // Health text
        if (size.x > 30) {
            char healthText[32];
            snprintf(healthText, sizeof(healthText), "%.0f", health);
            
            ImVec2 textSize = ImGui::CalcTextSize(healthText);
            ImVec2 textPos = ImVec2(position.x + (size.x - textSize.x) * 0.5f, 
                                  position.y + (size.y - textSize.y) * 0.5f);
            
            drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), 
                            ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), healthText);
            drawList->AddText(textPos, 
                            ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1)), healthText);
        }
    }
    
    // Draw bone ESP with realistic skeleton
    void DrawBoneESP(const Player& player, const ImVec4& color) {
        // Simplified bone structure - in real implementation would get actual bone positions
        Vector3 basePos = player.GetPosition();
        
        // Define bone connections
        struct BoneConnection {
            Vector3 from, to;
        };
        
        std::vector<BoneConnection> bones = {
            // Head to neck
            {Vector3(0, 1.8f, 0), Vector3(0, 1.6f, 0)},
            // Spine
            {Vector3(0, 1.6f, 0), Vector3(0, 1.2f, 0)},
            {Vector3(0, 1.2f, 0), Vector3(0, 0.9f, 0)},
            // Arms
            {Vector3(0, 1.5f, 0), Vector3(-0.3f, 1.2f, 0)},
            {Vector3(-0.3f, 1.2f, 0), Vector3(-0.5f, 0.9f, 0)},
            {Vector3(0, 1.5f, 0), Vector3(0.3f, 1.2f, 0)},
            {Vector3(0.3f, 1.2f, 0), Vector3(0.5f, 0.9f, 0)},
            // Legs
            {Vector3(0, 0.9f, 0), Vector3(-0.15f, 0.5f, 0)},
            {Vector3(-0.15f, 0.5f, 0), Vector3(-0.1f, 0.0f, 0)},
            {Vector3(0, 0.9f, 0), Vector3(0.15f, 0.5f, 0)},
            {Vector3(0.15f, 0.5f, 0), Vector3(0.1f, 0.0f, 0)},
        };
        
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        
        for (const auto& bone : bones) {
            Vector3 fromWorld = basePos + bone.from;
            Vector3 toWorld = basePos + bone.to;
            
            Vector2 fromScreen = WorldToScreen(fromWorld, g_MainCamera);
            Vector2 toScreen = WorldToScreen(toWorld, g_MainCamera);
            
            if (fromScreen.x >= 0 && fromScreen.y >= 0 && toScreen.x >= 0 && toScreen.y >= 0) {
                drawList->AddLine(ImVec2(fromScreen.x, fromScreen.y), 
                                ImVec2(toScreen.x, toScreen.y), 
                                ImGui::ColorConvertFloat4ToU32(color), 2.0f);
            }
        }
    }
    
    // Draw velocity indicator
    void DrawVelocityIndicator(const Player& player, const Vector2& screenPos, const ImVec4& color) {
        Vector3 velocity = player.GetVelocity();
        float speed = velocity.Magnitude();
        
        if (speed < 0.1f) return; // Not moving
        
        // Calculate direction
        Vector3 futurePos = player.GetPosition() + velocity * 2.0f; // 2 seconds ahead
        Vector2 futureScreen = WorldToScreen(futurePos, g_MainCamera);
        
        if (futureScreen.x >= 0 && futureScreen.y >= 0) {
            ImDrawList* drawList = ImGui::GetBackgroundDrawList();
            
            // Draw velocity line
            drawList->AddLine(ImVec2(screenPos.x, screenPos.y), 
                            ImVec2(futureScreen.x, futureScreen.y), 
                            ImGui::ColorConvertFloat4ToU32(color), 2.0f);
            
            // Draw arrowhead
            Vector2 direction = Vector2(futureScreen.x - screenPos.x, futureScreen.y - screenPos.y);
            float length = sqrt(direction.x * direction.x + direction.y * direction.y);
            if (length > 0) {
                direction.x /= length;
                direction.y /= length;
                
                Vector2 arrowTip = futureScreen;
                Vector2 arrowLeft = Vector2(arrowTip.x - direction.x * 10 - direction.y * 5, 
                                          arrowTip.y - direction.y * 10 + direction.x * 5);
                Vector2 arrowRight = Vector2(arrowTip.x - direction.x * 10 + direction.y * 5, 
                                           arrowTip.y - direction.y * 10 - direction.x * 5);
                
                drawList->AddTriangleFilled(ImVec2(arrowTip.x, arrowTip.y),
                                          ImVec2(arrowLeft.x, arrowLeft.y),
                                          ImVec2(arrowRight.x, arrowRight.y),
                                          ImGui::ColorConvertFloat4ToU32(color));
            }
        }
    }
    
    // Draw custom crosshair
    void DrawCustomCrosshair() {
        if (!g_ESPConfig.customCrosshair) return;
        
        ImVec2 screenCenter = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        
        float size = 10.0f;
        float thickness = 2.0f;
        ImU32 color = ImGui::ColorConvertFloat4ToU32(g_ESPConfig.colors.crosshair);
        
        // Draw crosshair lines
        drawList->AddLine(ImVec2(screenCenter.x - size, screenCenter.y), 
                        ImVec2(screenCenter.x + size, screenCenter.y), color, thickness);
        drawList->AddLine(ImVec2(screenCenter.x, screenCenter.y - size), 
                        ImVec2(screenCenter.x, screenCenter.y + size), color, thickness);
        
        // Draw center dot
        drawList->AddCircleFilled(screenCenter, 2.0f, color);
    }
    
    // Draw mini radar
    void DrawMiniRadar() {
        if (!g_ESPConfig.miniRadar || !g_LocalPlayer.IsValid()) return;
        
        ImVec2 radarPos = ImVec2(ImGui::GetIO().DisplaySize.x - 150, 50);
        ImVec2 radarSize = ImVec2(120, 120);
        
        ImDrawList* drawList = ImGui::GetBackgroundDrawList();
        
        // Radar background
        drawList->AddRectFilled(radarPos, ImVec2(radarPos.x + radarSize.x, radarPos.y + radarSize.y), 
                              ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 0.7f)));
        drawList->AddRect(radarPos, ImVec2(radarPos.x + radarSize.x, radarPos.y + radarSize.y), 
                        ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 0.5f)));
        
        Vector3 localPos = g_LocalPlayer.GetPosition();
        ImVec2 radarCenter = ImVec2(radarPos.x + radarSize.x * 0.5f, radarPos.y + radarSize.y * 0.5f);
        
        // Draw center (local player)
        drawList->AddCircleFilled(radarCenter, 3.0f, ImGui::ColorConvertFloat4ToU32(ImVec4(0, 1, 0, 1)));
        
        // Draw other players
        for (const auto& [playerId, cache] : g_PlayerCache) {
            if (!cache.shouldRender || cache.distance > 200.0f) continue;
            
            Vector3 relativePos = cache.worldPos - localPos;
            float radarX = radarCenter.x + (relativePos.x / 200.0f) * (radarSize.x * 0.4f);
            float radarY = radarCenter.y - (relativePos.z / 200.0f) * (radarSize.y * 0.4f);
            
            if (radarX >= radarPos.x && radarX <= radarPos.x + radarSize.x &&
                radarY >= radarPos.y && radarY <= radarPos.y + radarSize.y) {
                
                ImVec4 playerColor = GetPlayerColor(cache.player, g_LocalPlayer);
                drawList->AddCircleFilled(ImVec2(radarX, radarY), 2.0f, 
                                        ImGui::ColorConvertFloat4ToU32(playerColor));
            }
        }
        
        // Radar title
        drawList->AddText(ImVec2(radarPos.x, radarPos.y - 20), 
                        ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1)), "Radar");
    }
    
    // Main ESP rendering function
    void RenderESP() {
        if (!g_ESPConfig.enabled || !g_LocalPlayer.IsValid() || !g_MainCamera.IsValid()) return;
        
        auto startTime = std::chrono::high_resolution_clock::now();
        
        // Update player cache
        UpdatePlayerCache();
        
        // Draw custom crosshair
        DrawCustomCrosshair();
        
        // Draw mini radar
        DrawMiniRadar();
        
        // Render ESP for each player
        for (const auto& [playerId, cache] : g_PlayerCache) {
            if (!cache.shouldRender) continue;
            
            Vector2 screenPos = cache.screenPos;
            ImVec4 playerColor = GetPlayerColor(cache.player, g_LocalPlayer);
            playerColor = ApplyDistanceFade(playerColor, cache.distance, g_ESPConfig.maxDistance);
            
            // Calculate box dimensions
            float boxHeight = 60.0f / (cache.distance / 100.0f + 1.0f);
            float boxWidth = boxHeight * 0.6f;
            Vector2 boxTop = Vector2(screenPos.x - boxWidth * 0.5f, screenPos.y - boxHeight);
            Vector2 boxBottom = Vector2(screenPos.x + boxWidth * 0.5f, screenPos.y);
            
            // Draw box ESP
            if (g_ESPConfig.showBox) {
                DrawSmoothBox(boxTop, boxBottom, playerColor);
            }
            
            // Draw line ESP
            if (g_ESPConfig.showLine) {
                ImVec2 screenCenter = ImVec2(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y);
                ImDrawList* drawList = ImGui::GetBackgroundDrawList();
                drawList->AddLine(screenCenter, ImVec2(screenPos.x, screenPos.y), 
                                ImGui::ColorConvertFloat4ToU32(playerColor));
            }
            
            // Draw health bar
            if (g_ESPConfig.showHealth) {
                float health = cache.player.GetHealth();
                float maxHealth = cache.player.GetMaxHealth();
                Vector2 healthBarPos = Vector2(boxTop.x - 8, boxTop.y);
                DrawHealthBar(healthBarPos, health, maxHealth, ImVec2(4, boxHeight));
            }
            
            // Draw head dot
            if (g_ESPConfig.showHeadDot) {
                Vector3 headPos = cache.worldPos + Vector3(0, 1.8f, 0);
                Vector2 headScreen = WorldToScreen(headPos, g_MainCamera);
                if (headScreen.x >= 0 && headScreen.y >= 0) {
                    ImDrawList* drawList = ImGui::GetBackgroundDrawList();
                    drawList->AddCircleFilled(ImVec2(headScreen.x, headScreen.y), 3.0f, 
                                            ImGui::ColorConvertFloat4ToU32(playerColor));
                }
            }
            
            // Draw bones
            if (g_ESPConfig.showBones) {
                DrawBoneESP(cache.player, playerColor);
            }
            
            // Draw velocity indicator
            if (g_ESPConfig.showVelocity) {
                DrawVelocityIndicator(cache.player, screenPos, playerColor);
            }
            
            // Draw text information
            ImDrawList* drawList = ImGui::GetBackgroundDrawList();
            float textY = boxTop.y - 5;
            
            // Distance
            if (g_ESPConfig.showDistance) {
                char distText[32];
                snprintf(distText, sizeof(distText), "%.0fm", cache.distance);
                ImVec2 textSize = ImGui::CalcTextSize(distText);
                ImVec2 textPos = ImVec2(screenPos.x - textSize.x * 0.5f, textY);
                
                drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), 
                                ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), distText);
                drawList->AddText(textPos, 
                                ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, 1)), distText);
                textY -= 15;
            }
            
            // Player name
            if (g_ESPConfig.showName) {
                const char* playerName = cache.player.GetName();
                if (playerName && strlen(playerName) > 0) {
                    ImVec2 textSize = ImGui::CalcTextSize(playerName);
                    ImVec2 textPos = ImVec2(screenPos.x - textSize.x * 0.5f, textY);
                    
                    drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), 
                                    ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), playerName);
                    drawList->AddText(textPos, 
                                    ImGui::ColorConvertFloat4ToU32(playerColor), playerName);
                    textY -= 15;
                }
            }
            
            // Weapon information
            if (g_ESPConfig.showWeapon) {
                const char* weaponName = cache.player.GetCurrentWeaponName();
                if (weaponName && strlen(weaponName) > 0) {
                    ImVec2 textSize = ImGui::CalcTextSize(weaponName);
                    ImVec2 textPos = ImVec2(screenPos.x - textSize.x * 0.5f, boxBottom.y + 5);
                    
                    drawList->AddText(ImVec2(textPos.x + 1, textPos.y + 1), 
                                    ImGui::ColorConvertFloat4ToU32(ImVec4(0, 0, 0, 1)), weaponName);
                    drawList->AddText(textPos, 
                                    ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 0, 1)), weaponName);
                }
            }
            
            g_ESPPerformance.playersRendered++;
            
            // Limit rendering for performance
            if (g_ESPPerformance.playersRendered >= g_ESPConfig.maxPlayersToRender) break;
        }
        
        // Update performance metrics
        auto endTime = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime);
        g_ESPPerformance.renderTime = duration.count() / 1000.0f; // Convert to milliseconds
        g_ESPPerformance.lastUpdate = endTime;
    }
    
    // Initialize ESP system
    void Initialize() {
        g_ESPConfig = AdvancedESPConfig();
        g_ESPPerformance = ESPPerformance();
        g_PlayerCache.clear();
    }
    
    // Cleanup ESP system
    void Shutdown() {
        g_PlayerCache.clear();
    }
}