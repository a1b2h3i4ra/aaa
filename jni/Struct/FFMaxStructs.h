#pragma once

#include <Vector3.hpp>
#include <Color.h>
#include "FFMaxOffsets.h"

// FreeFire Max Specific Structures
namespace FFMaxStructs {
    
    struct Vector3 {
        float x, y, z;
        
        Vector3() : x(0), y(0), z(0) {}
        Vector3(float x, float y, float z) : x(x), y(y), z(z) {}
        
        Vector3 operator+(const Vector3& other) const {
            return Vector3(x + other.x, y + other.y, z + other.z);
        }
        
        Vector3 operator-(const Vector3& other) const {
            return Vector3(x - other.x, y - other.y, z - other.z);
        }
        
        float Distance(const Vector3& other) const {
            float dx = x - other.x;
            float dy = y - other.y;
            float dz = z - other.z;
            return sqrt(dx*dx + dy*dy + dz*dz);
        }
        
        float Magnitude() const {
            return sqrt(x*x + y*y + z*z);
        }
        
        Vector3 Normalize() const {
            float mag = Magnitude();
            if (mag == 0) return Vector3();
            return Vector3(x/mag, y/mag, z/mag);
        }
    };
    
    struct Vector2 {
        float x, y;
        
        Vector2() : x(0), y(0) {}
        Vector2(float x, float y) : x(x), y(y) {}
    };
    
    struct Quaternion {
        float x, y, z, w;
        
        Quaternion() : x(0), y(0), z(0), w(1) {}
        Quaternion(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}
    };
    
    struct Matrix4x4 {
        float m[4][4];
        
        Matrix4x4() {
            memset(m, 0, sizeof(m));
            m[0][0] = m[1][1] = m[2][2] = m[3][3] = 1.0f;
        }
    };
    
    // Unity GameObject wrapper
    struct GameObject {
        void* instance;
        
        GameObject(void* obj) : instance(obj) {}
        
        void* GetTransform() const {
            if (!instance) return nullptr;
            return *(void**)((uintptr_t)instance + FFMaxOffsets::Unity::GameObject_Transform);
        }
        
        int GetLayer() const {
            if (!instance) return 0;
            return *(int*)((uintptr_t)instance + FFMaxOffsets::Unity::GameObject_Layer);
        }
        
        bool IsActive() const {
            if (!instance) return false;
            return *(bool*)((uintptr_t)instance + FFMaxOffsets::Unity::GameObject_ActiveSelf);
        }
    };
    
    // Unity Transform wrapper
    struct Transform {
        void* instance;
        
        Transform(void* obj) : instance(obj) {}
        
        Vector3 GetPosition() const {
            if (!instance) return Vector3();
            return *(Vector3*)((uintptr_t)instance + FFMaxOffsets::Unity::Transform_Position);
        }
        
        void SetPosition(const Vector3& pos) {
            if (!instance) return;
            *(Vector3*)((uintptr_t)instance + FFMaxOffsets::Unity::Transform_Position) = pos;
        }
        
        Vector3 GetLocalPosition() const {
            if (!instance) return Vector3();
            return *(Vector3*)((uintptr_t)instance + FFMaxOffsets::Unity::Transform_LocalPosition);
        }
        
        Quaternion GetRotation() const {
            if (!instance) return Quaternion();
            return *(Quaternion*)((uintptr_t)instance + FFMaxOffsets::Unity::Transform_Rotation);
        }
        
        Vector3 GetLocalScale() const {
            if (!instance) return Vector3(1,1,1);
            return *(Vector3*)((uintptr_t)instance + FFMaxOffsets::Unity::Transform_LocalScale);
        }
    };
    
    // FreeFire Player structure
    struct Player {
        void* instance;
        
        Player(void* obj) : instance(obj) {}
        
        bool IsValid() const {
            return instance != nullptr;
        }
        
        Transform GetTransform() const {
            if (!instance) return Transform(nullptr);
            void* characterController = *(void**)((uintptr_t)instance + FFMaxOffsets::Game::Player_CharacterController);
            if (!characterController) return Transform(nullptr);
            void* gameObject = *(void**)((uintptr_t)characterController + FFMaxOffsets::Unity::Component_GameObject);
            if (!gameObject) return Transform(nullptr);
            void* transform = *(void**)((uintptr_t)gameObject + FFMaxOffsets::Unity::GameObject_Transform);
            return Transform(transform);
        }
        
        Vector3 GetPosition() const {
            Transform transform = GetTransform();
            return transform.GetPosition();
        }
        
        float GetHealth() const {
            if (!instance) return 0;
            return *(float*)((uintptr_t)instance + FFMaxOffsets::Game::Player_Health);
        }
        
        float GetMaxHealth() const {
            if (!instance) return 100;
            return *(float*)((uintptr_t)instance + FFMaxOffsets::Game::Player_MaxHealth);
        }
        
        float GetShield() const {
            if (!instance) return 0;
            return *(float*)((uintptr_t)instance + FFMaxOffsets::Game::Player_Shield);
        }
        
        float GetMaxShield() const {
            if (!instance) return 100;
            return *(float*)((uintptr_t)instance + FFMaxOffsets::Game::Player_MaxShield);
        }
        
        int GetTeamID() const {
            if (!instance) return 0;
            return *(int*)((uintptr_t)instance + FFMaxOffsets::Game::Player_TeamID);
        }
        
        int GetPlayerID() const {
            if (!instance) return 0;
            return *(int*)((uintptr_t)instance + FFMaxOffsets::Game::Player_PlayerID);
        }
        
        bool IsBot() const {
            if (!instance) return false;
            return *(bool*)((uintptr_t)instance + FFMaxOffsets::Game::Player_IsBot);
        }
        
        bool IsVisible() const {
            if (!instance) return false;
            return *(bool*)((uintptr_t)instance + FFMaxOffsets::Game::Player_IsVisible);
        }
        
        bool IsKnocked() const {
            if (!instance) return false;
            return *(bool*)((uintptr_t)instance + FFMaxOffsets::Game::Player_IsKnocked);
        }
        
        bool IsDead() const {
            if (!instance) return false;
            return *(bool*)((uintptr_t)instance + FFMaxOffsets::Game::Player_IsDead);
        }
        
        bool IsInVehicle() const {
            if (!instance) return false;
            return *(bool*)((uintptr_t)instance + FFMaxOffsets::Game::Player_GetInVehicle);
        }
        
        bool IsEnemy(const Player& localPlayer) const {
            if (!IsValid() || !localPlayer.IsValid()) return false;
            return GetTeamID() != localPlayer.GetTeamID();
        }
        
        float GetDistance(const Player& other) const {
            Vector3 pos1 = GetPosition();
            Vector3 pos2 = other.GetPosition();
            return pos1.Distance(pos2);
        }
    };
    
    // Camera structure for world-to-screen conversion
    struct Camera {
        void* instance;
        
        Camera(void* obj) : instance(obj) {}
        
        bool IsValid() const {
            return instance != nullptr;
        }
        
        Matrix4x4 GetViewMatrix() const {
            if (!instance) return Matrix4x4();
            return *(Matrix4x4*)((uintptr_t)instance + FFMaxOffsets::Unity::Camera_ViewMatrix);
        }
        
        Matrix4x4 GetProjectionMatrix() const {
            if (!instance) return Matrix4x4();
            return *(Matrix4x4*)((uintptr_t)instance + FFMaxOffsets::Unity::Camera_ProjectionMatrix);
        }
        
        Vector2 WorldToScreen(const Vector3& worldPos) const;
    };
    
    // ESP Configuration
    struct ESPConfig {
        bool enabled = false;
        bool showBox = true;
        bool showLine = true;
        bool showHealth = true;
        bool showDistance = true;
        bool showName = false;
        bool showTeam = true;
        bool showBots = false;
        bool showKnocked = true;
        bool showInVehicle = true;
        float maxDistance = 500.0f;
        
        // Colors
        struct Colors {
            ImVec4 enemy = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);      // Red
            ImVec4 team = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);       // Green
            ImVec4 bot = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);        // Yellow
            ImVec4 knocked = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);    // Gray
            ImVec4 vehicle = ImVec4(0.0f, 0.0f, 1.0f, 1.0f);    // Blue
            ImVec4 line = ImVec4(1.0f, 1.0f, 1.0f, 0.8f);       // White
            ImVec4 healthGreen = ImVec4(0.0f, 1.0f, 0.0f, 1.0f);
            ImVec4 healthYellow = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
            ImVec4 healthRed = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
        } colors;
    };
    
    // Aimbot Configuration
    struct AimbotConfig {
        bool enabled = false;
        bool aimKey = false;
        bool autoShoot = false;
        bool visibilityCheck = true;
        bool teamCheck = true;
        bool botCheck = false;
        bool knockedCheck = false;
        float fov = 90.0f;
        float smoothing = 1.0f;
        float maxDistance = 300.0f;
        int targetBone = 0; // 0 = Head, 1 = Chest, 2 = Body
        int aimKey_code = 0; // Key code for aim key
        
        enum TargetBone {
            HEAD = 0,
            CHEST = 1,
            BODY = 2
        };
    };
    
    // Cheat Configuration
    struct CheatConfig {
        ESPConfig esp;
        AimbotConfig aimbot;
        
        // Additional Features
        bool wallhack = false;
        bool speedHack = false;
        bool noRecoil = false;
        bool noSpread = false;
        bool infiniteAmmo = false;
        bool rapidFire = false;
        bool jumpHack = false;
        bool flyHack = false;
        
        float speedMultiplier = 2.0f;
        float jumpHeight = 2.0f;
        float fireRateMultiplier = 2.0f;
    };
}