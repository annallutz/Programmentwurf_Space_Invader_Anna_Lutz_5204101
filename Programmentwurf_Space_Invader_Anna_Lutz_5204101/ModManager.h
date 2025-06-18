#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include "InputManager.h"
#include <vector>
#include <string>

namespace SpaceInvaders {

    /**
     * @brief Mod-Informations-Struktur
     */
    struct ModInfo {
        ModType type;
        std::string name;
        std::string description;
        int cost;
        bool isUnlocked;
        ButtonState buttonState;
    };

    /**
     * @brief ModManager - Verwaltet Modifikator-Auswahl und -Freischaltung
     *
     * Zuständig für:
     * - Mod-Freischaltung mit Coins
     * - Auswahl-Interface vor Spielstart
     * - Mod-Informationen und Beschreibungen
     * - Speichern/Laden der freigeschalteten Mods
     */
    class ModManager {
    public:
        // === Konstruktor & Destruktor ===
        ModManager();
        ~ModManager() = default;

        // === Initialisierung ===
        bool Initialize();
        void LoadModUnlocks();
        void SaveModUnlocks();

        // === Mod-Auswahl ===
        enum SelectionResult {
            SELECTING,
            MOD_CHOSEN,
            BACK_PRESSED
        };

        SelectionResult UpdateModSelection(float deltaTime, int& coins);
        void RenderModSelection(int coins);
        ModType GetSelectedMod() const;
        void ResetSelection();

        // === Mod-Informationen ===
        const ModInfo& GetModInfo(ModType type) const;
        std::string GetModDescription(ModType type) const;
        bool IsModUnlocked(ModType type) const;
        bool CanAfford(ModType type, int coins) const;

        // === Mod kaufen ===
        bool PurchaseMod(ModType type, int& coins);

        // === Constants ===
        static const int MOD_COUNT = 5;

    private:
        // === Mod-Daten ===
        std::vector<ModInfo> mods;

        // === Selection State ===
        int currentSelection;
        ModType selectedMod;
        bool modConfirmed;
        float notEnoughCoinsTimer;

        // === Input ===
        InputManager inputManager;

        // === Private Hilfsfunktionen ===
        void InitializeModData();
        void HandleInput(int& coins);
        void RenderModGrid();
        void RenderModCard(const ModInfo& mod, Rectangle rect, bool isSelected, bool isHovered);
        void RenderModDescription(ModType type);
        void RenderCoinDisplay(int coins);
        Rectangle GetModRect(int index);

        // === Animation ===
        void UpdateButtonAnimations(float deltaTime);
    };

} // namespace SpaceInvaders