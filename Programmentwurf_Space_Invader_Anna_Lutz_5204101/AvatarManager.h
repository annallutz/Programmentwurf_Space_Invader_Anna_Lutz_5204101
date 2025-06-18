#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include "InputManager.h"
#include <vector>
#include <string>

namespace SpaceInvaders {

    /**
     * @brief Avatar-Informations-Struktur
     */
    struct AvatarInfo {
        AvatarType type;
        std::string name;
        std::string description;
        int cost;
        bool isUnlocked;
        ButtonState buttonState;
    };

    /**
     * @brief AvatarManager - Verwaltet Avatar-Auswahl und -Freischaltung
     *
     * Zuständig für:
     * - Avatar-Freischaltung mit Coins
     * - Auswahl-Interface (SinglePlayer & MultiPlayer)
     * - Avatar-Informationen und Beschreibungen
     * - Speichern/Laden der freigeschalteten Avatare
     */
    class AvatarManager {
    public:
        // === Konstruktor & Destruktor ===
        AvatarManager();
        ~AvatarManager() = default;

        // === Initialisierung ===
        bool Initialize();
        void LoadAvatarUnlocks();
        void SaveAvatarUnlocks();

        // === SinglePlayer Avatar-Auswahl ===
        enum SinglePlayerResult {
            SP_SELECTING,
            SP_AVATAR_CHOSEN,
            SP_BACK_PRESSED
        };

        SinglePlayerResult UpdateSinglePlayerSelection(float deltaTime, int& coins);
        void RenderSinglePlayerSelection();
        AvatarType GetSelectedAvatar() const;
        void ResetSinglePlayerSelection();

        // === MultiPlayer Avatar-Auswahl ===
        enum MultiPlayerResult {
            MP_PLAYER1_SELECTING,
            MP_PLAYER2_SELECTING,
            MP_BOTH_CHOSEN,
            MP_BACK_PRESSED
        };

        MultiPlayerResult UpdateMultiPlayerSelection(float deltaTime);
        void RenderMultiPlayerSelection();
        AvatarType GetPlayer1Avatar() const;
        AvatarType GetPlayer2Avatar() const;
        void ResetMultiPlayerSelection();

        // === Avatar-Informationen ===
        const AvatarInfo& GetAvatarInfo(AvatarType type) const;
        std::string GetAvatarDescription(AvatarType type) const;
        bool IsAvatarUnlocked(AvatarType type) const;
        bool CanAfford(AvatarType type, int coins) const;

        // === Avatar kaufen ===
        bool PurchaseAvatar(AvatarType type, int& coins);

        // === Constants ===
        static const int AVATAR_COUNT = 5;

    private:
        // === Avatar-Daten ===
        std::vector<AvatarInfo> avatars;

        // === SinglePlayer State ===
        int spCurrentSelection;
        AvatarType spSelectedAvatar;
        bool spAvatarConfirmed;
        float spNotEnoughCoinsTimer;

        // === MultiPlayer State ===
        int mpPlayer1Selection;
        int mpPlayer2Selection;
        AvatarType mpPlayer1Avatar;
        AvatarType mpPlayer2Avatar;
        bool mpPlayer1Confirmed;
        bool mpPlayer2Confirmed;

        // === Input ===
        InputManager inputManager;

        // === Private Hilfsfunktionen ===
        void InitializeAvatarData();
        void HandleSinglePlayerInput(int& coins);
        void HandleMultiPlayerInput();
        void RenderAvatarGrid(bool isMultiPlayer = false);
        void RenderAvatarCard(const AvatarInfo& avatar, Rectangle rect, bool isSelected, bool isHovered, bool isPlayer1 = true);
        void RenderAvatarDescription(AvatarType type);
        void RenderCoinDisplay(int coins);
        void RenderSelectionIndicators();
        Rectangle GetAvatarRect(int index, bool isMultiPlayer = false);

        // === Animation ===
        void UpdateButtonAnimations(float deltaTime);
    };

} // namespace SpaceInvaders