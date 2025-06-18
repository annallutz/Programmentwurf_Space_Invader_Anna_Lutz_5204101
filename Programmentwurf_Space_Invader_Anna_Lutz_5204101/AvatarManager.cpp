#include "AvatarManager.h"
#include "TextureManager.h"
#include <fstream>
#include <iostream>

namespace SpaceInvaders {

    // === Konstruktor ===
    AvatarManager::AvatarManager()
        : spCurrentSelection(0)
        , spSelectedAvatar(AVATAR_BLUE)
        , spAvatarConfirmed(false)
        , spNotEnoughCoinsTimer(0.0f)
        , mpPlayer1Selection(0)
        , mpPlayer2Selection(1)
        , mpPlayer1Avatar(AVATAR_BLUE)
        , mpPlayer2Avatar(AVATAR_GREEN)
        , mpPlayer1Confirmed(false)
        , mpPlayer2Confirmed(false)
    {
    }

    // === Initialisierung ===
    bool AvatarManager::Initialize() {
        InitializeAvatarData();
        LoadAvatarUnlocks();
        return true;
    }

    void AvatarManager::LoadAvatarUnlocks() {
        std::ifstream file(GameConfig::AVATAR_UNLOCKS_FILE);

        // Standardmäßig alle gesperrt außer dem blauen Avatar
        for (auto& avatar : avatars) {
            avatar.isUnlocked = (avatar.type == AVATAR_BLUE);
        }

        if (file.is_open()) {
            int avatarType;
            while (file >> avatarType) {
                if (avatarType >= 0 && avatarType < AVATAR_COUNT) {
                    avatars[avatarType].isUnlocked = true;
                }
            }
            file.close();
        }
    }

    void AvatarManager::SaveAvatarUnlocks() {
        std::ofstream file(GameConfig::AVATAR_UNLOCKS_FILE);
        if (file.is_open()) {
            for (const auto& avatar : avatars) {
                if (avatar.isUnlocked) {
                    file << (int)avatar.type << "\n";
                }
            }
            file.close();
        }
    }

    // === SinglePlayer Avatar-Auswahl ===
    AvatarManager::SinglePlayerResult AvatarManager::UpdateSinglePlayerSelection(float deltaTime, int& coins) {
        UpdateButtonAnimations(deltaTime);

        if (spNotEnoughCoinsTimer > 0) {
            spNotEnoughCoinsTimer -= deltaTime;
        }

        HandleSinglePlayerInput(coins);

        if (spAvatarConfirmed) {
            return SP_AVATAR_CHOSEN;
        }

        // Prüfen auf Zurück-Input
        if (inputManager.GetNavigationInput() == InputManager::NAV_BACK) {
            return SP_BACK_PRESSED;
        }

        return SP_SELECTING;
    }

    void AvatarManager::RenderSinglePlayerSelection() {
        BeginDrawing();
        ClearBackground(BLACK);

        // Titel
        const char* title = "Avatar waehlen:";
        int titleWidth = MeasureText(title, 40);
        DrawText(title, (GameConfig::SCREEN_WIDTH - titleWidth) / 2, 50, 40, WHITE);

        // Avatar-Grid rendern
        RenderAvatarGrid(false);

        // Beschreibung des aktuell ausgewählten Avatars
        RenderAvatarDescription(avatars[spCurrentSelection].type);

        // Anweisungen
        DrawText("Pfeiltasten: Navigieren | ENTER: Auswaehlen",
            50, GameConfig::SCREEN_HEIGHT - 60, 20, GRAY);

        // Nicht genug Coins Warnung
        if (spNotEnoughCoinsTimer > 0) {
            const char* warning = "Nicht genug Coins!";
            int warningWidth = MeasureText(warning, 30);
            DrawText(warning, (GameConfig::SCREEN_WIDTH - warningWidth) / 2,
                GameConfig::SCREEN_HEIGHT - 100, 30, RED);
        }

        EndDrawing();
    }

    AvatarType AvatarManager::GetSelectedAvatar() const {
        return spSelectedAvatar;
    }

    void AvatarManager::ResetSinglePlayerSelection() {
        spCurrentSelection = 0;
        spSelectedAvatar = AVATAR_BLUE;
        spAvatarConfirmed = false;
        spNotEnoughCoinsTimer = 0.0f;
    }

    // === MultiPlayer Avatar-Auswahl ===
    AvatarManager::MultiPlayerResult AvatarManager::UpdateMultiPlayerSelection(float deltaTime) {
        UpdateButtonAnimations(deltaTime);
        HandleMultiPlayerInput();

        if (mpPlayer1Confirmed && mpPlayer2Confirmed) {
            return MP_BOTH_CHOSEN;
        }

        if (inputManager.GetNavigationInput() == InputManager::NAV_BACK) {
            return MP_BACK_PRESSED;
        }

        if (!mpPlayer1Confirmed) {
            return MP_PLAYER1_SELECTING;
        }
        else {
            return MP_PLAYER2_SELECTING;
        }
    }

    void AvatarManager::RenderMultiPlayerSelection() {
        BeginDrawing();
        ClearBackground(BLACK);

        // Titel
        const char* title = mpPlayer1Confirmed ?
            "Player 2: Avatar waehlen" :
            "Player 1: Avatar waehlen";
        int titleWidth = MeasureText(title, 40);
        DrawText(title, (GameConfig::SCREEN_WIDTH - titleWidth) / 2, 50, 40, WHITE);

        // Avatar-Grid rendern
        RenderAvatarGrid(true);

        // Auswahl-Indikatoren
        RenderSelectionIndicators();

        // Steuerung anzeigen
        if (!mpPlayer1Confirmed) {
            DrawText("Player 1: A/D bewegen, W auswaehlen", 50, GameConfig::SCREEN_HEIGHT - 80, 20, YELLOW);
        }
        else {
            DrawText("Player 2: Pfeiltasten bewegen, Pfeil-Hoch auswaehlen", 50, GameConfig::SCREEN_HEIGHT - 80, 20, SKYBLUE);
        }

        EndDrawing();
    }

    AvatarType AvatarManager::GetPlayer1Avatar() const {
        return mpPlayer1Avatar;
    }

    AvatarType AvatarManager::GetPlayer2Avatar() const {
        return mpPlayer2Avatar;
    }

    void AvatarManager::ResetMultiPlayerSelection() {
        mpPlayer1Selection = 0;
        mpPlayer2Selection = 1;
        mpPlayer1Avatar = AVATAR_BLUE;
        mpPlayer2Avatar = AVATAR_GREEN;
        mpPlayer1Confirmed = false;
        mpPlayer2Confirmed = false;
    }

    // === Avatar-Informationen ===
    const AvatarInfo& AvatarManager::GetAvatarInfo(AvatarType type) const {
        return avatars[(int)type];
    }

    std::string AvatarManager::GetAvatarDescription(AvatarType type) const {
        return avatars[(int)type].description;
    }

    bool AvatarManager::IsAvatarUnlocked(AvatarType type) const {
        return avatars[(int)type].isUnlocked;
    }

    bool AvatarManager::CanAfford(AvatarType type, int coins) const {
        return coins >= avatars[(int)type].cost;
    }

    // === Avatar kaufen ===
    bool AvatarManager::PurchaseAvatar(AvatarType type, int& coins) {
        if (IsAvatarUnlocked(type)) return true;

        if (CanAfford(type, coins)) {
            coins -= avatars[(int)type].cost;
            avatars[(int)type].isUnlocked = true;
            SaveAvatarUnlocks();
            return true;
        }

        return false;
    }

    // === Private Hilfsfunktionen ===
    void AvatarManager::InitializeAvatarData() {
        avatars.resize(AVATAR_COUNT);

        // Blauer Avatar (kostenlos)
        avatars[AVATAR_BLUE] = {
            AVATAR_BLUE,
            "Blauer Avatar",
            "Standard Avatar - Keine besonderen Boni",
            0,
            true, // Immer freigeschaltet
            {}
        };

        // Grüner Avatar
        avatars[AVATAR_GREEN] = {
            AVATAR_GREEN,
            "Gruener Avatar",
            "+10% Lebenspunkte (+1 Leben)",
            GameConfig::AVATAR_COST,
            false,
            {}
        };

        // Roter Avatar
        avatars[AVATAR_RED] = {
            AVATAR_RED,
            "Roter Avatar",
            "+10% Projektilgeschwindigkeit",
            GameConfig::AVATAR_COST,
            false,
            {}
        };

        // Gelber Avatar
        avatars[AVATAR_YELLOW] = {
            AVATAR_YELLOW,
            "Gelber Avatar",
            "+10% Bewegungsgeschwindigkeit",
            GameConfig::AVATAR_COST,
            false,
            {}
        };

        // Lila Avatar
        avatars[AVATAR_PURPLE] = {
            AVATAR_PURPLE,
            "Lila Avatar",
            "+10% Schussfrequenz",
            GameConfig::AVATAR_COST,
            false,
            {}
        };
    }

    void AvatarManager::HandleSinglePlayerInput(int& coins) {
        auto navInput = inputManager.GetNavigationInput();

        switch (navInput) {
        case InputManager::NAV_LEFT:
            spCurrentSelection = (spCurrentSelection - 1 + AVATAR_COUNT) % AVATAR_COUNT;
            break;

        case InputManager::NAV_RIGHT:
            spCurrentSelection = (spCurrentSelection + 1) % AVATAR_COUNT;
            break;

        case InputManager::NAV_CONFIRM: {
            AvatarType selectedType = avatars[spCurrentSelection].type;

            if (IsAvatarUnlocked(selectedType)) {
                spSelectedAvatar = selectedType;
                spAvatarConfirmed = true;
            }
            else if (PurchaseAvatar(selectedType, coins)) {
                spSelectedAvatar = selectedType;
                spAvatarConfirmed = true;
            }
            else {
                spNotEnoughCoinsTimer = 2.0f;
            }
            break;
        }

        default:
            break;
        }

        // Maus-Input für Avatar-Karten
        for (int i = 0; i < AVATAR_COUNT; i++) {
            Rectangle avatarRect = GetAvatarRect(i, false);
            if (UpdateAnimatedButton(avatarRect, avatars[i].buttonState, WHITE, YELLOW, ORANGE, GetFrameTime())) {
                spCurrentSelection = i;
                AvatarType selectedType = avatars[i].type;

                if (IsAvatarUnlocked(selectedType)) {
                    spSelectedAvatar = selectedType;
                    spAvatarConfirmed = true;
                }
                else if (PurchaseAvatar(selectedType, coins)) {
                    spSelectedAvatar = selectedType;
                    spAvatarConfirmed = true;
                }
                else {
                    spNotEnoughCoinsTimer = 2.0f;
                }
            }
        }
    }

    void AvatarManager::HandleMultiPlayerInput() {
        if (!mpPlayer1Confirmed) {
            // Player 1 Input (A/D/W)
            if (IsKeyPressed(KEY_A)) {
                mpPlayer1Selection = (mpPlayer1Selection - 1 + AVATAR_COUNT) % AVATAR_COUNT;
                // Sicherstellen dass Player 1 nicht den gleichen Avatar wie Player 2 wählt
                if (mpPlayer1Selection == mpPlayer2Selection) {
                    mpPlayer1Selection = (mpPlayer1Selection - 1 + AVATAR_COUNT) % AVATAR_COUNT;
                }
            }
            if (IsKeyPressed(KEY_D)) {
                mpPlayer1Selection = (mpPlayer1Selection + 1) % AVATAR_COUNT;
                if (mpPlayer1Selection == mpPlayer2Selection) {
                    mpPlayer1Selection = (mpPlayer1Selection + 1) % AVATAR_COUNT;
                }
            }
            if (IsKeyPressed(KEY_W)) {
                mpPlayer1Avatar = avatars[mpPlayer1Selection].type;
                mpPlayer1Confirmed = true;
            }
        }
        else {
            // Player 2 Input (Pfeiltasten)
            if (IsKeyPressed(KEY_LEFT)) {
                mpPlayer2Selection = (mpPlayer2Selection - 1 + AVATAR_COUNT) % AVATAR_COUNT;
                if (mpPlayer2Selection == mpPlayer1Selection) {
                    mpPlayer2Selection = (mpPlayer2Selection - 1 + AVATAR_COUNT) % AVATAR_COUNT;
                }
            }
            if (IsKeyPressed(KEY_RIGHT)) {
                mpPlayer2Selection = (mpPlayer2Selection + 1) % AVATAR_COUNT;
                if (mpPlayer2Selection == mpPlayer1Selection) {
                    mpPlayer2Selection = (mpPlayer2Selection + 1) % AVATAR_COUNT;
                }
            }
            if (IsKeyPressed(KEY_UP)) {
                mpPlayer2Avatar = avatars[mpPlayer2Selection].type;
                mpPlayer2Confirmed = true;
            }
        }
    }

    void AvatarManager::RenderAvatarGrid(bool isMultiPlayer) {
        const float avatarSize = 120.0f;
        const float spacing = 160.0f;
        const float startX = (GameConfig::SCREEN_WIDTH - (AVATAR_COUNT * spacing - (spacing - avatarSize))) / 2;
        const float startY = 150.0f;

        auto& textureManager = TextureManager::GetInstance();

        for (int i = 0; i < AVATAR_COUNT; i++) {
            Rectangle avatarRect = {
                startX + i * spacing,
                startY,
                avatarSize,
                avatarSize
            };

            bool isSelected = isMultiPlayer ?
                ((!mpPlayer1Confirmed && i == mpPlayer1Selection) ||
                    (mpPlayer1Confirmed && i == mpPlayer2Selection)) :
                (i == spCurrentSelection);

            bool isHovered = inputManager.IsMouseOverRectangle(avatarRect);

            RenderAvatarCard(avatars[i], avatarRect, isSelected, isHovered, !mpPlayer1Confirmed);
        }
    }

    void AvatarManager::RenderAvatarCard(const AvatarInfo& avatar, Rectangle rect, bool isSelected, bool isHovered, bool isPlayer1) {
        auto& textureManager = TextureManager::GetInstance();

        // Rahmen-Farbe bestimmen
        Color frameColor = WHITE;
        float frameThickness = 2.0f;

        if (avatar.isUnlocked) {
            if (isSelected) {
                frameColor = isPlayer1 ? YELLOW : SKYBLUE;
                frameThickness = 4.0f;
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

        // Avatar-Textur
        Texture2D avatarTexture = textureManager.GetAvatarTexture(avatar.type);
        Color tint = avatar.isUnlocked ? WHITE : Fade(GRAY, 0.5f);
        TextureManager::DrawTextureScaled(avatarTexture, rect.x + 10, rect.y + 10, rect.width - 20, rect.height - 20, tint);

        // Rahmen
        DrawRectangleLinesEx(rect, frameThickness, frameColor);

        // Schloss-Symbol für gesperrte Avatare
        if (!avatar.isUnlocked) {
            DrawText("LOCKED", rect.x + 25, rect.y + rect.height - 30, 20, RED);

            // Kosten anzeigen
            auto& coinTexture = textureManager.GetCoinTexture();
            TextureManager::DrawTextureScaled(coinTexture, rect.x + 5, rect.y + 5, 20, 20);
            DrawText(TextFormat("%d", avatar.cost), rect.x + 30, rect.y + 5, 20, YELLOW);
        }

        // Name unter dem Avatar
        const char* name = avatar.name.c_str();
        int nameWidth = MeasureText(name, 16);
        DrawText(name, rect.x + (rect.width - nameWidth) / 2, rect.y + rect.height + 5, 16, WHITE);
    }

    void AvatarManager::RenderSelectionIndicators() {
        if (mpPlayer1Confirmed) {
            // Player 1 Auswahl anzeigen
            Rectangle p1Rect = GetAvatarRect(mpPlayer1Selection, true);
            DrawRectangleLinesEx(p1Rect, 4, YELLOW);
            DrawText("P1", p1Rect.x + p1Rect.width - 30, p1Rect.y + 5, 20, YELLOW);
        }

        if (mpPlayer2Confirmed) {
            // Player 2 Auswahl anzeigen
            Rectangle p2Rect = GetAvatarRect(mpPlayer2Selection, true);
            DrawRectangleLinesEx(p2Rect, 4, SKYBLUE);
            DrawText("P2", p2Rect.x + p2Rect.width - 30, p2Rect.y + 25, 20, SKYBLUE);
        }
    }

    Rectangle AvatarManager::GetAvatarRect(int index, bool isMultiPlayer) {
        const float avatarSize = 120.0f;
        const float spacing = 160.0f;
        const float startX = (GameConfig::SCREEN_WIDTH - (AVATAR_COUNT * spacing - (spacing - avatarSize))) / 2;
        const float startY = 150.0f;

        return {
            startX + index * spacing,
            startY,
            avatarSize,
            avatarSize
        };
    }

    void AvatarManager::UpdateButtonAnimations(float deltaTime) {
        for (auto& avatar : avatars) {
            // Animation für Hover-Effekte wird automatisch in UpdateAnimatedButton behandelt
            // Hier könnten zusätzliche Animationen hinzugefügt werden
        }
    }

} // namespace SpaceInvaders