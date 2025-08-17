#pragma once

// FreeFire Max 2.115 [64Bit] Hardcoded Offsets
// Specifically for com.dts.freefiremax

namespace FFMaxOffsets {
    
    // Unity Engine Core Classes
    namespace Unity {
        // GameObject (0x52160)
        constexpr uintptr_t GameObject_Transform = 0x10;
        constexpr uintptr_t GameObject_Layer = 0x14;
        constexpr uintptr_t GameObject_ActiveSelf = 0x18;
        constexpr uintptr_t GameObject_ActiveInHierarchy = 0x19;
        
        // Transform (0x65192)
        constexpr uintptr_t Transform_Position = 0x90;
        constexpr uintptr_t Transform_LocalPosition = 0x9C;
        constexpr uintptr_t Transform_Rotation = 0xA8;
        constexpr uintptr_t Transform_LocalRotation = 0xB8;
        constexpr uintptr_t Transform_LocalScale = 0xC8;
        constexpr uintptr_t Transform_Parent = 0xD4;
        
        // Component (0x51704)
        constexpr uintptr_t Component_GameObject = 0x10;
        constexpr uintptr_t Component_Transform = 0x18;
        
        // Camera (0x51205)
        constexpr uintptr_t Camera_ViewMatrix = 0x2C;
        constexpr uintptr_t Camera_ProjectionMatrix = 0x6C;
        constexpr uintptr_t Camera_WorldToCameraMatrix = 0xAC;
        constexpr uintptr_t Camera_CameraToWorldMatrix = 0xEC;
        
        // CharacterController (0x68296)
        constexpr uintptr_t CharacterController_Center = 0x10;
        constexpr uintptr_t CharacterController_Radius = 0x1C;
        constexpr uintptr_t CharacterController_Height = 0x20;
        constexpr uintptr_t CharacterController_IsGrounded = 0x24;
        constexpr uintptr_t CharacterController_Velocity = 0x28;
    }
    
    // FreeFire Game Specific Classes
    namespace Game {
        // Player Class (0x194063)
        constexpr uintptr_t Player_CharacterController = 0x380;
        constexpr uintptr_t Player_PhysicalCCT = 0x128;
        constexpr uintptr_t Player_OriginPos = 0x130;
        constexpr uintptr_t Player_BlackBoard = 0x370;
        constexpr uintptr_t Player_UserControl = 0x378;
        constexpr uintptr_t Player_AvatarInitialized = 0x368;
        constexpr uintptr_t Player_IsDoingAction = 0x369;
        constexpr uintptr_t Player_IsReady = 0x388;
        constexpr uintptr_t Player_IsInShop = 0x389;
        constexpr uintptr_t Player_IsCuring = 0x398;
        constexpr uintptr_t Player_GetInVehicle = 0x3A0;
        constexpr uintptr_t Player_IsPreparing = 0x3A1;
        
        // Health and Stats
        constexpr uintptr_t Player_Health = 0x1F8;
        constexpr uintptr_t Player_MaxHealth = 0x1FC;
        constexpr uintptr_t Player_Shield = 0x200;
        constexpr uintptr_t Player_MaxShield = 0x204;
        constexpr uintptr_t Player_TeamID = 0x208;
        constexpr uintptr_t Player_PlayerID = 0x20C;
        constexpr uintptr_t Player_IsBot = 0x210;
        constexpr uintptr_t Player_IsVisible = 0x214;
        constexpr uintptr_t Player_IsKnocked = 0x218;
        constexpr uintptr_t Player_IsDead = 0x21C;
        
        // Weapon Related
        constexpr uintptr_t Player_CurrentWeapon = 0x2A0;
        constexpr uintptr_t Player_WeaponManager = 0x2A8;
        constexpr uintptr_t Weapon_WeaponID = 0x10;
        constexpr uintptr_t Weapon_Damage = 0x14;
        constexpr uintptr_t Weapon_Range = 0x18;
        constexpr uintptr_t Weapon_Accuracy = 0x1C;
        constexpr uintptr_t Weapon_FireRate = 0x20;
        constexpr uintptr_t Weapon_ReloadSpeed = 0x24;
        constexpr uintptr_t Weapon_ClipSize = 0x28;
        constexpr uintptr_t Weapon_CurrentAmmo = 0x2C;
    }
    
    // Camera and Rendering
    namespace Rendering {
        constexpr uintptr_t MainCamera = 0x0;
        constexpr uintptr_t ViewMatrix = 0x2C;
        constexpr uintptr_t ProjectionMatrix = 0x6C;
        constexpr uintptr_t ScreenWidth = 0xAC;
        constexpr uintptr_t ScreenHeight = 0xB0;
    }
    
    // Static Function Addresses (These would need to be updated based on actual memory layout)
    namespace Functions {
        constexpr uintptr_t GetAllPlayers = 0x0; // To be filled with actual address
        constexpr uintptr_t GetLocalPlayer = 0x0; // To be filled with actual address
        constexpr uintptr_t WorldToScreen = 0x0; // To be filled with actual address
        constexpr uintptr_t GetMainCamera = 0x0; // To be filled with actual address
        constexpr uintptr_t IsPlayerVisible = 0x0; // To be filled with actual address
    }
}