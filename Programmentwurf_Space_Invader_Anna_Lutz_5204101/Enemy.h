#pragma once

#include "raylib.h"
#include "GameConfig.h"

namespace SpaceInvaders {

    /**
     * @brief Enemy-Klasse für verschiedene Gegner-Typen
     *
     * Repräsentiert einen Gegner mit:
     * - Bewegung nach unten
     * - Schießverhalten (schwierigkeitsabhängig)
     * - Verschiedene Typen mit unterschiedlichen Eigenschaften
     * - Score-Werte
     */
    class Enemy {
    public:
        // === Konstruktor & Destruktor ===
        Enemy(int enemyType, Vector2 startPosition, Difficulty difficulty);
        ~Enemy() = default;

        // === Update & Render ===
        void Update(float deltaTime);
        void Render() const;

        // === Getters ===
        Vector2 GetPosition() const;
        Rectangle GetBounds() const;
        int GetScoreValue() const;
        int GetEnemyType() const;
        float GetSpeed() const;

        // === Shooting System ===
        bool WantsToShoot() const;
        void ResetShootTimer();
        Vector2 GetShootPosition() const;
        Color GetBulletColor() const;

        // === Collision & Cleanup ===
        bool IsOffScreen() const;
        bool IsActive() const;
        void Deactivate();

        // === Constants ===
        static const float DEFAULT_SIZE;
        static const int MAX_ENEMY_TYPES;

    private:
        // === Eigenschaften ===
        int enemyType;          // 0-4 für verschiedene Enemy-Typen
        Vector2 position;
        float speed;
        float width, height;
        int scoreValue;
        bool active;

        // === Shooting ===
        float shootTimer;
        float shootCooldown;
        Color bulletColor;

        // === Schwierigkeit ===
        Difficulty difficulty;

        // === Private Hilfsfunktionen ===
        void InitializeEnemyProperties();
        Color GetEnemyBulletColor(int type) const;
        int GetEnemyScoreValue(int type) const;
        float GetEnemySizeVariation() const;
    };

} // namespace SpaceInvaders