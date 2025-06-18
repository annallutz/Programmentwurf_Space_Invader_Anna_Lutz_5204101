#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include "Player.h"
#include "Boss.h"
#include "TextureManager.h"
#include <vector>
#include <memory>

namespace SpaceInvaders {

    // Forward Declarations
    class Bullet;
    class EnemyBullet;
    class Enemy;
    class PowerUp;
    struct Explosion;

    /**
     * GameRenderer - Separates Rendering-System
     *
     * Zuständig für alles Rendering um Game.cpp zu entlasten:
     * - UI-Rendering (Score, Leben, PowerUp-Indikatoren)
     * - Hintergrund und Effekte
     * - Einheitliche PowerUp-Indikator-Funktionen (DRY-Prinzip)
     */
    class GameRenderer {
    public:
        GameRenderer();
        ~GameRenderer() = default;

        // === Haupt-Rendering ===
        void RenderGameplay(
            const std::vector<std::unique_ptr<Player>>& players,
            const std::vector<std::unique_ptr<Enemy>>& enemies,
            const std::vector<std::unique_ptr<Bullet>>& bullets,
            const std::vector<std::unique_ptr<EnemyBullet>>& enemyBullets,
            const std::vector<std::unique_ptr<PowerUp>>& powerUps,
            const std::vector<Explosion>& explosions,
            const std::unique_ptr<Boss>& boss,
            bool bossActive,
            bool slowMotionActive,
            float backgroundScroll,
            Difficulty currentDifficulty,
            bool isMultiPlayer
        );

        // === UI-Komponenten ===
        void RenderUI(const std::vector<std::unique_ptr<Player>>& players,
            bool isMultiPlayer, Difficulty difficulty);

        void RenderPowerUpIndicators(const std::vector<std::unique_ptr<Player>>& players,
            bool isMultiPlayer, bool slowMotionActive, float slowMotionTimer);

        void RenderBackground(float backgroundScroll);

        // === Coin-Display (überall verwendbar) ===
        void RenderCoinsDisplay(int coins, int x, int y, int fontSize = 30);

    private:
        TextureManager& textureManager;

        // === PowerUp-Indikator Hilfsfunktionen (DRY-Prinzip) ===
        void RenderSinglePowerUpIndicator(
            ModType modType,
            float progress,
            Vector2 position,
            int size,
            Color ringColor
        );

        void RenderPlayerPowerUpIndicators(
            const Player* player,
            int startX,
            int startY,
            int indicatorSize,
            int spacing
        );

        // === Effekte ===
        void RenderSlowMotionEffect();
        void RenderExplosions(const std::vector<Explosion>& explosions);

        // === Boss-spezifisch ===
        void RenderBossUI();
    };

} // namespace SpaceInvaders