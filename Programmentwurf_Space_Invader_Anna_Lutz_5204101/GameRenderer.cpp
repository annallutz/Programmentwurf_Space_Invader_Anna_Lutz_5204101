#include "GameRenderer.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "PowerUp.h"

namespace SpaceInvaders {

    // === Explosion struct (hier definiert um Circular Dependencies zu vermeiden) ===
    struct Explosion {
        Vector2 position;
        float size;
        float timer;
        float duration;
    };

    // === Konstruktor ===
    GameRenderer::GameRenderer()
        : textureManager(TextureManager::GetInstance()) {
    }

    // === Haupt-Rendering ===
    void GameRenderer::RenderGameplay(
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
        bool isMultiPlayer) {

        BeginDrawing();
        ClearBackground(BLACK);

        // Hintergrund
        RenderBackground(backgroundScroll);

        // Players
        for (const auto& player : players) {
            if (player) {
                player->Render();
                player->RenderShield();
            }
        }

        // Boss (falls aktiv)
        if (bossActive && boss) {
            boss->Render();
            boss->RenderHealthBar();
            RenderBossUI();
        }

        // Bullets
        for (const auto& bullet : bullets) {
            bullet->Render();
        }

        // Enemies (nur wenn kein Boss)
        if (!bossActive) {
            for (const auto& enemy : enemies) {
                enemy->Render();
            }
        }

        // Enemy Bullets
        for (const auto& enemyBullet : enemyBullets) {
            enemyBullet->Render();
        }

        // PowerUps
        for (const auto& powerUp : powerUps) {
            powerUp->Render();
        }

        // Explosionen
        RenderExplosions(explosions);

        // SlowMotion Effekt
        if (slowMotionActive) {
            RenderSlowMotionEffect();
        }

        // UI
        RenderUI(players, isMultiPlayer, currentDifficulty);

        // PowerUp Indikatoren
        float slowMotionTimer = 0.0f; // TODO: Von Game übergeben lassen
        RenderPowerUpIndicators(players, isMultiPlayer, slowMotionActive, slowMotionTimer);

        EndDrawing();
    }

    // === UI-Komponenten ===
    void GameRenderer::RenderUI(const std::vector<std::unique_ptr<Player>>& players,
        bool isMultiPlayer, Difficulty difficulty) {

        // Player 1 Score und Leben
        if (!players.empty() && players[0]) {
            const Player* player1 = players[0].get();

            // Score
            const char* scoreText = TextFormat("Score: %d", player1->GetScore());
            DrawText(scoreText, 20, 20, 24, WHITE);

            // Leben
            auto& lifeTexture = textureManager.GetLifeTexture();
            for (int i = 0; i < player1->GetLives(); i++) {
                TextureManager::DrawTextureScaled(lifeTexture,
                    20 + i * 35, 50, 30, 30);
            }
        }

        // Player 2 Score (Multiplayer)
        if (isMultiPlayer && players.size() > 1 && players[1]) {
            const Player* player2 = players[1].get();

            const char* p2ScoreText = TextFormat("P2 Score: %d", player2->GetScore());
            int textWidth = MeasureText(p2ScoreText, 24);
            DrawText(p2ScoreText, GameConfig::SCREEN_WIDTH - textWidth - 20, 20, 24, SKYBLUE);
        }

        // Schwierigkeitsgrad
        std::string diffText = "Schwierigkeit: ";
        switch (difficulty) {
        case DIFF_EASY: diffText += "Leicht"; break;
        case DIFF_MEDIUM: diffText += "Mittel"; break;
        case DIFF_HARD: diffText += "Schwer"; break;
        }
        DrawText(diffText.c_str(), 20, GameConfig::SCREEN_HEIGHT - 30, 16, GRAY);

        // Pause-Hinweis
        const char* pauseText = "'P' fuer Pause";
        int pauseWidth = MeasureText(pauseText, 16);
        DrawText(pauseText, GameConfig::SCREEN_WIDTH - pauseWidth - 20,
            GameConfig::SCREEN_HEIGHT - 30, 16, GRAY);
    }

    void GameRenderer::RenderPowerUpIndicators(const std::vector<std::unique_ptr<Player>>& players,
        bool isMultiPlayer, bool slowMotionActive, float slowMotionTimer) {

        int indicatorSize = 50;
        int spacing = 60;

        // Player 1 Indikatoren (links)
        if (!players.empty() && players[0]) {
            RenderPlayerPowerUpIndicators(players[0].get(), 20, 120, indicatorSize, spacing);
        }

        // Player 2 Indikatoren (rechts, Multiplayer)
        if (isMultiPlayer && players.size() > 1 && players[1]) {
            int p2X = GameConfig::SCREEN_WIDTH - 70;
            RenderPlayerPowerUpIndicators(players[1].get(), p2X, 120, indicatorSize, spacing);
        }

        // SlowMotion Indikator (global, mittig)
        if (slowMotionActive) {
            int slowmoX = GameConfig::SCREEN_WIDTH / 2 - 25;
            int slowmoY = 20;

            float progress = 1.0f - (slowMotionTimer / GameConfig::POWERUP_DURATION);

            RenderSinglePowerUpIndicator(
                MOD_SLOWMO,
                progress,
                { slowmoX + 25.0f, slowmoY + 25.0f },
                indicatorSize,
                PURPLE
            );
        }
    }

    void GameRenderer::RenderBackground(float backgroundScroll) {
        auto& backgroundTexture = textureManager.GetBackgroundTexture();

        // Doppelter scrollender Hintergrund für nahtloses Scrollen
        TextureManager::DrawTextureScaled(backgroundTexture,
            0, backgroundScroll - backgroundTexture.height,
            GameConfig::SCREEN_WIDTH, backgroundTexture.height);

        TextureManager::DrawTextureScaled(backgroundTexture,
            0, backgroundScroll,
            GameConfig::SCREEN_WIDTH, backgroundTexture.height);
    }

    void GameRenderer::RenderCoinsDisplay(int coins, int x, int y, int fontSize) {
        auto& coinTexture = textureManager.GetCoinTexture();

        // Coins-Text
        const char* coinText = TextFormat("%d", coins);
        int textWidth = MeasureText(coinText, fontSize);
        DrawText(coinText, x, y, fontSize, YELLOW);

        // Coin-Icon
        TextureManager::DrawTextureScaled(coinTexture,
            x + textWidth + 10, y - 3,
            fontSize + 6, fontSize + 6);
    }

    // === Private Hilfsfunktionen (DRY-Prinzip) ===
    void GameRenderer::RenderSinglePowerUpIndicator(
        ModType modType,
        float progress,
        Vector2 position,
        int size,
        Color ringColor) {

        // Hintergrund-Kreis
        DrawCircleV(position, size / 2.0f, Fade(DARKGRAY, 0.5f));

        // Fortschritts-Ring
        DrawRing(position, size / 2.0f - 5, size / 2.0f,
            -90.0f, -90.0f + 360.0f * progress, 60, ringColor);

        // Icon
        auto iconTexture = textureManager.GetModIconTexture(modType);
        TextureManager::DrawTextureScaled(iconTexture,
            position.x - size / 2, position.y - size / 2,
            size, size);
    }

    void GameRenderer::RenderPlayerPowerUpIndicators(
        const Player* player,
        int startX,
        int startY,
        int indicatorSize,
        int spacing) {

        if (!player) return;

        int currentY = startY;

        // Shield Indikator
        if (player->HasShield()) {
            float remaining = player->GetShieldTimer();
            float total = GameConfig::POWERUP_DURATION;
            float progress = 1.0f - (remaining / total);

            RenderSinglePowerUpIndicator(
                MOD_SHIELD,
                progress,
                { startX + indicatorSize / 2.0f, currentY + indicatorSize / 2.0f },
                indicatorSize,
                SKYBLUE
            );

            currentY += spacing;
        }

        // TripleShot Indikator
        if (player->IsTripleShotActive()) {
            float remaining = player->GetTripleShotTimer();
            float total = GameConfig::POWERUP_DURATION;
            float progress = 1.0f - (remaining / total);

            RenderSinglePowerUpIndicator(
                MOD_TRIPLESHOT,
                progress,
                { startX + indicatorSize / 2.0f, currentY + indicatorSize / 2.0f },
                indicatorSize,
                ORANGE
            );

            currentY += spacing;
        }

        // FastFire Indikator
        if (player->IsFastFireActive()) {
            float remaining = player->GetFastFireTimer();
            float total = GameConfig::POWERUP_DURATION;
            float progress = 1.0f - (remaining / total);

            RenderSinglePowerUpIndicator(
                MOD_FASTFIRE,
                progress,
                { startX + indicatorSize / 2.0f, currentY + indicatorSize / 2.0f },
                indicatorSize,
                YELLOW
            );

            currentY += spacing;
        }
    }

    void GameRenderer::RenderSlowMotionEffect() {
        const char* slowmoText = "ZEITLUPE!";
        int slowmoWidth = MeasureText(slowmoText, 40);
        DrawText(slowmoText, (GameConfig::SCREEN_WIDTH - slowmoWidth) / 2, 50, 40, PURPLE);

        // Scanlines für Slow-Motion Effekt
        for (int y = 0; y < GameConfig::SCREEN_HEIGHT; y += 4) {
            DrawLine(0, y, GameConfig::SCREEN_WIDTH, y, Fade(DARKGRAY, 0.3f));
        }
    }

    void GameRenderer::RenderExplosions(const std::vector<Explosion>& explosions) {
        auto& explosionTexture = textureManager.GetExplosionTexture();

        for (const auto& explosion : explosions) {
            Rectangle dest = {
                explosion.position.x - explosion.size / 2,
                explosion.position.y - explosion.size / 2,
                explosion.size,
                explosion.size
            };
            TextureManager::DrawTextureScaled(explosionTexture,
                dest.x, dest.y, dest.width, dest.height);
        }
    }

    void GameRenderer::RenderBossUI() {
        const char* bossText = "BOSS FIGHT!";
        int bossTextWidth = MeasureText(bossText, 30);
        DrawText(bossText, (GameConfig::SCREEN_WIDTH - bossTextWidth) / 2, 100, 30, YELLOW);
    }

} // namespace SpaceInvaders