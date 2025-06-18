#pragma once

#include "raylib.h"
#include "GameConfig.h"

namespace SpaceInvaders {

    /**
     * @brief Spieler-Bullet Klasse
     *
     * Repräsentiert Projektile, die von Spielern abgefeuert werden.
     * Unterstützt verschiedene Bullet-Typen (normal, FastFire, TripleShot).
     */
    class Bullet {
    public:
        // === Bullet Typen ===
        enum BulletType {
            NORMAL,
            FAST_FIRE,
            TRIPLE_SHOT
        };

        // === Konstruktor & Destruktor ===
        Bullet(int playerId, Vector2 startPosition, float speed, BulletType type = NORMAL);
        ~Bullet() = default;

        // === Update & Render ===
        void Update(float deltaTime);
        void Render() const;

        // === Getters ===
        Vector2 GetPosition() const;
        float GetRadius() const;
        int GetPlayerId() const;
        BulletType GetType() const;
        Rectangle GetBounds() const;

        // === Collision & Cleanup ===
        bool IsOffScreen() const;
        bool IsActive() const;
        void Deactivate();

        // === Constants ===
        static const float DEFAULT_RADIUS;
        static const float DEFAULT_SPEED;

    private:
        // === Eigenschaften ===
        int playerId;           // 0 = Player1, 1 = Player2
        Vector2 position;
        float speed;
        float radius;
        BulletType bulletType;
        bool active;

        // === Visuelle Eigenschaften ===
        Color bulletColor;

        // === Private Hilfsfunktionen ===
        Color GetBulletColor() const;
        void UpdateColor();
    };

} // namespace SpaceInvaders