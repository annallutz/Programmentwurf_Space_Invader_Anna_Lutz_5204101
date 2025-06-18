#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include <vector>
#include <memory>

namespace SpaceInvaders {

    // Forward Declaration
    class EnemyBullet;

    /**
     * @brief Boss-Klasse für den großen Endgegner
     *
     * Der Boss erscheint nach einer bestimmten Zeit und hat:
     * - Mehr Lebenspunkte
     * - Komplexere Bewegungsmuster
     * - Mehrfach-Schüsse
     * - Schwierigkeitsabhängiges Verhalten
     */
    class Boss {
    public:
        // === Konstruktor & Destruktor ===
        Boss(Vector2 startPosition, Difficulty difficulty);
        ~Boss() = default;

        // === Update & Render ===
        void Update(float deltaTime);
        void Render() const;
        void RenderHealthBar() const;

        // === Getters ===
        Vector2 GetPosition() const;
        Rectangle GetBounds() const;
        int GetCurrentHealth() const;
        int GetMaxHealth() const;
        float GetHealthPercentage() const;

        // === Shooting System ===
        std::vector<std::unique_ptr<EnemyBullet>> Shoot();
        bool WantsToShoot() const;
        void ResetShootTimer();

        // === Combat ===
        void TakeDamage(int damage = 1);
        bool IsDefeated() const;
        bool IsActive() const;
        void Deactivate();

        // === Constants ===
        static const float DEFAULT_WIDTH;
        static const float DEFAULT_HEIGHT;
        static const int DEFAULT_HEALTH;

    private:
        // === Eigenschaften ===
        Vector2 position;
        Vector2 startPosition;
        float width, height;
        int currentHealth;
        int maxHealth;
        bool active;
        Difficulty difficulty;

        // === Bewegung ===
        float movementTimer;
        float horizontalSpeed;
        float verticalSpeed;
        float movementAmplitude;

        // === Shooting ===
        float shootTimer;
        float shootCooldown;

        // === Private Hilfsfunktionen ===
        void InitializeBossProperties();
        void UpdateMovement(float deltaTime);
        void UpdateShooting(float deltaTime);
        Vector2 CalculateMovementPattern(float time) const;

        // === Schwierigkeitsabhängige Werte ===
        float GetDifficultyMultiplier() const;
        float GetMovementSpeed() const;
        float GetShootCooldown() const;
        float GetBulletSpeed() const;
    };

} // namespace SpaceInvaders