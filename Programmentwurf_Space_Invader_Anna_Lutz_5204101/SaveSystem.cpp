#include "SaveSystem.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <filesystem>

namespace SpaceInvaders {

    // === Konstruktor ===
    SaveSystem::SaveSystem() {
    }

    // === Initialisierung ===
    bool SaveSystem::Initialize() {
        CreateSaveDirectoryIfNeeded();

        // Alle benötigten Dateien erstellen falls sie nicht existieren
        EnsureFileExists(GameConfig::COINS_FILE);
        EnsureFileExists(GameConfig::HIGHSCORE_FILE);
        EnsureFileExists(GameConfig::STATISTICS_FILE);
        EnsureFileExists(GameConfig::AVATAR_UNLOCKS_FILE);
        EnsureFileExists(GameConfig::MOD_UNLOCKS_FILE);

        return true;
    }

    void SaveSystem::CreateSaveDirectoryIfNeeded() {
        try {
            if (!std::filesystem::exists(GameConfig::SAVE_FOLDER)) {
                std::filesystem::create_directories(GameConfig::SAVE_FOLDER);
            }
        }
        catch (const std::exception& e) {
            std::cerr << "[FEHLER] Konnte Save-Ordner nicht erstellen: " << e.what() << std::endl;
        }
    }

    // === Coins System ===
    int SaveSystem::LoadCoins() {
        std::ifstream file(GameConfig::COINS_FILE);
        int coins = 0;

        if (file.is_open()) {
            file >> coins;
            file.close();
        }

        return coins;
    }

    void SaveSystem::SaveCoins(int coins) {
        std::ofstream file(GameConfig::COINS_FILE);
        if (file.is_open()) {
            file << coins << std::endl;
            file.close();
        }
        else {
            std::cerr << "[FEHLER] Konnte Coins nicht speichern!" << std::endl;
        }
    }

    // === Highscore System ===
    std::vector<HighscoreEntry> SaveSystem::LoadHighscores() {
        std::vector<HighscoreEntry> scores;
        std::ifstream file(GameConfig::HIGHSCORE_FILE);

        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                if (!line.empty()) {
                    HighscoreEntry entry = ParseHighscoreEntry(line);
                    if (!entry.playerName.empty()) {
                        scores.push_back(entry);
                    }
                }
            }
            file.close();
        }

        SortHighscores(scores);
        return scores;
    }

    void SaveSystem::SaveHighscore(const std::string& playerName, int score,
        AvatarType avatar, ModType mod, Difficulty difficulty) {
        // Bestehende Highscores laden
        std::vector<HighscoreEntry> scores = LoadHighscores();

        // Neuen Eintrag hinzufügen
        HighscoreEntry newEntry;
        newEntry.playerName = playerName;
        newEntry.score = score;
        newEntry.avatarUsed = avatar;
        newEntry.modUsed = mod;
        newEntry.difficulty = difficulty;

        scores.push_back(newEntry);

        // Sortieren und auf Maximum begrenzen
        SortHighscores(scores);
        TrimHighscores(scores);

        // Speichern
        SaveHighscores(scores);
    }

    void SaveSystem::SaveHighscores(const std::vector<HighscoreEntry>& scores) {
        std::ofstream file(GameConfig::HIGHSCORE_FILE);
        if (file.is_open()) {
            for (const auto& entry : scores) {
                file << FormatHighscoreEntry(entry) << std::endl;
            }
            file.close();
        }
        else {
            std::cerr << "[FEHLER] Konnte Highscores nicht speichern!" << std::endl;
        }
    }

    // === Multiplayer Bestscore ===
    int SaveSystem::LoadBestMultiplayerScore() {
        std::ifstream file("save/bestscore_multiplayer.txt");
        int score = 0;

        if (file.is_open()) {
            file >> score;
            file.close();
        }

        return score;
    }

    void SaveSystem::SaveBestMultiplayerScore(int score) {
        std::ofstream file("save/bestscore_multiplayer.txt");
        if (file.is_open()) {
            file << score << std::endl;
            file.close();
        }
        else {
            std::cerr << "[FEHLER] Konnte Multiplayer-Bestscore nicht speichern!" << std::endl;
        }
    }

    // === Statistiken ===
    GameStatistics SaveSystem::LoadStatistics() {
        GameStatistics stats = {}; // Alles auf 0 initialisieren

        std::ifstream file(GameConfig::STATISTICS_FILE);
        if (file.is_open()) {
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string key;
                if (std::getline(iss, key, '=')) {
                    std::string value;
                    if (std::getline(iss, value)) {
                        // Verschiedene Statistik-Werte parsen
                        if (key == "totalGamesPlayed") stats.totalGamesPlayed = std::stoi(value);
                        else if (key == "totalScore") stats.totalScore = std::stoi(value);
                        else if (key == "totalCoinsEarned") stats.totalCoinsEarned = std::stoi(value);
                        else if (key == "totalEnemiesKilled") stats.totalEnemiesKilled = std::stoi(value);
                        else if (key == "totalBossesDefeated") stats.totalBossesDefeated = std::stoi(value);
                        else if (key == "totalPlayTime") stats.totalPlayTime = std::stof(value);
                        // Avatar-Nutzung
                        else if (key.substr(0, 11) == "avatarUsage") {
                            int index = std::stoi(key.substr(11));
                            if (index >= 0 && index < 5) {
                                stats.avatarUsageCount[index] = std::stoi(value);
                            }
                        }
                        // Schwierigkeits-Statistiken
                        else if (key.substr(0, 13) == "difficultyWin") {
                            int index = std::stoi(key.substr(13));
                            if (index >= 0 && index < 3) {
                                stats.difficultyWins[index] = std::stoi(value);
                            }
                        }
                        else if (key.substr(0, 14) == "difficultyPlay") {
                            int index = std::stoi(key.substr(14));
                            if (index >= 0 && index < 3) {
                                stats.difficultyPlays[index] = std::stoi(value);
                            }
                        }
                    }
                }
            }
            file.close();
        }

        return stats;
    }

    void SaveSystem::SaveStatistics(const GameStatistics& stats) {
        std::ofstream file(GameConfig::STATISTICS_FILE);
        if (file.is_open()) {
            file << "totalGamesPlayed=" << stats.totalGamesPlayed << std::endl;
            file << "totalScore=" << stats.totalScore << std::endl;
            file << "totalCoinsEarned=" << stats.totalCoinsEarned << std::endl;
            file << "totalEnemiesKilled=" << stats.totalEnemiesKilled << std::endl;
            file << "totalBossesDefeated=" << stats.totalBossesDefeated << std::endl;
            file << "totalPlayTime=" << stats.totalPlayTime << std::endl;

            // Avatar-Nutzung
            for (int i = 0; i < 5; i++) {
                file << "avatarUsage" << i << "=" << stats.avatarUsageCount[i] << std::endl;
            }

            // Schwierigkeits-Statistiken
            for (int i = 0; i < 3; i++) {
                file << "difficultyWin" << i << "=" << stats.difficultyWins[i] << std::endl;
                file << "difficultyPlay" << i << "=" << stats.difficultyPlays[i] << std::endl;
            }

            file.close();
        }
        else {
            std::cerr << "[FEHLER] Konnte Statistiken nicht speichern!" << std::endl;
        }
    }

    void SaveSystem::UpdateGameStatistics(int score, int coinsEarned, int enemiesKilled,
        bool bossDefeated, float playTime,
        AvatarType avatar, Difficulty difficulty) {
        GameStatistics stats = LoadStatistics();

        // Statistiken aktualisieren
        stats.totalGamesPlayed++;
        stats.totalScore += score;
        stats.totalCoinsEarned += coinsEarned;
        stats.totalEnemiesKilled += enemiesKilled;
        if (bossDefeated) stats.totalBossesDefeated++;
        stats.totalPlayTime += playTime;

        // Avatar-Nutzung
        if ((int)avatar >= 0 && (int)avatar < 5) {
            stats.avatarUsageCount[(int)avatar]++;
        }

        // Schwierigkeits-Statistiken
        if ((int)difficulty >= 0 && (int)difficulty < 3) {
            stats.difficultyPlays[(int)difficulty]++;
            // Spiel gilt als "gewonnen" wenn Boss besiegt wurde
            if (bossDefeated) {
                stats.difficultyWins[(int)difficulty]++;
            }
        }

        SaveStatistics(stats);
    }

    // === Hilfsfunktionen ===
    bool SaveSystem::FileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }

    void SaveSystem::EnsureFileExists(const std::string& filename) {
        if (!FileExists(filename)) {
            std::ofstream file(filename);
            file.close();
        }
    }

    // === Private Hilfsfunktionen ===
    std::string SaveSystem::GetSaveFilePath(const std::string& filename) {
        return std::string(GameConfig::SAVE_FOLDER) + filename;
    }

    void SaveSystem::SortHighscores(std::vector<HighscoreEntry>& scores) {
        std::sort(scores.begin(), scores.end(), [](const HighscoreEntry& a, const HighscoreEntry& b) {
            return a.score > b.score; // Absteigend sortieren
            });
    }

    void SaveSystem::TrimHighscores(std::vector<HighscoreEntry>& scores) {
        if (scores.size() > MAX_HIGHSCORE_ENTRIES) {
            scores.resize(MAX_HIGHSCORE_ENTRIES);
        }
    }

    // === Parsing Hilfsfunktionen ===
    HighscoreEntry SaveSystem::ParseHighscoreEntry(const std::string& line) {
        HighscoreEntry entry;
        std::istringstream iss(line);
        std::string token;

        // Format: Name,Score,Avatar,Mod,Difficulty
        if (std::getline(iss, token, ',')) entry.playerName = token;
        if (std::getline(iss, token, ',')) entry.score = std::stoi(token);
        if (std::getline(iss, token, ',')) entry.avatarUsed = StringToAvatarType(token);
        if (std::getline(iss, token, ',')) entry.modUsed = StringToModType(token);
        if (std::getline(iss, token, ',')) entry.difficulty = StringToDifficulty(token);

        return entry;
    }

    std::string SaveSystem::FormatHighscoreEntry(const HighscoreEntry& entry) {
        return entry.playerName + "," +
            std::to_string(entry.score) + "," +
            AvatarTypeToString(entry.avatarUsed) + "," +
            ModTypeToString(entry.modUsed) + "," +
            DifficultyToString(entry.difficulty);
    }

    // === String-Konvertierungen ===
    AvatarType SaveSystem::StringToAvatarType(const std::string& str) {
        if (str == "BLUE") return AVATAR_BLUE;
        if (str == "GREEN") return AVATAR_GREEN;
        if (str == "RED") return AVATAR_RED;
        if (str == "YELLOW") return AVATAR_YELLOW;
        if (str == "PURPLE") return AVATAR_PURPLE;
        return AVATAR_BLUE;
    }

    ModType SaveSystem::StringToModType(const std::string& str) {
        if (str == "NONE") return MOD_NONE;
        if (str == "FASTFIRE") return MOD_FASTFIRE;
        if (str == "SHIELD") return MOD_SHIELD;
        if (str == "TRIPLESHOT") return MOD_TRIPLESHOT;
        if (str == "SLOWMO") return MOD_SLOWMO;
        return MOD_NONE;
    }

    Difficulty SaveSystem::StringToDifficulty(const std::string& str) {
        if (str == "EASY") return DIFF_EASY;
        if (str == "MEDIUM") return DIFF_MEDIUM;
        if (str == "HARD") return DIFF_HARD;
        return DIFF_MEDIUM;
    }

    std::string SaveSystem::AvatarTypeToString(AvatarType type) {
        switch (type) {
        case AVATAR_BLUE: return "BLUE";
        case AVATAR_GREEN: return "GREEN";
        case AVATAR_RED: return "RED";
        case AVATAR_YELLOW: return "YELLOW";
        case AVATAR_PURPLE: return "PURPLE";
        default: return "BLUE";
        }
    }

    std::string SaveSystem::ModTypeToString(ModType type) {
        switch (type) {
        case MOD_NONE: return "NONE";
        case MOD_FASTFIRE: return "FASTFIRE";
        case MOD_SHIELD: return "SHIELD";
        case MOD_TRIPLESHOT: return "TRIPLESHOT";
        case MOD_SLOWMO: return "SLOWMO";
        default: return "NONE";
        }
    }

    std::string SaveSystem::DifficultyToString(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY: return "EASY";
        case DIFF_MEDIUM: return "MEDIUM";
        case DIFF_HARD: return "HARD";
        default: return "MEDIUM";
        }
    }

} // namespace SpaceInvaders