#include "Bullet.h"

namespace SpaceInvaders {

    // === Konstanten ===
    const float Bullet::DEFAULT_RADIUS = 6.0f;
    const float Bullet::DEFAULT_SPEED = 500.0f;

    // === Konstruktor ===
    Bullet::Bullet(int playerId, Vector2 startPosition, float speed, BulletType type)
        : playerId(playerId)
        , position(startPosition)
        , speed(speed)
        , bulletType(type)
        , active(true)
        , radius(DEFAULT_RADIUS)
    {
        UpdateColor();
    }

    // === Update ===
    void Bullet::Update(float deltaTime) {
        if (!active) return;

        // Bullet fliegt nach oben
        position.y -= speed * deltaTime;

        // Prüfen ob außerhalb des Bildschirms
        if (IsOffScreen()) {
            active = false;
        }
    }

    // === Render ===
    void Bullet::Render() const {
        if (!active) return;

        DrawCircleV(position, radius, bulletColor);

        // Zusätzlicher Glow-Effekt für spezielle Bullets
        if (bulletType != NORMAL) {
            DrawCircleV(position, radius + 2.0f, Fade(bulletColor, 0.3f));
        }
    }#include "Bullet.h"

        namespace SpaceInvaders {

        // === Konstanten ===
        const float Bullet::DEFAULT_RADIUS = 6.0f;
        const float Bullet::DEFAULT_SPEED = 500.0f;

        // === Konstruktor ===
        Bullet::Bullet(int playerId, Vector2 startPosition, float speed, BulletType type)
            : playerId(playerId)
            , position(startPosition)
            , speed(speed)
            , bulletType(type)
            , active(true)
            , radius(DEFAULT_RADIUS)
        {
            UpdateColor();
        }

        // === Update ===
        void Bullet::Update(float deltaTime) {
            if (!active) return;

            // Bullet fliegt nach oben
            position.y -= speed * deltaTime;

            // Prüfen ob außerhalb des Bildschirms
            if (IsOffScreen()) {
                active = false;
            }
        }

        // === Render ===
        void Bullet::Render() const {
            if (!active) return;

            DrawCircleV(position, radius, bulletColor);

            // Zusätzlicher Glow-Effekt für spezielle Bullets
            if (bulletType != NORMAL) {
                DrawCircleV(position, radius + 2.0f, Fade(bulletColor, 0.3f));
            }
        }

        // === Getters ===
        Vector2 Bullet::GetPosition() const {
            return position;
        }

        float Bullet::GetRadius() const {
            return radius;
        }

        int Bullet::GetPlayerId() const {
            return playerId;
        }

        Bullet::BulletType Bullet::GetType() const {
            return bulletType;
        }

        Rectangle Bullet::GetBounds() const {
            return {
                position.x - radius,
                position.y - radius,
                radius * 2,
                radius * 2
            };
        }

        // === State ===
        bool Bullet::IsOffScreen() const {
            return position.y + radius < 0;
        }

        bool Bullet::IsActive() const {
            return active;
        }

        void Bullet::Deactivate() {
            active = false;
        }

        // === Private Hilfsfunktionen ===
        Color Bullet::GetBulletColor() const {
            switch (bulletType) {
            case FAST_FIRE:
                return YELLOW;
            case TRIPLE_SHOT:
                return ORANGE;
            default:
                return playerId == 0 ? WHITE : LIGHTBLUE;
            }
        }

        void Bullet::UpdateColor() {
            bulletColor = GetBulletColor();
        }

    } // namespace SpaceInvaders