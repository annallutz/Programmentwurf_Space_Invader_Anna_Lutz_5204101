#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include "InputManager.h"
#include "SaveSystem.h"
#include <vector>
#include <string>

namespace SpaceInvaders {

    /**
     * @brief Menü-Option Struktur
     */
    struct MenuOption {
        std::string text;
        Rectangle bounds;
        ButtonState buttonState;
        bool isEnabled;
    };

    /**
     * @brief MenuSystem - Verwaltet alle Menü-Bildschirme
     *
     * Zuständig für:
     * - Hauptmenü mit Maus+Tastatur Navigation
     * - Regeln-Bildschirm
     * - Statistiken-Bildschirm
     * - Schwierigkeitsauswahl
     * - Animierte Buttons und Hover-Effekte
     */
    class MenuSystem {
    public:
        // === Konstruktor & Destruktor ===
        MenuSystem();
        ~MenuSystem() = default;

        // === Initialisierung ===
        bool Initialize();

        // === Hauptmenü ===
        enum MainMenuResult {
            MAIN_SINGLEPLAYER,
            MAIN_MULTIPLAYER,
            MAIN_RULES,
            MAIN_STATISTICS,
            MAIN_DIFFICULTY,
            MAIN_EXIT,
            MAIN_SELECTING
        };

        MainMenuResult UpdateMainMenu(float deltaTime, int coins);
        void RenderMainMenu(int coins);

        // === Regeln ===
        bool UpdateRules(float deltaTime);
        void RenderRules();

        // === Statistiken ===
        bool UpdateStatistics(float deltaTime);
        void RenderStatistics();

        // === Schwierigkeitsauswahl ===
        enum DifficultyResult {
            DIFF_SELECTING,
            DIFF_CHOSEN,
            DIFF_BACK
        };

        DifficultyResult UpdateDifficultySelection(float deltaTime);
        void RenderDifficultySelection();
        Difficulty GetSelectedDifficulty() const;
        void ResetDifficultySelection();

        // === GameOver Screen ===
        bool UpdateGameOver(float deltaTime, int finalScore, bool isMultiplayer);
        void RenderGameOver(int finalScore, bool isMultiplayer, int coinsEarned);

        // === Pause Screen ===
        enum PauseResult {
            PAUSE_CONTINUE,
            PAUSE_MAIN_MENU,
            PAUSE_PAUSED
        };

        PauseResult UpdatePause(float deltaTime);
        void RenderPause();

    private:
        // === Input & Save System ===
        InputManager inputManager;
        SaveSystem* saveSystem;

        // === Hauptmenü State ===
        std::vector<MenuOption> mainMenuOptions;
        int mainMenuSelection;

        // === Schwierigkeits-State ===
        std::vector<MenuOption> difficultyOptions;
        int difficultySelection;
        Difficulty selectedDifficulty;

        // === Statistiken ===
        GameStatistics gameStats;
        std::vector<HighscoreEntry> highscores;
        int bestMultiplayerScore;

        // === Private Hilfsfunktionen ===
        void InitializeMainMenu();
        void InitializeDifficultyMenu();
        void UpdateMainMenuSelection(float deltaTime);
        void UpdateDifficultyMenuSelection(float deltaTime);

        // === Rendering Hilfsfunktionen ===
        void RenderMenuBackground();
        void RenderTitle(const std::string& title, int y = 80);
        void RenderMenuOptions(const std::vector<MenuOption>& options, int selectedIndex);
        void RenderCoinDisplay(int coins, int x, int y);
        void RenderStatisticsContent();
        void RenderHighscoresTable();
        void RenderGameStatsTable();

        // === Button Hilfsfunktionen ===
        bool HandleMenuOptionClick(std::vector<MenuOption>& options, int& selectedIndex);
        void UpdateMenuButtonAnimations(std::vector<MenuOption>& options, float deltaTime);
        Rectangle CalculateMenuOptionBounds(const std::string& text, int index, int totalOptions);

        // === Animationen ===
        float backgroundAnimation;
        float titlePulse;

        // === Text-Hilfsfunktionen ===
        std::string FormatPlayTime(float seconds);
        std::string GetDifficultyName(Difficulty diff);
        std::string GetAvatarName(AvatarType avatar);
        std::string GetModName(ModType mod);
    };

} // namespace SpaceInvaders