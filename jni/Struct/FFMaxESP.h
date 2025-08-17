#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <memory>
#include <cmath>
#include "FFMaxStructs.h"
#include "FFMaxOffsets.h"

namespace FFMaxESP {
    
    using namespace FFMaxStructs;
    
    // Global configuration
    extern CheatConfig g_Config;
    extern std::vector<Player> g_PlayerList;
    extern Player g_LocalPlayer;
    extern Camera g_MainCamera;
    
    // Utility functions
    inline float GetDistance2D(const Vector2& a, const Vector2& b) {
        float dx = a.x - b.x;
        float dy = a.y - b.y;
        return sqrt(dx*dx + dy*dy);
    }
    
    inline ImVec4 GetPlayerColor(const Player& player, const Player& localPlayer) {
        if (player.IsDead()) return g_Config.esp.colors.knocked;
        if (player.IsKnocked()) return g_Config.esp.colors.knocked;
        if (player.IsInVehicle()) return g_Config.esp.colors.vehicle;
        if (player.IsBot()) return g_Config.esp.colors.bot;
        if (player.IsEnemy(localPlayer)) return g_Config.esp.colors.enemy;
        return g_Config.esp.colors.team;
    }
    
    inline ImVec4 GetHealthColor(float healthPercent) {
        if (healthPercent > 0.7f) return g_Config.esp.colors.healthGreen;
        if (healthPercent > 0.3f) return g_Config.esp.colors.healthYellow;
        return g_Config.esp.colors.healthRed;
    }
    
    // World to Screen conversion
    Vector2 WorldToScreen(const Vector3& worldPos, const Camera& camera) {
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
    
    // Enhanced ESP Drawing Functions
    void DrawESPBox(const Vector2& screenPos, const Vector2& size, const ImVec4& color, float thickness = 2.0f) {
        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(color);
        
        float x = screenPos.x - size.x * 0.5f;
        float y = screenPos.y - size.y;
        float w = size.x;
        float h = size.y;
        
        // Draw box outline
        drawList->AddRect(ImVec2(x, y), ImVec2(x + w, y + h), col, 0.0f, 0, thickness);
        
        // Draw corner lines for better visibility
        float cornerSize = std::min(w, h) * 0.15f;
        
        // Top-left corner
        drawList->AddLine(ImVec2(x, y), ImVec2(x + cornerSize, y), col, thickness + 1);
        drawList->AddLine(ImVec2(x, y), ImVec2(x, y + cornerSize), col, thickness + 1);
        
        // Top-right corner
        drawList->AddLine(ImVec2(x + w, y), ImVec2(x + w - cornerSize, y), col, thickness + 1);
        drawList->AddLine(ImVec2(x + w, y), ImVec2(x + w, y + cornerSize), col, thickness + 1);
        
        // Bottom-left corner
        drawList->AddLine(ImVec2(x, y + h), ImVec2(x + cornerSize, y + h), col, thickness + 1);
        drawList->AddLine(ImVec2(x, y + h), ImVec2(x, y + h - cornerSize), col, thickness + 1);
        
        // Bottom-right corner
        drawList->AddLine(ImVec2(x + w, y + h), ImVec2(x + w - cornerSize, y + h), col, thickness + 1);
        drawList->AddLine(ImVec2(x + w, y + h), ImVec2(x + w, y + h - cornerSize), col, thickness + 1);
    }
    
    void DrawESPLine(const Vector2& from, const Vector2& to, const ImVec4& color, float thickness = 2.0f) {
        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        ImU32 col = ImGui::ColorConvertFloat4ToU32(color);
        drawList->AddLine(ImVec2(from.x, from.y), ImVec2(to.x, to.y), col, thickness);
    }
    
    void DrawHealthBar(const Vector2& screenPos, const Vector2& size, float health, float maxHealth, bool horizontal = false) {
        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        
        float healthPercent = health / maxHealth;
        healthPercent = std::clamp(healthPercent, 0.0f, 1.0f);
        
        ImVec4 healthColor = GetHealthColor(healthPercent);
        ImU32 healthCol = ImGui::ColorConvertFloat4ToU32(healthColor);
        ImU32 bgCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 0.8f));
        ImU32 borderCol = ImGui::ColorConvertFloat4ToU32(ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        
        if (horizontal) {
            // Horizontal health bar (above player)
            float barWidth = size.x;
            float barHeight = 8.0f;
            float x = screenPos.x - barWidth * 0.5f;
            float y = screenPos.y - size.y - barHeight - 5.0f;
            
            // Background
            drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + barWidth, y + barHeight), bgCol);
            // Health
            drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + barWidth * healthPercent, y + barHeight), healthCol);
            // Border
            drawList->AddRect(ImVec2(x, y), ImVec2(x + barWidth, y + barHeight), borderCol, 0.0f, 0, 1.0f);
        } else {
            // Vertical health bar (left side of box)
            float barWidth = 6.0f;
            float barHeight = size.y;
            float x = screenPos.x - size.x * 0.5f - barWidth - 3.0f;
            float y = screenPos.y - barHeight;
            
            // Background
            drawList->AddRectFilled(ImVec2(x, y), ImVec2(x + barWidth, y + barHeight), bgCol);
            // Health
            float healthBarHeight = barHeight * healthPercent;
            drawList->AddRectFilled(ImVec2(x, y + barHeight - healthBarHeight), ImVec2(x + barWidth, y + barHeight), healthCol);
            // Border
            drawList->AddRect(ImVec2(x, y), ImVec2(x + barWidth, y + barHeight), borderCol, 0.0f, 0, 1.0f);
        }
    }
    
    void DrawESPText(const Vector2& pos, const char* text, const ImVec4& color, float fontSize = 14.0f, bool centered = true, bool outlined = true) {
        ImDrawList* drawList = ImGui::GetForegroundDrawList();
        ImU32 textCol = ImGui::ColorConvertFloat4ToU32(color);
        ImU32 outlineCol = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, 1.0f));
        
        ImVec2 textSize = ImGui::CalcTextSize(text);
        ImVec2 textPos = ImVec2(pos.x, pos.y);
        
        if (centered) {
            textPos.x -= textSize.x * 0.5f;
        }
        
        if (outlined) {
            // Draw outline
            for (int x = -1; x <= 1; x++) {
                for (int y = -1; y <= 1; y++) {
                    if (x == 0 && y == 0) continue;
                    drawList->AddText(nullptr, fontSize, ImVec2(textPos.x + x, textPos.y + y), outlineCol, text);
                }
            }
        }
        
        // Draw main text
        drawList->AddText(nullptr, fontSize, textPos, textCol, text);
    }
    
    void DrawDistanceText(const Vector2& screenPos, const Vector2& size, float distance) {
        char distanceText[32];
        snprintf(distanceText, sizeof(distanceText), "%.0fm", distance);
        
        Vector2 textPos = Vector2(screenPos.x, screenPos.y + 5.0f);
        DrawESPText(textPos, distanceText, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), 12.0f);
    }
    
    void DrawPlayerInfo(const Player& player, const Vector2& screenPos, const Vector2& size) {
        float yOffset = 5.0f;
        
        // Player status
        if (player.IsBot()) {
            DrawESPText(Vector2(screenPos.x, screenPos.y + size.y + yOffset), "[BOT]", g_Config.esp.colors.bot, 12.0f);
            yOffset += 15.0f;
        }
        
        if (player.IsKnocked()) {
            DrawESPText(Vector2(screenPos.x, screenPos.y + size.y + yOffset), "[KNOCKED]", g_Config.esp.colors.knocked, 12.0f);
            yOffset += 15.0f;
        }
        
        if (player.IsInVehicle()) {
            DrawESPText(Vector2(screenPos.x, screenPos.y + size.y + yOffset), "[VEHICLE]", g_Config.esp.colors.vehicle, 12.0f);
            yOffset += 15.0f;
        }
    }
    
    // Main ESP rendering function
    void RenderESP() {
        if (!g_Config.esp.enabled) return;
        if (!g_MainCamera.IsValid()) return;
        if (!g_LocalPlayer.IsValid()) return;
        
        ImGuiIO& io = ImGui::GetIO();
        Vector2 screenCenter = Vector2(io.DisplaySize.x * 0.5f, io.DisplaySize.y * 0.5f);
        
        for (const Player& player : g_PlayerList) {
            if (!player.IsValid()) continue;
            if (player.instance == g_LocalPlayer.instance) continue; // Skip local player
            
            // Distance check
            float distance = player.GetDistance(g_LocalPlayer);
            if (distance > g_Config.esp.maxDistance) continue;
            
            // Filter checks
            if (!g_Config.esp.showBots && player.IsBot()) continue;
            if (!g_Config.esp.showKnocked && player.IsKnocked()) continue;
            if (!g_Config.esp.showInVehicle && player.IsInVehicle()) continue;
            if (player.IsDead()) continue;
            
            // Get world position
            Vector3 worldPos = player.GetPosition();
            Vector3 headPos = worldPos + Vector3(0, 1.8f, 0); // Approximate head height
            
            // World to screen conversion
            Vector2 screenPos = WorldToScreen(worldPos, g_MainCamera);
            Vector2 headScreenPos = WorldToScreen(headPos, g_MainCamera);
            
            if (screenPos.x < 0 || screenPos.y < 0 || headScreenPos.x < 0 || headScreenPos.y < 0) continue;
            if (screenPos.x > io.DisplaySize.x || screenPos.y > io.DisplaySize.y) continue;
            
            // Calculate box size based on distance
            float boxHeight = abs(screenPos.y - headScreenPos.y) * 2.2f;
            float boxWidth = boxHeight * 0.6f;
            Vector2 boxSize = Vector2(boxWidth, boxHeight);
            
            // Get player color
            ImVec4 playerColor = GetPlayerColor(player, g_LocalPlayer);
            
            // Draw ESP elements
            if (g_Config.esp.showBox) {
                DrawESPBox(screenPos, boxSize, playerColor);
            }
            
            if (g_Config.esp.showLine) {
                DrawESPLine(screenCenter, screenPos, g_Config.esp.colors.line);
            }
            
            if (g_Config.esp.showHealth) {
                float health = player.GetHealth();
                float maxHealth = player.GetMaxHealth();
                if (health > 0 && maxHealth > 0) {
                    DrawHealthBar(screenPos, boxSize, health, maxHealth, false);
                }
            }
            
            if (g_Config.esp.showDistance) {
                DrawDistanceText(screenPos, boxSize, distance);
            }
            
            // Draw player info
            DrawPlayerInfo(player, screenPos, boxSize);
        }
    }
    
    // Player list management
    void UpdatePlayerList();
    void InitializeESP();
    void CleanupESP();
}