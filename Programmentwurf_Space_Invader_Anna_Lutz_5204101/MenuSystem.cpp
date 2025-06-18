#include "MenuSystem.h"
#include "TextureManager.h"
#include <sstream>
#include <iomanip>

namespace SpaceInvaders {

    // === Konstruktor ===
    MenuSystem::MenuSystem()
        : saveSystem(nullptr)
        , mainMenuSelection(0)
        , difficultySelection(1) // Medium als Standard
        , selectedDifficulty(DIFF_MEDIUM)
        , backgroundAnimation(0.0f)
        , titlePulse(0.0f)
        , bestMultiplayerScore(0)
    {
    }

    // === Initialisierung ===
    bool MenuSystem::Initialize() {
        InitializeMainMenu();
        InitializeDifficultyMenu();

        // SaveSystem-Referenz setzen (wird von Game übergeben)
        // saveSystem wird später von außen gesetzt

        return true;
    }

    // === Hauptmenü ===
    MenuSystem::MainMenuResult MenuSystem::UpdateMainMenu(float deltaTime, int coins) {
        backgroundAnimation += deltaTime;
        titlePulse += deltaTime;

        UpdateMainMenuSelection(deltaTime);

        // Überprüfen ob eine Option ausgewählt wurde
        if (inputManager.GetNavigationInput() == InputManager::NAV_CONFIRM) {
            switch (mainMenuSelection) {
            case 0: return MAIN_SINGLEPLAYER;
            case 1: return MAIN_MULTIPLAYER;
            case 2: return MAIN_RULES;
            case 3: return MAIN_STATISTICS;
            case 4: return MAIN_DIFFICULTY;
            case 5: return MAIN_EXIT;
            }
        }

        // Maus-Klicks prüfen
        int clickedOption = -1;
        if (HandleMenuOptionClick(mainMenuOptions, clickedOption)) {
            switch (clickedOption) {
            case 0: return MAIN_SINGLEPLAYER;
            case 1: return MAIN_MULTIPLAYER;
            case 2: return MAIN_RULES;
            case 3: return MAIN_STATISTICS;
            case 4: return MAIN_DIFFICULTY;
            case 5: return MAIN_EXIT;
            }
        }

        return MAIN_SELECTING;
    }

    void MenuSystem::RenderMainMenu(int coins) {
        BeginDrawing();
        ClearBackground(BLACK);

        RenderMenuBackground();

        // Animierter Titel
        float pulseScale = 1.0f + 0.05f * sinf(titlePulse * 2.0f);
        const char* title = "SPACE INVADERS";
        int titleSize = (int)(50 * pulseScale);
        int titleWidth = MeasureText(title, titleSize);
        Color titleColor = {
            (unsigned char)(200 + 55 * sinf(titlePulse)),
            (unsigned char)(150 + 105 * sinf(titlePulse * 1.5f)),
            255,
            255
        };
        DrawText(title, (GameConfig::SCREEN_WIDTH - titleWidth) / 2, 80, titleSize, titleColor);

        // Coins anzeigen
        RenderCoinDisplay(coins, GameConfig::SCREEN_WIDTH - 200, 30);

        // Menü-Optionen
        RenderMenuOptions(mainMenuOptions, mainMenuSelection);

        // Untertitel/Version
        const char* subtitle = "v1.0 - Erstellt mit Raylib";
        int subtitleWidth = MeasureText(subtitle, 16);
        DrawText(subtitle, (GameConfig::SCREEN_WIDTH - subtitleWidth) / 2,
            GameConfig::SCREEN_HEIGHT - 40, 16, GRAY);

        EndDrawing();
    }

    // === Regeln ===
    bool MenuSystem::UpdateRules(float deltaTime) {
        return inputManager.GetNavigationInput() == InputManager::NAV_CONFIRM ||
            inputManager.GetNavigationInput() == InputManager::NAV_BACK ||
            inputManager.IsMouseClicked();
    }

    void MenuSystem::RenderRules() {
        BeginDrawing();
        ClearBackground(BLACK);

        RenderMenuBackground();
        RenderTitle("SPIELREGELN");

        // Regel-Text
        std::vector<std::string> rules = {
            "ZIEL:",
            "• Schuetze alle Gegner ab und ueberlebe so lange wie moeglich",
            "• Sammle Coins um neue Avatare und Mods freizuschalten",
            "",
            "STEUERUNG SINGLEPLAYER:",
            "• Pfeiltasten Links/Rechts: Bewegen",
            "• Pfeil Hoch: Schiessen",
            "• P: Pause",
            "",
            "STEUERUNG MULTIPLAYER:",
            "• Player 1: A/D bewegen, W schiessen",
            "• Player 2: Pfeiltasten bewegen, Pfeil Hoch schiessen",
            "",
            "AVATAR-BONI:",
            "• Blau: Keine Boni (kostenlos)",
            "• Gruen: +1 Leben",
            "• Rot: +10% Projektilgeschwindigkeit",
            "• Gelb: +10% Bewegungsgeschwindigkeit",
            "• Lila: +10% Schussfrequenz",
            "",
            "POWER-UPS:",
            "• Schild: Schuetzt vor Treffern",
            "• Triple-Shot: Schiesst 3 Projektile",
            "• Zeitlupe: Verlangsamt Gegner"
        };

        int startY = 150;
        for (size_t i = 0; i < rules.size(); i++) {
            Color textColor = WHITE;
            int fontSize = 18;

            // Überschriften hervorheben
            if (rules[i].find(":") != std::string::npos && rules[i][0] != '•') {
                textColor = YELLOW;
                fontSize = 22;
            }

            DrawText(rules[i].c_str(), 100, startY + i * 25, fontSize, textColor);
        }

        // Zurück-Hinweis
        const char* backText = "ENTER oder Mausklick zum Zurueckkehren";
        int backWidth = MeasureText(backText, 20);
        DrawText(backText, (GameConfig::SCREEN_WIDTH - backWidth) / 2,
            GameConfig::SCREEN_HEIGHT - 80, 20, ORANGE);

        EndDrawing();
    }

    // === Statistiken ===
    bool MenuSystem::UpdateStatistics(float deltaTime) {
        // Statistiken laden (falls noch nicht geschehen)
        if (saveSystem) {
            gameStats = saveSystem->LoadStatistics();
            highscores = saveSystem->LoadHighscores();
            bestMultiplayerScore = saveSystem->LoadBestMultiplayerScore();
        }

        return inputManager.GetNavigationInput() == InputManager::NAV_CONFIRM ||
            inputManager.GetNavigationInput() == InputManager::NAV_BACK ||
            inputManager.IsMouseClicked();
    }

    void MenuSystem::RenderStatistics() {
        BeginDrawing();
        ClearBackground(BLACK);

        RenderMenuBackground();
        RenderTitle("STATISTIKEN");

        RenderStatisticsContent();

        // Zurück-Hinweis
        const char* backText = "ENTER oder Mausklick zum Zurueckkehren";
        int backWidth = MeasureText(backText, 20);
        DrawText(backText, (GameConfig::SCREEN_WIDTH - backWidth) / 2,
            GameConfig::SCREEN_HEIGHT - 60, 20, ORANGE);

        EndDrawing();
    }

    // === Schwierigkeitsauswahl ===
    MenuSystem::DifficultyResult MenuSystem::UpdateDifficultySelection(float deltaTime) {
        UpdateDifficultyMenuSelection(deltaTime);

        if (inputManager.GetNavigationInput() == InputManager::NAV_CONFIRM) {
            selectedDifficulty = (Difficulty)difficultySelection;
            return DIFF_CHOSEN;
        }

        if (inputManager.GetNavigationInput() == InputManager::NAV_BACK) {
            return DIFF_BACK;
        }

        // Maus-Klicks prüfen
        int clickedOption = -1;
        if (HandleMenuOptionClick(difficultyOptions, clickedOption)) {
            difficultySelection = clickedOption;
            selectedDifficulty = (Difficulty)clickedOption;
            return DIFF_CHOSEN;
        }

        return DIFF_SELECTING;
    }

    void MenuSystem::RenderDifficultySelection() {
        BeginDrawing();
        ClearBackground(BLACK);

        RenderMenuBackground();
        RenderTitle("SCHWIERIGKEIT WAEHLEN");

        RenderMenuOptions(difficultyOptions, difficultySelection);

        // Schwierigkeits-Beschreibungen
        Rectangle descBox = { 200, 400, GameConfig::SCREEN_WIDTH - 400, 150 };
        DrawRectangleRec(descBox, Fade(DARKGRAY, 0.8f));
        DrawRectangleLinesEx(descBox, 2, WHITE);

        std::string diffName = GetDifficultyName((Difficulty)difficultySelection);
        std::vector<std::string> descriptions = {
            "LEICHT: Weniger Gegner, langsamere Geschwindigkeit",
            "MITTEL: Ausgewogenes Gameplay",
            "SCHWER: Viele Gegner, hohe Geschwindigkeit"
        };

        DrawText(diffName.c_str(), descBox.x + 20, descBox.y + 20, 24, YELLOW);
        DrawText(descriptions[difficultySelection].c_str(), descBox.x + 20, descBox.y + 50, 18, WHITE);

        // Zurück-Hinweis
        const char* backText = "ENTER: Bestaetigen | ESC: Zurueck";
        int backWidth = MeasureText(backText, 20);
        DrawText(backText, (GameConfig::SCREEN_WIDTH - backWidth) / 2,
            GameConfig::SCREEN_HEIGHT - 80, 20, GRAY);

        EndDrawing();
    }

    Difficulty MenuSystem::GetSelectedDifficulty() const {
        return selectedDifficulty;
    }

    void MenuSystem::ResetDifficultySelection() {
        difficultySelection = 1; // Medium als Standard
        selectedDifficulty = DIFF_MEDIUM;
    }

    // === GameOver Screen ===
    bool MenuSystem::UpdateGameOver(float deltaTime, int finalScore, bool isMultiplayer) {
        return inputManager.GetNavigationInput() == InputManager::NAV_CONFIRM ||
            inputManager.IsMouseClicked();
    }

    void MenuSystem::RenderGameOver(int finalScore, bool isMultiplayer, int coinsEarned) {
        BeginDrawing();
        ClearBackground(BLACK);

        RenderMenuBackground();

        // GAME OVER Titel
        const char* gameOverText = "GAME OVER";
        int titleSize = 60;
        int titleWidth = MeasureText(gameOverText, titleSize);
        DrawText(gameOverText, (GameConfig::SCREEN_WIDTH - titleWidth) / 2, 200, titleSize, RED);

        // Score anzeigen
        std::string scoreText = isMultiplayer ?
            "Kombinierter Score: " + std::to_string(finalScore) :
            "Dein Score: " + std::to_string(finalScore);
        int scoreWidth = MeasureText(scoreText.c_str(), 30);
        DrawText(scoreText.c_str(), (GameConfig::SCREEN_WIDTH - scoreWidth) / 2, 300, 30, WHITE);

        // Coins earned (nur im Singleplayer)
        if (!isMultiplayer && coinsEarned > 0) {
            std::string coinsText = "+" + std::to_string(coinsEarned) + " Coins erhalten!";
            int coinsWidth = MeasureText(coinsText.c_str(), 25);
            DrawText(coinsText.c_str(), (GameConfig::SCREEN_WIDTH - coinsWidth) / 2, 350, 25, YELLOW);
        }

        // Zurück-Hinweis
        const char* backText = "ENTER zum Hauptmenue";
        int backWidth = MeasureText(backText, 20);
        DrawText(backText, (GameConfig::SCREEN_WIDTH - backWidth) / 2,
            GameConfig::SCREEN_HEIGHT - 100, 20, ORANGE);

        EndDrawing();
    }

    // === Pause Screen ===
    MenuSystem::PauseResult MenuSystem::UpdatePause(float deltaTime) {
        if (inputManager.IsPausePressed()) {
            return PAUSE_CONTINUE;
        }

        if (IsKeyPressed(KEY_M)) {
            return PAUSE_MAIN_MENU;
        }

        return PAUSE_PAUSED;
    }

    void MenuSystem::RenderPause() {
        // Spiel im Hintergrund leicht abdunkeln
        DrawRectangle(0, 0, GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT, Fade(BLACK, 0.5f));

        // Pause-Box
        Rectangle pauseBox = {
            GameConfig::SCREEN_WIDTH / 2 - 200,
            GameConfig::SCREEN_HEIGHT / 2 - 150,
            400,
            300
        };
        DrawRectangleRec(pauseBox, Fade(DARKGRAY, 0.9f));
        DrawRectangleLinesEx(pauseBox, 3, WHITE);

        // PAUSE Titel
        const char* pauseText = "PAUSE";
        int titleWidth = MeasureText(pauseText, 40);
        DrawText(pauseText, pauseBox.x + (pauseBox.width - titleWidth) / 2,
            pauseBox.y + 50, 40, YELLOW);

        // Optionen
        DrawText("P - Spiel fortsetzen", pauseBox.x + 50, pauseBox.y + 120, 20, WHITE);
        DrawText("M - Zum Hauptmenue", pauseBox.x + 50, pauseBox.y + 150, 20, WHITE);
        DrawText("ESC - Spiel beenden", pauseBox.x + 50, pauseBox.y + 180, 20, WHITE);
    }

    // === Private Hilfsfunktionen ===
    void MenuSystem::InitializeMainMenu() {
        std::vector<std::string> optionTexts = {
            "1 Spieler Modus",
            "2 Spieler Modus",
            "Regeln",
            "Statistiken",
            "Schwierigkeit",
            "Beenden"
        };

        mainMenuOptions.clear();
        for (size_t i = 0; i < optionTexts.size(); i++) {
            MenuOption option;
            option.text = optionTexts[i];
            option.bounds = CalculateMenuOptionBounds(option.text, i, optionTexts.size());
            option.isEnabled = true;
            mainMenuOptions.push_back(option);
        }
    }

    void MenuSystem::InitializeDifficultyMenu() {
        std::vector<std::string> optionTexts = {
            "LEICHT",
            "MITTEL",
            "SCHWER"
        };

        difficultyOptions.clear();
        for (size_t i = 0; i < optionTexts.size(); i++) {
            MenuOption option;
            option.text = optionTexts[i];
            option.bounds = CalculateMenuOptionBounds(option.text, i, optionTexts.size());
            option.isEnabled = true;
            difficultyOptions.push_back(option);
        }
    }

    void MenuSystem::UpdateMainMenuSelection(float deltaTime) {
        UpdateMenuButtonAnimations(mainMenuOptions, deltaTime);

        auto navInput = inputManager.GetNavigationInput();
        if (navInput == InputManager::NAV_UP) {
            mainMenuSelection = (mainMenuSelection - 1 + mainMenuOptions.size()) % mainMenuOptions.size();
        }
        else if (navInput == InputManager::NAV_DOWN) {
            mainMenuSelection = (mainMenuSelection + 1) % mainMenuOptions.size();
        }
    }

    void MenuSystem::UpdateDifficultyMenuSelection(float deltaTime) {
        UpdateMenuButtonAnimations(difficultyOptions, deltaTime);

        auto navInput = inputManager.GetNavigationInput();
        if (navInput == InputManager::NAV_UP) {
            difficultySelection = (difficultySelection - 1 + difficultyOptions.size()) % difficultyOptions.size();
        }
        else if (navInput == InputManager::NAV_DOWN) {
            difficultySelection = (difficultySelection + 1) % difficultyOptions.size();
        }
    }

    // === Rendering Hilfsfunktionen ===
    void MenuSystem::RenderMenuBackground() {
        // Animierter Sterne-Hintergrund
        for (int i = 0; i < 100; i++) {
            float x = (i * 123.4f) * 0.01f; // Pseudo-Zufallsverteilung
            x = fmod(x, 1.0f) * GameConfig::SCREEN_WIDTH;

            float y = (i * 456.7f) * 0.01f;
            y = fmod(y + backgroundAnimation * 0.1f, 1.0f) * GameConfig::SCREEN_HEIGHT;

            float brightness = 0.3f + 0.7f * sin(backgroundAnimation + i);
            Color starColor = { 255, 255, 255, (unsigned char)(brightness * 100) };

            DrawPixel((int)x, (int)y, starColor);
        }
    }

    void MenuSystem::RenderTitle(const std::string& title, int y) {
        int titleWidth = MeasureText(title.c_str(), 40);
        DrawText(title.c_str(), (GameConfig::SCREEN_WIDTH - titleWidth) / 2, y, 40, YELLOW);
    }

    void MenuSystem::RenderMenuOptions(const std::vector<MenuOption>& options, int selectedIndex) {
        for (size_t i = 0; i < options.size(); i++) {
            const MenuOption& option = options[i];

            Color textColor = WHITE;
            int fontSize = 30;

            // Ausgewählte Option hervorheben
            if ((int)i == selectedIndex) {
                textColor = YELLOW;
                fontSize = 32;

                // Leuchtender Hintergrund
                DrawRectangleRec(option.bounds, Fade(YELLOW, 0.2f));
            }

            // Hover-Effekt für Maus
            if (inputManager.IsMouseOverRectangle(option.bounds)) {
                textColor = ORANGE;
                DrawRectangleRec(option.bounds, Fade(ORANGE, 0.1f));
            }

            // Text zentriert rendern
            int textWidth = MeasureText(option.text.c_str(), fontSize);
            int textX = option.bounds.x + (option.bounds.width - textWidth) / 2;
            int textY = option.bounds.y + (option.bounds.height - fontSize) / 2;

            DrawText(option.text.c_str(), textX, textY, fontSize, textColor);
        }
    }

    void MenuSystem::RenderCoinDisplay(int coins, int x, int y) {
        auto& textureManager = TextureManager::GetInstance();
        auto& coinTexture = textureManager.GetCoinTexture();

        int fontSize = 30;
        const char* coinText = TextFormat("%d", coins);
        int textWidth = MeasureText(coinText, fontSize);

        DrawText(coinText, x, y, fontSize, YELLOW);
        TextureManager::DrawTextureScaled(coinTexture,
            x + textWidth + 10, y - 3,
            fontSize + 6, fontSize + 6);
    }

    void MenuSystem::RenderStatisticsContent() {
        // Zwei Spalten: Highscores links, Statistiken rechts
        int leftColumnX = 50;
        int rightColumnX = GameConfig::SCREEN_WIDTH / 2 + 50;
        int startY = 150;

        // Highscores Tabelle
        DrawText("HIGHSCORES (Top 5):", leftColumnX, startY, 24, YELLOW);
        RenderHighscoresTable();

        // Game Statistiken
        DrawText("SPIEL-STATISTIKEN:", rightColumnX, startY, 24, YELLOW);
        RenderGameStatsTable();

        // Multiplayer Bestscore
        DrawText("MULTIPLAYER BESTSCORE:", leftColumnX, 450, 20, SKYBLUE);
        DrawText(TextFormat("%d", bestMultiplayerScore), leftColumnX, 480, 24, WHITE);
    }

    void MenuSystem::RenderHighscoresTable() {
        int x = 50;
        int y = 180;

        if (highscores.empty()) {
            DrawText("Noch keine Highscores vorhanden", x, y, 18, GRAY);
            return;
        }

        for (size_t i = 0; i < std::min(highscores.size(), (size_t)5); i++) {
            const auto& entry = highscores[i];

            // Rang
            DrawText(TextFormat("%d.", (int)(i + 1)), x, y + i * 30, 20, YELLOW);

            // Name
            DrawText(entry.playerName.c_str(), x + 30, y + i * 30, 20, WHITE);

            // Score
            DrawText(TextFormat("%d", entry.score), x + 200, y + i * 30, 20, GREEN);

            // Avatar & Schwierigkeit
            std::string details = GetAvatarName(entry.avatarUsed) + " / " + GetDifficultyName(entry.difficulty);
            DrawText(details.c_str(), x + 300, y + i * 30, 16, GRAY);
        }
    }

    void MenuSystem::RenderGameStatsTable() {
        int x = GameConfig::SCREEN_WIDTH / 2 + 50;
        int y = 180;
        int lineHeight = 25;
        int currentY = y;

        // Allgemeine Statistiken
        DrawText(TextFormat("Spiele gespielt: %d", gameStats.totalGamesPlayed), x, currentY, 18, WHITE);
        currentY += lineHeight;

        DrawText(TextFormat("Gesamt-Score: %d", gameStats.totalScore), x, currentY, 18, WHITE);
        currentY += lineHeight;

        DrawText(TextFormat("Coins verdient: %d", gameStats.totalCoinsEarned), x, currentY, 18, WHITE);
        currentY += lineHeight;

        DrawText(TextFormat("Gegner getoetet: %d", gameStats.totalEnemiesKilled), x, currentY, 18, WHITE);
        currentY += lineHeight;

        DrawText(TextFormat("Bosse besiegt: %d", gameStats.totalBossesDefeated), x, currentY, 18, WHITE);
        currentY += lineHeight;

        DrawText(TextFormat("Spielzeit: %s", FormatPlayTime(gameStats.totalPlayTime).c_str()), x, currentY, 18, WHITE);
        currentY += lineHeight + 20;

        // Avatar-Nutzung
        DrawText("AVATAR-NUTZUNG:", x, currentY, 20, YELLOW);
        currentY += 30;

        std::vector<std::string> avatarNames = { "Blau", "Gruen", "Rot", "Gelb", "Lila" };
        for (int i = 0; i < 5; i++) {
            DrawText(TextFormat("%s: %dx", avatarNames[i].c_str(), gameStats.avatarUsageCount[i]),
                x, currentY, 16, WHITE);
            currentY += 20;
        }
    }

    // === Button Hilfsfunktionen ===
    bool MenuSystem::HandleMenuOptionClick(std::vector<MenuOption>& options, int& selectedIndex) {
        for (size_t i = 0; i < options.size(); i++) {
            if (inputManager.IsMouseOverRectangle(options[i].bounds)) {
                selectedIndex = (int)i;
                if (inputManager.IsMouseClicked()) {
                    return true;
                }
            }
        }
        return false;
    }

    void MenuSystem::UpdateMenuButtonAnimations(std::vector<MenuOption>& options, float deltaTime) {
        for (auto& option : options) {
            bool isHovered = inputManager.IsMouseOverRectangle(option.bounds);
            UpdateAnimatedButton(option.bounds, option.buttonState, WHITE, YELLOW, ORANGE, deltaTime);
        }
    }

    Rectangle MenuSystem::CalculateMenuOptionBounds(const std::string& text, int index, int totalOptions) {
        int fontSize = 30;
        int textWidth = MeasureText(text.c_str(), fontSize);
        int textHeight = fontSize;

        float spacing = 60.0f;
        float totalHeight = totalOptions * spacing;
        float startY = (GameConfig::SCREEN_HEIGHT - totalHeight) / 2 + 100;

        return {
            (GameConfig::SCREEN_WIDTH - textWidth) / 2.0f - 20,
            startY + index * spacing,
            (float)textWidth + 40,
            (float)textHeight + 10
        };
    }

    // === Text-Hilfsfunktionen ===
    std::string MenuSystem::FormatPlayTime(float seconds) {
        int hours = (int)(seconds / 3600);
        int minutes = (int)((seconds - hours * 3600) / 60);
        int secs = (int)(seconds - hours * 3600 - minutes * 60);

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hours << ":"
            << std::setfill('0') << std::setw(2) << minutes << ":"
            << std::setfill('0') << std::setw(2) << secs;
        return oss.str();
    }

    std::string MenuSystem::GetDifficultyName(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY: return "Leicht";
        case DIFF_MEDIUM: return "Mittel";
        case DIFF_HARD: return "Schwer";
        default: return "Unbekannt";
        }
    }

    std::string MenuSystem::GetAvatarName(AvatarType avatar) {
        switch (avatar) {
        case AVATAR_BLUE: return "Blau";
        case AVATAR_GREEN: return "Gruen";
        case AVATAR_RED: return "Rot";
        case AVATAR_YELLOW: return "Gelb";
        case AVATAR_PURPLE: return "Lila";
        default: return "Unbekannt";
        }
    }

    std::string MenuSystem::GetModName(ModType mod) {
        switch (mod) {
        case MOD_NONE: return "Kein Mod";
        case MOD_FASTFIRE: return "Schnellfeuer";
        case MOD_SHIELD: return "Schild";
        case MOD_TRIPLESHOT: return "Triple-Shot";
        case MOD_SLOWMO: return "Zeitlupe";
        default: return "Unbekannt";
        }
    }

} // namespace SpaceInvaders