#include "EnemyBullet.h"
#include "GameConfig.h"

namespace SpaceInvaders {

    // === Konstanten ===
    const float EnemyBullet::DEFAULT_RADIUS = 8.0f;
    const float EnemyBullet::DEFAULT_SPEED = 280.0f;

    // === Konstruktor ===
    EnemyBullet::EnemyBullet(Vector2 startPosition, float speed, Color color)
        : position(startPosition)
        , speed(speed)
        , radius(DEFAULT_RADIUS)
        , bulletColor(color)
        , active(true)
    {
    }

    // === Update ===
    void EnemyBullet::Update(float deltaTime) {
        if (!active) return;

        // Bullet fliegt nach unten
        position.y += speed * deltaTime;

        // Prüfen ob außerhalb des Bildschirms
        if (IsOffScreen()) {
            active = false;
        }
    }

    // === Render ===
    void EnemyBullet::Render() const {
        if (!active) return;

        // Hauptbullet
        DrawCircleV(position, radius, bulletColor);

        // Innerer Kern für bessere Sichtbarkeit
        DrawCircleV(position, radius * 0.6f, Fade(WHITE, 0.8f));

        // Äußerer Glow
        DrawCircleV(position, radius + 3.0f, Fade(bulletColor, 0.2f));
    }

    // === Getters ===
    Vector2 EnemyBullet::GetPosition() const {
        return position;
    }

    float EnemyBullet::GetRadius() const {
        return radius;
    }

    Color EnemyBullet::GetColor() const {
        return bulletColor;
    }

    Rectangle EnemyBullet::GetBounds() const {
        return {
            position.x - radius,
            position.y - radius,
            radius * 2,
            radius * 2
        };
    }

    // === State ===
    bool EnemyBullet::IsOffScreen() const {
        return position.y - radius > GameConfig::SCREEN_HEIGHT;
    }

    bool EnemyBullet::IsActive() const {
        return active;
    }

    void EnemyBullet::Deactivate() {
        active = false;
    }

    // === Setters ===
    void EnemyBullet::SetColor(Color newColor) {
        bulletColor = newColor;
    }

} // namespace SpaceInvaders