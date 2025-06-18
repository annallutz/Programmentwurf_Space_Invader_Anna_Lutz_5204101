#pragma once

#include "raylib.h"
#include "GameConfig.h"

namespace SpaceInvaders {

    /**
     * @brief PowerUp-Klasse für Items die von Gegnern gedroppt werden
     *
     * PowerUps fallen nach unten und können von Spielern eingesammelt werden.
     * Verschiedene Typen gewähren temporäre Fähigkeiten.
     */
    class PowerUp {
    public:
        // === Konstruktor & Destruktor ===
        PowerUp(Vector2 startPosition, PowerUpType type);
        ~PowerUp() = default;

        // === Update & Render ===
        void Update(float deltaTime);
        void Render() const;

        // === Getters ===
        Vector2 GetPosition() const;
        Rectangle GetBounds() const;
        PowerUpType GetType() const;
        float GetSize() const;

        // === Collision & Cleanup ===
        bool IsOffScreen() const;
        bool IsActive() const;
        void Deactivate();

        // === Animation ===
        void UpdateAnimation(float deltaTime);

        // === Constants ===
        static const float DEFAULT_SIZE;
        static const float DEFAULT_SPEED;

    private:
        // === Eigenschaften ===
        PowerUpType powerUpType;
        Vector2 position;
        float speed;
        float size;
        bool active;

        // === Animation ===
        float animationTimer;
        float rotationAngle;
        float pulseScale;
        float glowIntensity;

        // === Private Hilfsfunktionen ===
        Color GetPowerUpColor() const;
        void RenderGlowEffect() const;
        void RenderIcon() const;
    };

} // namespace SpaceInvaders