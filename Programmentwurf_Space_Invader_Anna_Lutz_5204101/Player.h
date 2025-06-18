#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include <string>

namespace SpaceInvaders {

    /**
     * @brief Player-Klasse mit Avatar-System und PowerUps
     *
     * Repr‰sentiert einen Spieler mit seinen Eigenschaften:
     * - Avatar-spezifische Boni
     * - PowerUp-Management
     * - Bewegung und Schieﬂen
     * - Leben und Score-System
     */
    class Player {
    public:
        // === Konstruktor & Destruktor ===
        Player(AvatarType avatarType, Vector2 startPosition);
        ~Player() = default;

        // === Update & Render ===
        void Update(float deltaTime, Difficulty difficulty);
        void Render() const;
        void RenderShield() const;

        // === Movement & Input ===
        void HandleInput(bool leftPressed, bool rightPressed, bool shootPressed);
        void SetPosition(Vector2 newPosition);
        Vector2 GetPosition() const;
        Rectangle GetBounds() const;

        // === Shooting System ===
        bool WantsToShoot();
        void ResetShootRequest();
        bool CanShoot() const;
        void UpdateShootCooldown(float deltaTime);

        // === Health & Score ===
        int GetLives() const;
        int GetScore() const;
        void AddScore(int points);
        void LoseLife();
        void GainLife();
        void Reset(Vector2 newPosition);

        // === Player Info ===
        void SetName(const std::string& name);
        const std::string& GetName() const;
        AvatarType GetAvatarType() const;

        // === Avatar-spezifische Boni ===
        float GetMovementSpeedMultiplier() const;
        float GetBulletSpeedMultiplier() const;
        float GetFireRateMultiplier() const;
        int GetBonusLives() const;

        // === PowerUp System ===
        void ActivateShield(int strength, float duration);
        void ActivateTripleShot(float duration);
        void ActivateFastFire(float duration);
        void UpdatePowerUps(float deltaTime);

        // === PowerUp Queries ===
        bool HasShield() const;
        bool IsTripleShotActive() const;
        bool IsFastFireActive() const;
        int GetShieldStrength() const;
        float GetShieldTimer() const;
        float GetTripleShotTimer() const;
        float GetFastFireTimer() const;

        // === PowerUp Events ===
        void HitShield(); // Reduziert Shield-St‰rke bei Treffer

        // === Mod System (Start-Mods) ===
        void ApplyStartMod(ModType mod);
        ModType GetStartMod() const;

        // === Constants ===
        static const float AVATAR_WIDTH;
        static const float AVATAR_HEIGHT;

    private:
        // === Avatar & Eigenschaften ===
        AvatarType avatarType;
        std::string playerName;
        ModType startMod;

        // === Position & Movement ===
        Vector2 position;
        float movementSpeed;
        Rectangle bounds;

        // === Health & Score ===
        int lives;
        int maxLives;
        int score;

        // === Shooting ===
        bool shootRequested;
        float shootCooldown;
        float shootTimer;

        // === PowerUp States ===
        struct PowerUpState {
            bool isActive = false;
            float timer = 0.0f;
            float duration = 0.0f;
        };

        PowerUpState shieldState;
        PowerUpState tripleShotState;
        PowerUpState fastFireState;
        int shieldStrength;

        // === Avatar-spezifische Multiplikatoren ===
        float movementMultiplier;
        float bulletSpeedMultiplier;
        float fireRateMultiplier;
        int bonusLives;

        // === Private Hilfsfunktionen ===
        void CalculateAvatarBonuses();
        void UpdateBounds();
        void ClampPosition();
    };

} // namespace SpaceInvaders