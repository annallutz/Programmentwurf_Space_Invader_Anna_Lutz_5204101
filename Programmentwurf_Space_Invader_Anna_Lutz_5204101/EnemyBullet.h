#pragma once

#include "raylib.h"

namespace SpaceInvaders {

    /**
     * @brief Gegner-Bullet Klasse
     *
     * Repräsentiert Projektile, die von Gegnern oder dem Boss abgefeuert werden.
     * Verschiedene Gegner-Typen können unterschiedliche Bullet-Farben haben.
     */
    class EnemyBullet {
    public:
        // === Konstruktor & Destruktor ===
        EnemyBullet(Vector2 startPosition, float speed, Color color = RED);
        ~EnemyBullet() = default;

        // === Update & Render ===
        void Update(float deltaTime);
        void Render() const;

        // === Getters ===
        Vector2 GetPosition() const;
        float GetRadius() const;
        Color GetColor() const;
        Rectangle GetBounds() const;

        // === Collision & Cleanup ===
        bool IsOffScreen() const;
        bool IsActive() const;
        void Deactivate();

        // === Setters ===
        void SetColor(Color newColor);

        // === Constants ===
        static const float DEFAULT_RADIUS;
        static const float DEFAULT_SPEED;

    private:
        // === Eigenschaften ===
        Vector2 position;
        float speed;
        float radius;
        Color bulletColor;
        bool active;
    };

} // namespace SpaceInvaders