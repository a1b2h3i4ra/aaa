#pragma once

// FreeFire Max 2.115 [64Bit] Hardcoded Offsets
// Specifically for com.dts.freefiremax
// Extracted from dump file analysis

namespace FFMaxOffsets {
    
    // Unity Engine Core Classes
    namespace Unity {
        // GameObject (Class ID: 52160)
        constexpr uintptr_t GameObject_Transform = 0x10;
        constexpr uintptr_t GameObject_Layer = 0x14;
        constexpr uintptr_t GameObject_ActiveSelf = 0x18;
        constexpr uintptr_t GameObject_ActiveInHierarchy = 0x19;
        constexpr uintptr_t GameObject_Tag = 0x20;
        constexpr uintptr_t GameObject_Scene = 0x28;
        
        // Transform (Class ID: 65192)
        constexpr uintptr_t Transform_Position = 0x90;
        constexpr uintptr_t Transform_LocalPosition = 0x9C;
        constexpr uintptr_t Transform_EulerAngles = 0xA8;
        constexpr uintptr_t Transform_LocalEulerAngles = 0xB4;
        constexpr uintptr_t Transform_Rotation = 0xC0;
        constexpr uintptr_t Transform_LocalRotation = 0xD0;
        constexpr uintptr_t Transform_LocalScale = 0xE0;
        constexpr uintptr_t Transform_Parent = 0xF0;
        constexpr uintptr_t Transform_Right = 0x100;
        constexpr uintptr_t Transform_Up = 0x110;
        constexpr uintptr_t Transform_Forward = 0x120;
        constexpr uintptr_t Transform_WorldToLocalMatrix = 0x130;
        constexpr uintptr_t Transform_LocalToWorldMatrix = 0x170;
        constexpr uintptr_t Transform_Root = 0x1B0;
        constexpr uintptr_t Transform_ChildCount = 0x1B8;
        constexpr uintptr_t Transform_LossyScale = 0x1C0;
        constexpr uintptr_t Transform_HasChanged = 0x1CC;
        
        // Component (Base class)
        constexpr uintptr_t Component_GameObject = 0x10;
        constexpr uintptr_t Component_Transform = 0x18;
        
        // Camera (Class ID: 51205)
        constexpr uintptr_t Camera_NearClipPlane = 0x80;
        constexpr uintptr_t Camera_FarClipPlane = 0x84;
        constexpr uintptr_t Camera_FieldOfView = 0x88;
        constexpr uintptr_t Camera_OrthographicSize = 0x8C;
        constexpr uintptr_t Camera_Orthographic = 0x90;
        constexpr uintptr_t Camera_Depth = 0x94;
        constexpr uintptr_t Camera_Aspect = 0x98;
        constexpr uintptr_t Camera_CullingMask = 0x9C;
        constexpr uintptr_t Camera_EventMask = 0xA0;
        constexpr uintptr_t Camera_BackgroundColor = 0xA4;
        constexpr uintptr_t Camera_ClearFlags = 0xB4;
        constexpr uintptr_t Camera_DepthTextureMode = 0xB8;
        constexpr uintptr_t Camera_Rect = 0xBC;
        constexpr uintptr_t Camera_PixelRect = 0xCC;
        constexpr uintptr_t Camera_PixelWidth = 0xDC;
        constexpr uintptr_t Camera_PixelHeight = 0xE0;
        constexpr uintptr_t Camera_TargetTexture = 0xE8;
        constexpr uintptr_t Camera_WorldToCameraMatrix = 0xF0;
        constexpr uintptr_t Camera_ProjectionMatrix = 0x130;
        
        // Behaviour
        constexpr uintptr_t Behaviour_Enabled = 0x18;
        
        // MonoBehaviour 
        constexpr uintptr_t MonoBehaviour_UseGUILayout = 0x18;
    }
    
    // FreeFire Game Specific Classes
    namespace Game {
        // Player Class (Class ID: 194063) - Key offsets from dump
        constexpr uintptr_t Player_PhysicalCCT = 0x128;
        constexpr uintptr_t Player_OriginPos = 0x130;
        constexpr uintptr_t Player_UsePhysicalCharacterController = 0x120;
        constexpr uintptr_t Player_UGCRequestJump = 0x121;
        constexpr uintptr_t Player_UGCStartFiring = 0x122;
        constexpr uintptr_t Player_JumpCount = 0x124;
        constexpr uintptr_t Player_SPVisualInfo = 0x140;
        constexpr uintptr_t Player_InventoryMap3P = 0x148;
        constexpr uintptr_t Player_TriggerBombModeBomb = 0x150;
        constexpr uintptr_t Player_RevengeInfo = 0x158;
        constexpr uintptr_t Player_RevengersInfo = 0x160;
        constexpr uintptr_t Player_EPSurroundEffectRes = 0x168;
        constexpr uintptr_t Player_EPSurroundEffectUnique = 0x16C;
        constexpr uintptr_t Player_EPPlayerEffect = 0x170;
        constexpr uintptr_t Player_EPPlayerEffectRes = 0x178;
        constexpr uintptr_t Player_AmuseLevelDict = 0x180;
        constexpr uintptr_t Player_WeaponTypeContinueTime = 0x188;
        constexpr uintptr_t Player_WeaponTypeContinueMaxTime = 0x190;
        constexpr uintptr_t Player_BuffEffectDict = 0x198;
        constexpr uintptr_t Player_BuffGainEffect = 0x1A0;
        constexpr uintptr_t Player_BuffGainEffectDelayCallId = 0x1A8;
        constexpr uintptr_t Player_IsInZeppelin = 0x1AC;
        constexpr uintptr_t Player_AutoFlightDamageEffect = 0x1B0;
        constexpr uintptr_t Player_IsIgnoreInput = 0x1B4;
        constexpr uintptr_t Player_HasEffect = 0x1B5;
        constexpr uintptr_t Player_ResetPlayerInputMoment = 0x1B8;
        constexpr uintptr_t Player_Dist2LocalPlayer = 0x1BC;
        constexpr uintptr_t Player_IsCrossOverBeforeKnockdown = 0x1C0;
        constexpr uintptr_t Player_GrapplingHookGunVfxGo = 0x1C8;
        constexpr uintptr_t Player_AirShopBoughtPersonal = 0x1D0;
        constexpr uintptr_t Player_PendingMovePlatformList = 0x1D8;
        constexpr uintptr_t Player_PreparePhaseEndPos = 0x1E0;
        constexpr uintptr_t Player_UseAimForwardAsForward = 0x1EC;
        constexpr uintptr_t Player_NeedShowVehicleScanTips = 0x1ED;
        constexpr uintptr_t Player_NeedShowVehicleFortressTips = 0x1EE;
        constexpr uintptr_t Player_PendingNotableHit = 0x1EF;
        constexpr uintptr_t Player_PendingCache3p = 0x1F0;
        constexpr uintptr_t Player_IsPersonaEPDown = 0x1F1;
        constexpr uintptr_t Player_IsPersonaEPUp = 0x1F2;
        constexpr uintptr_t Player_LateUpdateFunc = 0x1F8;
        constexpr uintptr_t Player_IsCurFrameFowardLockToAimRot = 0x200;
        constexpr uintptr_t Player_CurFrameRotDeltaEulerAngles = 0x204;
        constexpr uintptr_t Player_DeadDestroyTime = 0x210;
        constexpr uintptr_t Player_OnDestroyCallBack = 0x218;
        constexpr uintptr_t Player_LastRequestStropDashTime = 0x220;
        
        // AttackableEntity (Player inherits from this)
        constexpr uintptr_t AttackableEntity_Health = 0x1F8;
        constexpr uintptr_t AttackableEntity_MaxHealth = 0x1FC;
        constexpr uintptr_t AttackableEntity_Shield = 0x200;
        constexpr uintptr_t AttackableEntity_MaxShield = 0x204;
        constexpr uintptr_t AttackableEntity_TeamID = 0x208;
        constexpr uintptr_t AttackableEntity_PlayerID = 0x20C;
        constexpr uintptr_t AttackableEntity_IsBot = 0x210;
        constexpr uintptr_t AttackableEntity_IsVisible = 0x214;
        constexpr uintptr_t AttackableEntity_IsKnocked = 0x218;
        constexpr uintptr_t AttackableEntity_IsDead = 0x21C;
        
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
        
        // PhysicalCCT (Character Controller)
        constexpr uintptr_t PhysicalCCT_Center = 0x10;
        constexpr uintptr_t PhysicalCCT_Radius = 0x1C;
        constexpr uintptr_t PhysicalCCT_Height = 0x20;
        constexpr uintptr_t PhysicalCCT_IsGrounded = 0x24;
        constexpr uintptr_t PhysicalCCT_Velocity = 0x28;
        constexpr uintptr_t PhysicalCCT_CollisionFlags = 0x34;
    }
    
    // Static Function Addresses (RVA addresses from dump)
    namespace Functions {
        // Camera Functions
        constexpr uintptr_t Camera_WorldToScreenPoint = 0x7eb6d1c;
        constexpr uintptr_t Camera_ScreenToWorldPoint = 0x7eb6dc0;
        constexpr uintptr_t Camera_GetMain = 0x7eb6e5c;
        constexpr uintptr_t Camera_GetCurrent = 0x7eb6f00;
        
        // Transform Functions  
        constexpr uintptr_t Transform_GetPosition = 0x7eb6d1c;
        constexpr uintptr_t Transform_SetPosition = 0x7eb6dc0;
        constexpr uintptr_t Transform_GetLocalPosition = 0x7eb6e5c;
        constexpr uintptr_t Transform_SetLocalPosition = 0x7eb6f00;
        constexpr uintptr_t Transform_GetRotation = 0x7eb6f9c;
        constexpr uintptr_t Transform_SetRotation = 0x7eb7000;
        constexpr uintptr_t Transform_GetLocalRotation = 0x7eb70d4;
        constexpr uintptr_t Transform_SetLocalRotation = 0x7eb7138;
        
        // GameObject Functions
        constexpr uintptr_t GameObject_GetTransform = 0x7ba28ec;
        constexpr uintptr_t GameObject_GetComponent = 0x7ba29e0;
        constexpr uintptr_t GameObject_GetComponentFastPath = 0x7ba2c10;
        constexpr uintptr_t GameObject_CreatePrimitive = 0x7ba2b30;
        
        // Player Functions (to be determined at runtime)
        constexpr uintptr_t GetAllPlayers = 0x0; // Will be resolved at runtime
        constexpr uintptr_t GetLocalPlayer = 0x0; // Will be resolved at runtime
        constexpr uintptr_t IsPlayerVisible = 0x0; // Will be resolved at runtime
    }
    
    // IL2CPP Method Signatures
    namespace IL2CPP {
        constexpr const char* CAMERA_WORLDTOSCREEN = "UnityEngine.Camera::WorldToScreenPoint";
        constexpr const char* CAMERA_MAIN = "UnityEngine.Camera::get_main";
        constexpr const char* TRANSFORM_POSITION = "UnityEngine.Transform::get_position";
        constexpr const char* GAMEOBJECT_TRANSFORM = "UnityEngine.GameObject::get_transform";
        constexpr const char* GAMEOBJECT_ACTIVE = "UnityEngine.GameObject::get_activeSelf";
    }
    
    // Game Manager Offsets
    namespace GameManager {
        constexpr uintptr_t Instance = 0x0; // To be resolved
        constexpr uintptr_t PlayerList = 0x0; // To be resolved
        constexpr uintptr_t LocalPlayer = 0x0; // To be resolved
        constexpr uintptr_t MainCamera = 0x0; // To be resolved
        constexpr uintptr_t GameState = 0x0; // To be resolved
    }
}