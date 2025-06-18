#include "Enemy.h"
#include "TextureManager.h"
#include <cstdlib>

namespace SpaceInvaders {

    // === Konstanten ===
    const float Enemy::DEFAULT_SIZE = 64.0f;
    const int Enemy::MAX_ENEMY_TYPES = 5;

    // === Konstruktor ===
    Enemy::Enemy(int enemyType, Vector2 startPosition, Difficulty difficulty)
        : enemyType(enemyType% MAX_ENEMY_TYPES) // Sicherstellen dass Type gültig ist
        , position(startPosition)
        , difficulty(difficulty)
        , active(true)
        , shootTimer(0.0f)
    {
        InitializeEnemyProperties();
    }

    // === Update ===
    void Enemy::Update(float deltaTime) {
        if (!active) return;

        // Nach unten bewegen
        position.y += speed * deltaTime;

        // Schuss-Timer aktualisieren
        shootTimer += deltaTime;

        // Prüfen ob außerhalb des Bildschirms
        if (IsOffScreen()) {
            active = false;
        }
    }

    // === Render ===
    void Enemy::Render() const {
        if (!active) return;

        auto& textureManager = TextureManager::GetInstance();
        Texture2D enemyTexture = textureManager.GetEnemyTexture(enemyType + 1); // +1 weil Texturen bei 1 beginnen

        TextureManager::DrawTextureScaled(enemyTexture,
            position.x, position.y,
            width, height);
    }

    // === Getters ===
    Vector2 Enemy::GetPosition() const {
        return position;
    }

    Rectangle Enemy::GetBounds() const {
        return { position.x, position.y, width, height };
    }

    int Enemy::GetScoreValue() const {
        return scoreValue;
    }

    int Enemy::GetEnemyType() const {
        return enemyType;
    }

    float Enemy::GetSpeed() const {
        return speed;
    }

    // === Shooting System ===
    bool Enemy::WantsToShoot() const {
        return shootTimer >= shootCooldown;
    }

    void Enemy::ResetShootTimer() {
        shootTimer = 0.0f;
    }

    Vector2 Enemy::GetShootPosition() const {
        return {
            position.x + width / 2,
            position.y + height
        };
    }

    Color Enemy::GetBulletColor() const {
        return bulletColor;
    }

    // === State ===
    bool Enemy::IsOffScreen() const {
        return position.y > GameConfig::SCREEN_HEIGHT;
    }

    bool Enemy::IsActive() const {
        return active;
    }

    void Enemy::Deactivate() {
        active = false;
    }

    // === Private Hilfsfunktionen ===
    void Enemy::InitializeEnemyProperties() {
        // Basis-Geschwindigkeit nach Schwierigkeit
        speed = GameConfig::GetEnemySpeed(difficulty);

        // Schuss-Cooldown nach Schwierigkeit
        shootCooldown = GameConfig::GetEnemyShootCooldown(difficulty);

        // Größenvariation hinzufügen
        float sizeVariation = GetEnemySizeVariation();
        width = DEFAULT_SIZE + sizeVariation;
        height = DEFAULT_SIZE + sizeVariation;

        // Geschwindigkeitsvariation (-25% bis +50%)
        float speedVariation = (rand() % 76 - 25) / 100.0f; // -0.25 bis +0.50
        speed += speed * speedVariation;

        // Score-Wert nach Enemy-Typ
        scoreValue = GetEnemyScoreValue(enemyType);

        // Bullet-Farbe nach Enemy-Typ
        bulletColor = GetEnemyBulletColor(enemyType);

        // Schuss-Cooldown-Variation (±20%)
        float cooldownVariation = (rand() % 41 - 20) / 100.0f; // -0.2 bis +0.2
        shootCooldown += shootCooldown * cooldownVariation;
        if (shootCooldown < 0.2f) shootCooldown = 0.2f; // Minimum
    }

    Color Enemy::GetEnemyBulletColor(int type) const {
        switch (type) {
        case 0: return PURPLE;
        case 1: return DARKGREEN;
        case 2: return ORANGE;
        case 3: return PINK;
        case 4: return SKYBLUE;
        default: return RED;
        }
    }

    int Enemy::GetEnemyScoreValue(int type) const {
        // Verschiedene Enemy-Typen geben unterschiedliche Punkte
        switch (type) {
        case 0: return 10;  // Enemy1
        case 1: return 15;  // Enemy2
        case 2: return 20;  // Enemy3
        case 3: return 25;  // Enemy4
        case 4: return 30;  // Enemy5
        default: return 10;
        }
    }

    float Enemy::GetEnemySizeVariation() const {
        // Größenvariation: -15 bis +15 Pixel
        return (rand() % 31) - 15;
    }

} // namespace SpaceInvaders