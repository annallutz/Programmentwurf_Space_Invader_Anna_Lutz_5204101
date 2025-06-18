#include "ModManager.h"
#include "TextureManager.h"
#include <fstream>
#include <iostream>

namespace SpaceInvaders {

    // === Konstruktor ===
    ModManager::ModManager()
        : currentSelection(0)
        , selectedMod(MOD_NONE)
        , modConfirmed(false)
        , notEnoughCoinsTimer(0.0f)
    {
    }

    // === Initialisierung ===
    bool ModManager::Initialize() {
        InitializeModData();
        LoadModUnlocks();
        return true;
    }

    void ModManager::LoadModUnlocks() {
        std::ifstream file(GameConfig::MOD_UNLOCKS_FILE);

        // Standardmäßig alle gesperrt außer MOD_NONE
        for (auto& mod : mods) {
            mod.isUnlocked = (mod.type == MOD_NONE);
        }

        if (file.is_open()) {
            int modType;
            while (file >> modType) {
                if (modType >= 0 && modType < MOD_COUNT) {
                    mods[modType].isUnlocked = true;
                }
            }
            file.close();
        }
    }

    void ModManager::SaveModUnlocks() {
        std::ofstream file(GameConfig::MOD_UNLOCKS_FILE);
        if (file.is_open()) {
            for (const auto& mod : mods) {
                if (mod.isUnlocked) {
                    file << (int)mod.type << "\n";
                }
            }
            file.close();
        }
    }

    // === Mod-Auswahl ===
    ModManager::SelectionResult ModManager::UpdateModSelection(float deltaTime, int& coins) {
        UpdateButtonAnimations(deltaTime);

        if (notEnoughCoinsTimer > 0) {
            notEnoughCoinsTimer -= deltaTime;
        }

        HandleInput(coins);

        if (modConfirmed) {
            return MOD_CHOSEN;
        }

        // Prüfen auf Zurück-Input
        if (inputManager.GetNavigationInput() == InputManager::NAV_BACK) {
            return BACK_PRESSED;
        }

        return SELECTING;
    }

    void ModManager::RenderModSelection(int coins) {
        BeginDrawing();
        ClearBackground(BLACK);

        // Titel
        const char* title = "Modifikator waehlen:";
        int titleWidth = MeasureText(title, 40);
        DrawText(title, (GameConfig::SCREEN_WIDTH - titleWidth) / 2, 50, 40, WHITE);

        // Coins anzeigen
        RenderCoinDisplay(coins);

        // Mod-Grid rendern
        RenderModGrid();

        // Beschreibung des aktuell ausgewählten Mods
        RenderModDescription(mods[currentSelection].type);

        // Anweisungen
        DrawText("Pfeiltasten: Navigieren | ENTER: Auswaehlen",
            50, GameConfig::SCREEN_HEIGHT - 60, 20, GRAY);

        // Nicht genug Coins Warnung
        if (notEnoughCoinsTimer > 0) {
            const char* warning = "Nicht genug Coins!";
            int warningWidth = MeasureText(warning, 30);
            DrawText(warning, (GameConfig::SCREEN_WIDTH - warningWidth) / 2,
                GameConfig::SCREEN_HEIGHT - 100, 30, RED);
        }

        EndDrawing();
    }

    ModType ModManager::GetSelectedMod() const {
        return selectedMod;
    }

    void ModManager::ResetSelection() {
        currentSelection = 0;
        selectedMod = MOD_NONE;
        modConfirmed = false;
        notEnoughCoinsTimer = 0.0f;
    }

    // === Mod-Informationen ===
    const ModInfo& ModManager::GetModInfo(ModType type) const {
        return mods[(int)type];
    }

    std::string ModManager::GetModDescription(ModType type) const {
        return mods[(int)type].description;
    }

    bool ModManager::IsModUnlocked(ModType type) const {
        return mods[(int)type].isUnlocked;
    }

    bool ModManager::CanAfford(ModType type, int coins) const {
        return coins >= mods[(int)type].cost;
    }

    // === Mod kaufen ===
    bool ModManager::PurchaseMod(ModType type, int& coins) {
        if (IsModUnlocked(type)) return true;

        if (CanAfford(type, coins)) {
            coins -= mods[(int)type].cost;
            mods[(int)type].isUnlocked = true;
            SaveModUnlocks();
            return true;
        }

        return false;
    }

    // === Private Hilfsfunktionen ===
    void ModManager::InitializeModData() {
        mods.resize(MOD_COUNT);

        // Kein Mod (kostenlos)
        mods[MOD_NONE] = {
            MOD_NONE,
            "Kein Mod",
            "Normales Spiel ohne besondere Modifikationen",
            0,
            true, // Immer freigeschaltet
            {}
        };

        // FastFire Mod
        mods[MOD_FASTFIRE] = {
            MOD_FASTFIRE,
            "Schnellfeuer",
            "Startet mit erhoehter Schussfrequenz",
            GameConfig::MOD_COST,
            false,
            {}
        };

        // Shield Mod
        mods[MOD_SHIELD] = {
            MOD_SHIELD,
            "Schild",
            "Startet mit aktivem Schutzschild",
            GameConfig::MOD_COST,
            false,
            {}
        };

        // TripleShot Mod
        mods[MOD_TRIPLESHOT] = {
            MOD_TRIPLESHOT,
            "Dreifach-Schuss",
            "Startet mit Dreifach-Schuss Faehigkeit",
            GameConfig::MOD_COST,
            false,
            {}
        };

        // SlowMo Mod
        mods[MOD_SLOWMO] = {
            MOD_SLOWMO,
            "Zeitlupe",
            "Startet mit aktivierter Zeitlupe",
            GameConfig::MOD_COST,
            false,
            {}
        };
    }

    void ModManager::HandleInput(int& coins) {
        auto navInput = inputManager.GetNavigationInput();

        switch (navInput) {
        case InputManager::NAV_LEFT:
            currentSelection = (currentSelection - 1 + MOD_COUNT) % MOD_COUNT;
            break;

        case InputManager::NAV_RIGHT:
            currentSelection = (currentSelection + 1) % MOD_COUNT;
            break;

        case InputManager::NAV_CONFIRM: {
            ModType selectedType = mods[currentSelection].type;

            if (IsModUnlocked(selectedType)) {
                selectedMod = selectedType;
                modConfirmed = true;
            }
            else if (PurchaseMod(selectedType, coins)) {
                selectedMod = selectedType;
                modConfirmed = true;
            }
            else {
                notEnoughCoinsTimer = 2.0f;
            }
            break;
        }

        default:
            break;
        }

        // Maus-Input für Mod-Karten
        for (int i = 0; i < MOD_COUNT; i++) {
            Rectangle modRect = GetModRect(i);
            if (UpdateAnimatedButton(modRect, mods[i].buttonState, WHITE, YELLOW, ORANGE, GetFrameTime())) {
                currentSelection = i;
                ModType selectedType = mods[i].type;

                if (IsModUnlocked(selectedType)) {
                    selectedMod = selectedType;
                    modConfirmed = true;
                }
                else if (PurchaseMod(selectedType, coins)) {
                    selectedMod = selectedType;
                    modConfirmed = true;
                }
                else {
                    notEnoughCoinsTimer = 2.0f;
                }
            }
        }
    }

    void ModManager::RenderModGrid() {
        const float modSize = 96.0f;
        const float spacing = 160.0f;
        const float startX = (GameConfig::SCREEN_WIDTH - (MOD_COUNT * spacing - (spacing - modSize))) / 2;
        const float startY = 150.0f;

        for (int i = 0; i < MOD_COUNT; i++) {
            Rectangle modRect = {
                startX + i * spacing,
                startY,
                modSize,
                modSize
            };

            bool isSelected = (i == currentSelection);
            bool isHovered = inputManager.IsMouseOverRectangle(modRect);

            RenderModCard(mods[i], modRect, isSelected, isHovered);
        }
    }

    void ModManager::RenderModCard(const ModInfo& mod, Rectangle rect, bool isSelected, bool isHovered) {
        auto& textureManager = TextureManager::GetInstance();

        // Rahmen-Farbe bestimmen
        Color frameColor = WHITE;
        float frameThickness = 2.0f;

        if (mod.isUnlocked) {
            if (isSelected) {
                frameColor = YELLOW;
                frameThickness = 4.0f;

                // Leuchtender Effekt für ausgewählte Mods
                DrawRectangleRec({ rect.x - 5, rect.y - 5, rect.width + 10, rect.height + 10 },
                    Fade(YELLOW, 0.3f));
            }
            else if (isHovered) {
                frameColor = LIGHTGRAY;
                frameThickness = 3.0f;
            }
        }
        else {
            frameColor = DARKGRAY;
        }

        // Hintergrund
        DrawRectangleRec(rect, Fade(BLACK, 0.8f));

        // Mod-Icon
        Texture2D modTexture = textureManager.GetModIconTexture(mod.type);
        Color tint = mod.isUnlocked ? WHITE : Fade(GRAY, 0.5f);
        TextureManager::DrawTextureScaled(modTexture, rect.x + 10, rect.y + 10, rect.width - 20, rect.height - 20, tint);

        // Rahmen
        DrawRectangleLinesEx(rect, frameThickness, frameColor);

        // Schloss-Symbol für gesperrte Mods
        if (!mod.isUnlocked) {
            DrawText("LOCKED", rect.x + 15, rect.y + rect.height - 25, 16, RED);

            // Kosten anzeigen
            auto& coinTexture = textureManager.GetCoinTexture();
            TextureManager::DrawTextureScaled(coinTexture, rect.x + 5, rect.y + 5, 20, 20);
            DrawText(TextFormat("%d", mod.cost), rect.x + 30, rect.y + 5, 16, YELLOW);
        }

        // Name unter dem Mod
        const char* name = mod.name.c_str();
        int nameWidth = MeasureText(name, 14);
        DrawText(name, rect.x + (rect.width - nameWidth) / 2, rect.y + rect.height + 5, 14, WHITE);
    }

    void ModManager::RenderModDescription(ModType type) {
        const ModInfo& mod = mods[(int)type];

        // Beschreibungs-Box
        Rectangle descBox = { 50, 350, GameConfig::SCREEN_WIDTH - 100, 100 };
        DrawRectangleRec(descBox, Fade(DARKGRAY, 0.8f));
        DrawRectangleLinesEx(descBox, 2, WHITE);

        // Titel
        DrawText(mod.name.c_str(), descBox.x + 20, descBox.y + 20, 24, YELLOW);

        // Beschreibung
        DrawText(mod.description.c_str(), descBox.x + 20, descBox.y + 50, 18, WHITE);

        // Status
        if (mod.isUnlocked) {
            DrawText("FREIGESCHALTET", descBox.x + 20, descBox.y + 75, 16, GREEN);
        }
        else {
            DrawText(TextFormat("Kosten: %d Coins", mod.cost), descBox.x + 20, descBox.y + 75, 16, ORANGE);
        }
    }

    void ModManager::RenderCoinDisplay(int coins) {
        auto& textureManager = TextureManager::GetInstance();
        auto& coinTexture = textureManager.GetCoinTexture();

        int x = GameConfig::SCREEN_WIDTH - 150;
        int y = 20;
        int fontSize = 30;

        // Coins-Text
        const char* coinText = TextFormat("%d", coins);
        int textWidth = MeasureText(coinText, fontSize);
        DrawText(coinText, x, y, fontSize, YELLOW);

        // Coin-Icon
        TextureManager::DrawTextureScaled(coinTexture,
            x + textWidth + 10, y - 3,
            fontSize + 6, fontSize + 6);
    }

    Rectangle ModManager::GetModRect(int index) {
        const float modSize = 96.0f;
        const float spacing = 160.0f;
        const float startX = (GameConfig::SCREEN_WIDTH - (MOD_COUNT * spacing - (spacing - modSize))) / 2;
        const float startY = 150.0f;

        return {
            startX + index * spacing,
            startY,
            modSize,
            modSize
        };
    }

    void ModManager::UpdateButtonAnimations(float deltaTime) {
        for (auto& mod : mods) {
            // Animation für Hover-Effekte wird automatisch in UpdateAnimatedButton behandelt
            // Hier könnten zusätzliche Animationen hinzugefügt werden
        }
    }

} // namespace SpaceInvaders