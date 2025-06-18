#include "Player.h"
#include "TextureManager.h"

namespace SpaceInvaders {

    // === Konstanten ===
    const float Player::AVATAR_WIDTH = 120.0f;
    const float Player::AVATAR_HEIGHT = 120.0f;

    // === Konstruktor ===
    Player::Player(AvatarType avatarType, Vector2 startPosition)
        : avatarType(avatarType)
        , position(startPosition)
        , lives(3)
        , maxLives(3)
        , score(0)
        , shootRequested(false)
        , shootCooldown(0.3f)
        , shootTimer(0.0f)
        , shieldStrength(0)
        , startMod(MOD_NONE)
        , movementSpeed(320.0f)
    {
        CalculateAvatarBonuses();
        UpdateBounds();

        // Avatar-spezifische Leben-Boni anwenden
        maxLives += bonusLives;
        lives = maxLives;
    }

    // === Update ===
    void Player::Update(float deltaTime, Difficulty difficulty) {
        // Basis-Werte nach Schwierigkeitsgrad anpassen
        float baseSpeed = GameConfig::GetPlayerMovementSpeed(difficulty);
        movementSpeed = baseSpeed * movementMultiplier;

        float baseCooldown = GameConfig::GetPlayerShootCooldown(difficulty);
        shootCooldown = baseCooldown / fireRateMultiplier;

        // PowerUps aktualisieren
        UpdatePowerUps(deltaTime);
        UpdateShootCooldown(deltaTime);

        // Position-Bounds aktualisieren
        UpdateBounds();
        ClampPosition();
    }

    // === Render ===
    void Player::Render() const {
        auto& textureManager = TextureManager::GetInstance();
        Texture2D avatarTexture = textureManager.GetAvatarTexture(avatarType);

        TextureManager::DrawTextureScaled(avatarTexture,
            position.x, position.y,
            AVATAR_WIDTH, AVATAR_HEIGHT);
    }

    void Player::RenderShield() const {
        if (!HasShield()) return;

        Vector2 center = {
            position.x + AVATAR_WIDTH / 2,
            position.y + AVATAR_HEIGHT / 2
        };

        float outerRadius = AVATAR_WIDTH * 0.7f;
        float innerRadius = outerRadius - 8.0f;

        // Äußerer Glow-Effekt
        DrawCircleV(center, outerRadius + 15.0f, Fade(SKYBLUE, 0.1f));

        // Shield-Ring
        Color shieldColor = SKYBLUE;
        if (shieldStrength <= 1) {
            shieldColor = RED; // Warnung bei niedrigem Shield
        }

        DrawRing(center, innerRadius, outerRadius, 0.0f, 360.0f, 64, Fade(shieldColor, 0.6f));
    }

    // === Input Handling ===
    void Player::HandleInput(bool leftPressed, bool rightPressed, bool shootPressed) {
        // Bewegung
        if (leftPressed) {
            position.x -= movementSpeed * GetFrameTime();
        }
        if (rightPressed) {
            position.x += movementSpeed * GetFrameTime();
        }

        // Schießen
        if (shootPressed && CanShoot()) {
            shootRequested = true;
            shootTimer = 0.0f;
        }
    }

    // === Position & Bounds ===
    void Player::SetPosition(Vector2 newPosition) {
        position = newPosition;
        UpdateBounds();
    }

    Vector2 Player::GetPosition() const {
        return position;
    }

    Rectangle Player::GetBounds() const {
        return bounds;
    }

    void Player::UpdateBounds() {
        bounds = { position.x, position.y, AVATAR_WIDTH, AVATAR_HEIGHT };
    }

    void Player::ClampPosition() {
        if (position.x < 0) position.x = 0;
        if (position.x > GameConfig::SCREEN_WIDTH - AVATAR_WIDTH) {
            position.x = GameConfig::SCREEN_WIDTH - AVATAR_WIDTH;
        }
        if (position.y < 0) position.y = 0;
        if (position.y > GameConfig::SCREEN_HEIGHT - AVATAR_HEIGHT) {
            position.y = GameConfig::SCREEN_HEIGHT - AVATAR_HEIGHT;
        }
        UpdateBounds();
    }

    // === Shooting ===
    bool Player::WantsToShoot() {
        return shootRequested;
    }

    void Player::ResetShootRequest() {
        shootRequested = false;
    }

    bool Player::CanShoot() const {
        return shootTimer >= shootCooldown;
    }

    void Player::UpdateShootCooldown(float deltaTime) {
        shootTimer += deltaTime;
    }

    // === Health & Score ===
    int Player::GetLives() const {
        return lives;
    }

    int Player::GetScore() const {
        return score;
    }

    void Player::AddScore(int points) {
        score += points;
    }

    void Player::LoseLife() {
        if (lives > 0) {
            lives--;
        }
    }

    void Player::GainLife() {
        if (lives < maxLives) {
            lives++;
        }
    }

    void Player::Reset(Vector2 newPosition) {
        position = newPosition;
        lives = maxLives;
        score = 0;
        shootRequested = false;
        shootTimer = 0.0f;

        // PowerUps zurücksetzen
        shieldState = {};
        tripleShotState = {};
        fastFireState = {};
        shieldStrength = 0;

        UpdateBounds();
    }

    // === Player Info ===
    void Player::SetName(const std::string& name) {
        playerName = name;
    }

    const std::string& Player::GetName() const {
        return playerName;
    }

    AvatarType Player::GetAvatarType() const {
        return avatarType;
    }

    // === Avatar-Boni ===
    float Player::GetMovementSpeedMultiplier() const {
        return movementMultiplier;
    }

    float Player::GetBulletSpeedMultiplier() const {
        return bulletSpeedMultiplier;
    }

    float Player::GetFireRateMultiplier() const {
        // FastFire PowerUp berücksichtigen
        float bonus = IsFastFireActive() ? 2.0f : 1.0f;
        return fireRateMultiplier * bonus;
    }

    int Player::GetBonusLives() const {
        return bonusLives;
    }

    // === PowerUp System ===
    void Player::ActivateShield(int strength, float duration) {
        shieldState.isActive = true;
        shieldState.timer = 0.0f;
        shieldState.duration = duration;
        shieldStrength = strength;
    }

    void Player::ActivateTripleShot(float duration) {
        tripleShotState.isActive = true;
        tripleShotState.timer = 0.0f;
        tripleShotState.duration = duration;
    }

    void Player::ActivateFastFire(float duration) {
        fastFireState.isActive = true;
        fastFireState.timer = 0.0f;
        fastFireState.duration = duration;
    }

    void Player::UpdatePowerUps(float deltaTime) {
        // Shield Update
        if (shieldState.isActive) {
            shieldState.timer += deltaTime;
            if (shieldState.timer >= shieldState.duration || shieldStrength <= 0) {
                shieldState.isActive = false;
                shieldStrength = 0;
            }
        }

        // TripleShot Update
        if (tripleShotState.isActive) {
            tripleShotState.timer += deltaTime;
            if (tripleShotState.timer >= tripleShotState.duration) {
                tripleShotState.isActive = false;
            }
        }

        // FastFire Update
        if (fastFireState.isActive) {
            fastFireState.timer += deltaTime;
            if (fastFireState.timer >= fastFireState.duration) {
                fastFireState.isActive = false;
            }
        }
    }

    // === PowerUp Queries ===
    bool Player::HasShield() const {
        return shieldState.isActive && shieldStrength > 0;
    }

    bool Player::IsTripleShotActive() const {
        return tripleShotState.isActive;
    }

    bool Player::IsFastFireActive() const {
        return fastFireState.isActive;
    }

    int Player::GetShieldStrength() const {
        return shieldStrength;
    }

    float Player::GetShieldTimer() const {
        return shieldState.isActive ? shieldState.timer : 0.0f;
    }

    float Player::GetTripleShotTimer() const {
        return tripleShotState.isActive ? tripleShotState.timer : 0.0f;
    }

    float Player::GetFastFireTimer() const {
        return fastFireState.isActive ? fastFireState.timer : 0.0f;
    }

    void Player::HitShield() {
        if (HasShield()) {
            shieldStrength--;
        }
    }

    // === Mod System ===
    void Player::ApplyStartMod(ModType mod) {
        startMod = mod;

        switch (mod) {
        case MOD_SHIELD:
            ActivateShield(3, GameConfig::POWERUP_DURATION);
            break;
        case MOD_TRIPLESHOT:
            ActivateTripleShot(GameConfig::POWERUP_DURATION);
            break;
        case MOD_FASTFIRE:
            ActivateFastFire(GameConfig::POWERUP_DURATION);
            break;
        case MOD_SLOWMO:
            // Wird global in Game.cpp behandelt
            break;
        default:
            break;
        }
    }

    ModType Player::GetStartMod() const {
        return startMod;
    }

    // === Private Hilfsfunktionen ===
    void Player::CalculateAvatarBonuses() {
        // Standard-Werte
        movementMultiplier = 1.0f;
        bulletSpeedMultiplier = 1.0f;
        fireRateMultiplier = 1.0f;
        bonusLives = 0;

        // Avatar-spezifische Boni
        switch (avatarType) {
        case AVATAR_BLUE:
            // Keine Boni (kostenloser Avatar)
            break;

        case AVATAR_GREEN:
            // +10% Lebenspunkte (praktisch: +1 Leben)
            bonusLives = 1;
            break;

        case AVATAR_RED:
            // +10% Projektilgeschwindigkeit
            bulletSpeedMultiplier = 1.0f + GameConfig::AVATAR_BULLET_SPEED_BONUS;
            break;

        case AVATAR_YELLOW:
            // +10% Bewegungsgeschwindigkeit
            movementMultiplier = 1.0f + GameConfig::AVATAR_MOVEMENT_SPEED_BONUS;
            break;

        case AVATAR_PURPLE:
            // +10% Schussfrequenz
            fireRateMultiplier = 1.0f + GameConfig::AVATAR_FIRE_RATE_BONUS;
            break;
        }
    }

} // namespace SpaceInvaders