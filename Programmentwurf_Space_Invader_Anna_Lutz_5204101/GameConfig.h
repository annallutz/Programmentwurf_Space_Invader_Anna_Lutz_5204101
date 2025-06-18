#pragma once

#include <string>

namespace SpaceInvaders {

    // === Schwierigkeitsgrade ===
    enum Difficulty {
        DIFF_EASY,
        DIFF_MEDIUM,
        DIFF_HARD
    };

    // === Avatar Typen ===
    enum AvatarType {
        AVATAR_BLUE,    // Kostenlos, keine Boni
        AVATAR_GREEN,   // +10% Lebenspunkte
        AVATAR_RED,     // +10% Projektilgeschwindigkeit
        AVATAR_YELLOW,  // +10% Bewegungsgeschwindigkeit
        AVATAR_PURPLE   // +10% Schussfrequenz
    };

    // === Modifikator Typen ===
    enum ModType {
        MOD_NONE,
        MOD_FASTFIRE,
        MOD_SHIELD,
        MOD_TRIPLESHOT,
        MOD_SLOWMO
    };

    // === PowerUp Typen ===
    enum PowerUpType {
        POWERUP_NONE,
        POWERUP_SHIELD,
        POWERUP_TRIPLESHOT,
        POWERUP_SLOWMO
    };

    // === Game States ===
    enum GameState {
        STATE_MAINMENU,
        STATE_SINGLEPLAYER_SETUP,
        STATE_MULTIPLAYER_SETUP,
        STATE_DIFFICULTY_SELECTION,
        STATE_STATISTICS,
        STATE_RULES,
        STATE_GAME,
        STATE_GAMEOVER,
        STATE_PAUSE
    };

    // === Zentrale Spiel-Konfiguration ===
    struct GameConfig {
        // === Bildschirm ===
        static const int SCREEN_WIDTH = 1280;
        static const int SCREEN_HEIGHT = 800;
        static const int TARGET_FPS = 60;

        // === Avatar Kosten ===
        static const int AVATAR_COST = 30;
        static const int MOD_COST = 30;

        // === Avatar Effekte (in Prozent) - FIXED: als inline ===
        static inline const float AVATAR_HEALTH_BONUS = 0.1f;
        static inline const float AVATAR_BULLET_SPEED_BONUS = 0.1f;
        static inline const float AVATAR_MOVEMENT_SPEED_BONUS = 0.1f;
        static inline const float AVATAR_FIRE_RATE_BONUS = 0.1f;

        // === PowerUp Dauer ===
        static inline const float POWERUP_DURATION = 5.0f;

        // === Boss ===
        static inline const float BOSS_SPAWN_TIME_EASY = 90.0f;
        static inline const float BOSS_SPAWN_TIME_MEDIUM = 75.0f;
        static inline const float BOSS_SPAWN_TIME_HARD = 60.0f;

        // === Schwierigkeitsgrad-spezifische Werte - FIXED: als inline ===
        static inline const float ENEMY_SPAWN_INTERVAL_EASY = 1.8f;
        static inline const float ENEMY_SPAWN_INTERVAL_MEDIUM = 1.2f;
        static inline const float ENEMY_SPAWN_INTERVAL_HARD = 0.8f;

        static inline const float ENEMY_SPEED_EASY = 120.0f;
        static inline const float ENEMY_SPEED_MEDIUM = 180.0f;
        static inline const float ENEMY_SPEED_HARD = 240.0f;

        static inline const float ENEMY_SHOOT_COOLDOWN_EASY = 2.5f;
        static inline const float ENEMY_SHOOT_COOLDOWN_MEDIUM = 1.8f;
        static inline const float ENEMY_SHOOT_COOLDOWN_HARD = 1.2f;

        static inline const float ENEMY_BULLET_SPEED_EASY = 200.0f;
        static inline const float ENEMY_BULLET_SPEED_MEDIUM = 280.0f;
        static inline const float ENEMY_BULLET_SPEED_HARD = 360.0f;

        static inline const float BACKGROUND_SCROLL_SPEED_EASY = 80.0f;
        static inline const float BACKGROUND_SCROLL_SPEED_MEDIUM = 120.0f;
        static inline const float BACKGROUND_SCROLL_SPEED_HARD = 160.0f;

        static inline const float PLAYER_SHOOT_COOLDOWN_EASY = 0.35f;
        static inline const float PLAYER_SHOOT_COOLDOWN_MEDIUM = 0.30f;
        static inline const float PLAYER_SHOOT_COOLDOWN_HARD = 0.25f;

        static inline const float PLAYER_MOVEMENT_SPEED_EASY = 280.0f;
        static inline const float PLAYER_MOVEMENT_SPEED_MEDIUM = 320.0f;
        static inline const float PLAYER_MOVEMENT_SPEED_HARD = 360.0f;

        static inline const float POWERUP_DROP_CHANCE_EASY = 0.15f;
        static inline const float POWERUP_DROP_CHANCE_MEDIUM = 0.12f;
        static inline const float POWERUP_DROP_CHANCE_HARD = 0.08f;

        // === Datei-Pfade ===
        static inline const char* SAVE_FOLDER = "save/";
        static inline const char* HIGHSCORE_FILE = "save/highscores.txt";
        static inline const char* COINS_FILE = "save/coins.txt";
        static inline const char* STATISTICS_FILE = "save/statistics.txt";
        static inline const char* AVATAR_UNLOCKS_FILE = "save/avatar_unlocks.txt";
        static inline const char* MOD_UNLOCKS_FILE = "save/mod_unlocks.txt";

        // === Hilfsfunktionen ===
        static float GetEnemySpawnInterval(Difficulty diff);
        static float GetEnemySpeed(Difficulty diff);
        static float GetEnemyShootCooldown(Difficulty diff);
        static float GetEnemyBulletSpeed(Difficulty diff);
        static float GetBackgroundScrollSpeed(Difficulty diff);
        static float GetPlayerShootCooldown(Difficulty diff);
        static float GetPlayerMovementSpeed(Difficulty diff);
        static float GetPowerUpDropChance(Difficulty diff);
        static float GetBossSpawnTime(Difficulty diff);
    };

} // namespace SpaceInvaders