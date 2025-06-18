#pragma once

#include "GameConfig.h"
#include <vector>
#include <string>

namespace SpaceInvaders {

    /**
     * @brief Highscore-Eintrag Struktur
     */
    struct HighscoreEntry {
        std::string playerName;
        int score;
        AvatarType avatarUsed;
        ModType modUsed;
        Difficulty difficulty;
    };

    /**
     * @brief Spiel-Statistiken Struktur
     */
    struct GameStatistics {
        int totalGamesPlayed;
        int totalScore;
        int totalCoinsEarned;
        int totalEnemiesKilled;
        int totalBossesDefeated;
        float totalPlayTime; // in Sekunden

        // Avatar-Statistiken
        int avatarUsageCount[5]; // Wie oft jeder Avatar verwendet wurde

        // Schwierigkeit-Statistiken  
        int difficultyWins[3];   // Siege pro Schwierigkeitsgrad
        int difficultyPlays[3];  // Spiele pro Schwierigkeitsgrad
    };

    /**
     * @brief SaveSystem - Verwaltet alle Speicher-Operationen
     *
     * Zuständig für:
     * - Coins speichern/laden
     * - Highscores verwalten
     * - Statistiken sammeln und speichern
     * - Avatar/Mod-Freischaltungen
     */
    class SaveSystem {
    public:
        // === Konstruktor & Destruktor ===
        SaveSystem();
        ~SaveSystem() = default;

        // === Initialisierung ===
        bool Initialize();
        void CreateSaveDirectoryIfNeeded();

        // === Coins System ===
        int LoadCoins();
        void SaveCoins(int coins);

        // === Highscore System ===
        std::vector<HighscoreEntry> LoadHighscores();
        void SaveHighscore(const std::string& playerName, int score,
            AvatarType avatar, ModType mod, Difficulty difficulty);
        void SaveHighscores(const std::vector<HighscoreEntry>& scores);

        // === Multiplayer Bestscore ===
        int LoadBestMultiplayerScore();
        void SaveBestMultiplayerScore(int score);

        // === Statistiken ===
        GameStatistics LoadStatistics();
        void SaveStatistics(const GameStatistics& stats);
        void UpdateGameStatistics(int score, int coinsEarned, int enemiesKilled,
            bool bossDefeated, float playTime,
            AvatarType avatar, Difficulty difficulty);

        // === Hilfsfunktionen ===
        bool FileExists(const std::string& filename);
        void EnsureFileExists(const std::string& filename);

        // === Constants ===
        static const int MAX_HIGHSCORE_ENTRIES = 10;

    private:
        // === Private Hilfsfunktionen ===
        std::string GetSaveFilePath(const std::string& filename);
        void SortHighscores(std::vector<HighscoreEntry>& scores);
        void TrimHighscores(std::vector<HighscoreEntry>& scores);

        // === Parsing Hilfsfunktionen ===
        HighscoreEntry ParseHighscoreEntry(const std::string& line);
        std::string FormatHighscoreEntry(const HighscoreEntry& entry);

        AvatarType StringToAvatarType(const std::string& str);
        ModType StringToModType(const std::string& str);
        Difficulty StringToDifficulty(const std::string& str);

        std::string AvatarTypeToString(AvatarType type);
        std::string ModTypeToString(ModType type);
        std::string DifficultyToString(Difficulty diff);
    };

} // namespace SpaceInvaders