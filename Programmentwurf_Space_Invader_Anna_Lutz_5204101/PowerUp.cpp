#include "PowerUp.h"
#include "TextureManager.h"
#include <cmath>

namespace SpaceInvaders {

    // === Konstanten ===
    const float PowerUp::DEFAULT_SIZE = 48.0f;
    const float PowerUp::DEFAULT_SPEED = 120.0f;

    // === Konstruktor ===
    PowerUp::PowerUp(Vector2 startPosition, PowerUpType type)
        : powerUpType(type)
        , position(startPosition)
        , speed(DEFAULT_SPEED)
        , size(DEFAULT_SIZE)
        , active(true)
        , animationTimer(0.0f)
        , rotationAngle(0.0f)
        , pulseScale(1.0f)
        , glowIntensity(0.5f)
    {
    }

    // === Update ===
    void PowerUp::Update(float deltaTime) {
        if (!active) return;

        // Nach unten fallen
        position.y += speed * deltaTime;

        // Animation aktualisieren
        UpdateAnimation(deltaTime);

        // Prüfen ob außerhalb des Bildschirms
        if (IsOffScreen()) {
            active = false;
        }
    }

    // === Render ===
    void PowerUp::Render() const {
        if (!active) return;

        // Glow-Effekt rendern
        RenderGlowEffect();

        // Icon rendern
        RenderIcon();
    }

    // === Animation ===
    void PowerUp::UpdateAnimation(float deltaTime) {
        animationTimer += deltaTime;

        // Rotation
        rotationAngle += 120.0f * deltaTime; // 120 Grad pro Sekunde
        if (rotationAngle >= 360.0f) {
            rotationAngle -= 360.0f;
        }

        // Pulsierender Effekt
        pulseScale = 1.0f + 0.1f * sinf(animationTimer * 4.0f);

        // Glow-Intensität
        glowIntensity = 0.3f + 0.3f * sinf(animationTimer * 3.0f);
    }

    // === Getters ===
    Vector2 PowerUp::GetPosition() const {
        return position;
    }

    Rectangle PowerUp::GetBounds() const {
        float actualSize = size * pulseScale;
        return {
            position.x - actualSize / 2,
            position.y - actualSize / 2,
            actualSize,
            actualSize
        };
    }

    PowerUpType PowerUp::GetType() const {
        return powerUpType;
    }

    float PowerUp::GetSize() const {
        return size * pulseScale;
    }

    // === State ===
    bool PowerUp::IsOffScreen() const {
        return position.y - size > GameConfig::SCREEN_HEIGHT;
    }

    bool PowerUp::IsActive() const {
        return active;
    }

    void PowerUp::Deactivate() {
        active = false;
    }

    // === Private Hilfsfunktionen ===
    Color PowerUp::GetPowerUpColor() const {
        switch (powerUpType) {
        case POWERUP_SHIELD:     return SKYBLUE;
        case POWERUP_TRIPLESHOT: return ORANGE;
        case POWERUP_SLOWMO:     return PURPLE;
        default:                 return WHITE;
        }
    }

    void PowerUp::RenderGlowEffect() const {
        Vector2 center = position;
        float actualSize = size * pulseScale;
        Color glowColor = GetPowerUpColor();

        // Äußerer Glow (groß und transparent)
        DrawCircleV(center, actualSize * 0.8f, Fade(glowColor, glowIntensity * 0.2f));

        // Mittlerer Glow
        DrawCircleV(center, actualSize * 0.6f, Fade(glowColor, glowIntensity * 0.4f));

        // Innerer Glow
        DrawCircleV(center, actualSize * 0.4f, Fade(glowColor, glowIntensity * 0.6f));
    }

    void PowerUp::RenderIcon() const {
        auto& textureManager = TextureManager::GetInstance();

        // Entsprechende Mod-Textur laden
        ModType modType = MOD_NONE;
        switch (powerUpType) {
        case POWERUP_SHIELD:     modType = MOD_SHIELD; break;
        case POWERUP_TRIPLESHOT: modType = MOD_TRIPLESHOT; break;
        case POWERUP_SLOWMO:     modType = MOD_SLOWMO; break;
        default: return;
        }

        Texture2D iconTexture = textureManager.GetModIconTexture(modType);
        float actualSize = size * pulseScale;

        // Icon mit Rotation rendern
        Rectangle source = { 0, 0, (float)iconTexture.width, (float)iconTexture.height };
        Rectangle dest = {
            position.x,
            position.y,
            actualSize,
            actualSize
        };
        Vector2 origin = { actualSize / 2, actualSize / 2 };

        DrawTexturePro(iconTexture, source, dest, origin, rotationAngle, WHITE);
    }

} // namespace SpaceInvaders