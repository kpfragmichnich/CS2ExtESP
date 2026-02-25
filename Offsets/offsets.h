#pragma once
#include <cstdint>

// ==========================================================
// LUNIX CS2 - USER OFFSET CONFIGURATION (v1.2.8)
// ==========================================================
// Hier kannst du die Offsets anpassen. 
// Ich habe Kommentare hinzugefügt, damit wir wissen, was was ist.
// ==========================================================

namespace LunixOffsets {
    
    // --- Globale Pointer (client.dll) ---
    constexpr std::ptrdiff_t dwEntityList = 0x24AB1C8;           // Die Liste aller Spieler
    constexpr std::ptrdiff_t dwLocalPlayerController = 0x22F0188; // Dein eigener Controller
    constexpr std::ptrdiff_t dwViewMatrix = 0x230BEE0;            // Für 3D-zu-2D (ESP Boxen)
    constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x2065AF0;

    // --- Controller Offsets (CBasePlayerController) ---
    constexpr std::ptrdiff_t m_hPawn = 0x6C4; // CHandle<C_BasePlayerPawn>                // Link vom Controller zum Körper (Pawn)
    constexpr std::ptrdiff_t m_iTeamNum = 0x3F3; // uint8

    // --- Pawn Offsets (C_BasePlayerPawn) ---
    constexpr std::ptrdiff_t m_iHealth = 0x354; // int32
    constexpr std::ptrdiff_t m_vOldOrigin = 0x1588; // Vector
    constexpr std::ptrdiff_t m_pGameSceneNode = 0x338; // CGameSceneNode*
    
    // --- Sonstiges ---
    constexpr std::ptrdiff_t m_pPredictionOwner = 0x16B8; // CBasePlayerController*

    // --- Validation & Filtering (v1.3.1) ---
    constexpr std::ptrdiff_t m_sSanitizedPlayerName = 0x860;    // CUtlString
    constexpr std::ptrdiff_t m_bIsLocalPlayerController = 0x788; // bool
}
