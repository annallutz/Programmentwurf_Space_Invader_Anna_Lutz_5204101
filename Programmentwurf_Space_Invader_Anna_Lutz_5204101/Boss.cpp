#include "Boss.h"
#include "TextureManager.h"
#include "EnemyBullet.h"
#include <cmath>

namespace SpaceInvaders {

    // === Konstanten ===
    const float Boss::DEFAULT_WIDTH = 200.0f;
    const float Boss::DEFAULT_HEIGHT = 140.0f;
    const int Boss::DEFAULT_HEALTH = 20;

    // === Konstruktor ===
    Boss::Boss(Vector2 startPosition, Difficulty difficulty)
        : position(startPosition)
        , startPosition(startPosition)
        , width(DEFAULT_WIDTH)
        , height(DEFAULT_HEIGHT)
        , difficulty(difficulty)
        , active(true)
        , movementTimer(0.0f)
        , shootTimer(0.0f)
    {
        InitializeBossProperties();
    }

    // === Update ===
    void Boss::Update(float deltaTime) {
        if (!active) return;

        UpdateMovement(deltaTime);
        UpdateShooting(deltaTime);
    }

    // === Render ===
    void Boss::Render() const {
        if (!active) return;

        auto& textureManager = TextureManager::GetInstance();
        Texture2D bossTexture = textureManager.GetBossTexture();

        // Boss mit leichtem Schüttel-Effekt bei niedrigem Leben
        Vector2 renderPos = position;
        if (GetHealthPercentage() < 0.3f) {
            // Schüttel-Effekt wenn Boss fast tot ist
            float shakeIntensity = 3.0f;
            renderPos.x += (rand() % (int)(shakeIntensity * 2)) - shakeIntensity;
            renderPos.y += (rand() % (int)(shakeIntensity * 2)) - shakeIntensity;
        }

        // Boss-Textur rendern
        TextureManager::DrawTextureScaled(bossTexture,
            renderPos.x, renderPos.y,
            width, height);

        // Schatten-Effekt
        TextureManager::DrawTextureScaled(bossTexture,
            renderPos.x + 5, renderPos.y + 5,
            width, height, Fade(BLACK, 0.3f));
    }

    void Boss::RenderHealthBar() const {
        if (!active) return;

        const float barWidth = 300.0f;
        const float barHeight = 20.0f;
        const float x = (GameConfig::SCREEN_WIDTH - barWidth) / 2.0f;
        const float y = 20.0f;

        // Hintergrund
        DrawRectangle((int)x, (int)y, (int)barWidth, (int)barHeight, DARKGRAY);

        // Health-Bar
        float healthRatio = GetHealthPercentage();
        Color healthColor = RED;
        if (healthRatio > 0.6f) healthColor = GREEN;
        else if (healthRatio > 0.3f) healthColor = YELLOW;

        DrawRectangle((int)x, (int)y, (int)(barWidth * healthRatio), (int)barHeight, healthColor);

        // Rahmen
        DrawRectangleLines((int)x, (int)y, (int)barWidth, (int)barHeight, WHITE);

        // Boss-Label
        const char* bossText = "BOSS";
        int textWidth = MeasureText(bossText, 30);
        DrawText(bossText, (GameConfig::SCREEN_WIDTH - textWidth) / 2, 50, 30, YELLOW);
    }

    // === Getters ===
    Vector2 Boss::GetPosition() const {
        return position;
    }

    Rectangle Boss::GetBounds() const {
        return { position.x, position.y, width, height };
    }

    int Boss::GetCurrentHealth() const {
        return currentHealth;
    }

    int Boss::GetMaxHealth() const {
        return maxHealth;
    }

    float Boss::GetHealthPercentage() const {
        return (float)currentHealth / (float)maxHealth;
    }

    // === Shooting System ===
    std::vector<std::unique_ptr<EnemyBullet>> Boss::Shoot() {
        std::vector<std::unique_ptr<EnemyBullet>> bullets;

        if (!WantsToShoot()) return bullets;

        ResetShootTimer();

        // Mehrere Schüsse basierend auf Schwierigkeit
        float bulletSpeed = GetBulletSpeed();
        Vector2 leftShoot = { position.x + width * 0.25f, position.y + height };
        Vector2 rightShoot = { position.x + width * 0.75f, position.y + height };

        // Basis-Schüsse (immer)
        bullets.push_back(std::make_unique<EnemyBullet>(leftShoot, bulletSpeed, BLUE));
        bullets.push_back(std::make_unique<EnemyBullet>(rightShoot, bulletSpeed, BLUE));

        // Zusätzliche Schüsse bei höherer Schwierigkeit
        if (difficulty >= DIFF_MEDIUM) {
            Vector2 centerShoot = { position.x + width * 0.5f, position.y + height };
            bullets.push_back(std::make_unique<EnemyBullet>(centerShoot, bulletSpeed, DARKBLUE));
        }

        if (difficulty >= DIFF_HARD) {
            // Diagonal-Schüsse
            Vector2 leftDiag = { position.x + width * 0.15f, position.y + height };
            Vector2 rightDiag = { position.x + width * 0.85f, position.y + height };
            bullets.push_back(std::make_unique<EnemyBullet>(leftDiag, bulletSpeed * 0.8f, PURPLE));
            bullets.push_back(std::make_unique<EnemyBullet>(rightDiag, bulletSpeed * 0.8f, PURPLE));
        }

        return bullets;
    }

    bool Boss::WantsToShoot() const {
        return shootTimer >= shootCooldown;
    }

    void Boss::ResetShootTimer() {
        shootTimer = 0.0f;
    }

    // === Combat ===
    void Boss::TakeDamage(int damage) {
        currentHealth -= damage;
        if (currentHealth < 0) {
            currentHealth = 0;
        }
    }

    bool Boss::IsDefeated() const {
        return currentHealth <= 0;
    }

    bool Boss::IsActive() const {
        return active;
    }

    void Boss::Deactivate() {
        active = false;
    }

    // === Private Hilfsfunktionen ===
    void Boss::InitializeBossProperties() {
        // Health basierend auf Schwierigkeit
        maxHealth = DEFAULT_HEALTH;
        switch (difficulty) {
        case DIFF_EASY:   maxHealth = 15; break;
        case DIFF_MEDIUM: maxHealth = 20; break;
        case DIFF_HARD:   maxHealth = 30; break;
        }
        currentHealth = maxHealth;

        // Bewegungsparameter
        horizontalSpeed = GetMovementSpeed();
        verticalSpeed = horizontalSpeed * 0.5f;
        movementAmplitude = 100.0f + 40.0f * GetDifficultyMultiplier();

        // Schuss-Cooldown
        shootCooldown = GetShootCooldown();
    }

    void Boss::UpdateMovement(float deltaTime) {
        movementTimer += deltaTime;
        position = CalculateMovementPattern(movementTimer);

        // Grenzen einhalten
        if (position.x < 0) position.x = 0;
        if (position.x + width > GameConfig::SCREEN_WIDTH) {
            position.x = GameConfig::SCREEN_WIDTH - width;
        }
        if (position.y < 30) position.y = 30;
        if (position.y > 150) position.y = 150;
    }

    void Boss::UpdateShooting(float deltaTime) {
        shootTimer += deltaTime;
    }

    Vector2 Boss::CalculateMovementPattern(float time) const {
        // Komplexes Bewegungsmuster: Horizontal-Sinus + Vertikal-Cosinus
        float centerX = startPosition.x;
        float centerY = startPosition.y;

        float x = centerX + movementAmplitude * sinf(time * horizontalSpeed);
        float y = centerY + 20.0f * sinf(time * verticalSpeed * 2.0f);

        return { x, y };
    }

    // === Schwierigkeitsabhängige Werte ===
    float Boss::GetDifficultyMultiplier() const {
        switch (difficulty) {
        case DIFF_EASY:   return 0.8f;
        case DIFF_MEDIUM: return 1.0f;
        case DIFF_HARD:   return 1.5f;
        default:          return 1.0f;
        }
    }

    float Boss::GetMovementSpeed() const {
        return 1.0f + 0.3f * GetDifficultyMultiplier();
    }

    float Boss::GetShootCooldown() const {
        float baseCooldown = 2.0f;
        return baseCooldown / (1.0f + 0.5f * GetDifficultyMultiplier());
    }

    float Boss::GetBulletSpeed() const {
        float baseSpeed = 320.0f;
        return baseSpeed + 60.0f * GetDifficultyMultiplier();
    }

} // namespace SpaceInvaders