#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "PowerUp.h"
#include <vector>
#include <memory>

namespace SpaceInvaders {

    /**
     * Explosion-Effekt Struktur
     */
    struct Explosion {
        Vector2 position;
        float size;
        float timer;
        float duration;
    };

    /**
     * GameplayManager - Verwaltet komplette Gameplay-Logik
     *
     * Zuständig für:
     * - Kollisionserkennung
     * - Object Spawning (Enemies, PowerUps, Boss)
     * - Game Object Updates
     * - Explosion-System
     *
     * Entlastet Game.cpp erheblich von Implementierungsdetails.
     */
    class GameplayManager {
    public:
        GameplayManager();
        ~GameplayManager() = default;

        // === Update-Funktionen ===
        void UpdateGameplay(
            std::vector<std::unique_ptr<Player>>& players,
            std::vector<std::unique_ptr<Enemy>>& enemies,
            std::vector<std::unique_ptr<Bullet>>& bullets,
            std::vector<std::unique_ptr<EnemyBullet>>& enemyBullets,
            std::vector<std::unique_ptr<PowerUp>>& powerUps,
            std::vector<Explosion>& explosions,
            std::unique_ptr<Boss>& boss,
            bool& bossActive,
            float deltaTime,
            float scaledDeltaTime,
            Difficulty difficulty,
            bool isMultiPlayer
        );

        // === Spawning ===
        void SpawnEnemy(std::vector<std::unique_ptr<Enemy>>& enemies, Difficulty difficulty);
        void SpawnPowerUp(std::vector<std::unique_ptr<PowerUp>>& powerUps, Vector2 position, Difficulty difficulty);
        void SpawnBoss(std::unique_ptr<Boss>& boss, bool& bossActive, Difficulty difficulty);

        // === Kollisionen ===
        void HandleAllCollisions(
            std::vector<std::unique_ptr<Player>>& players,
            std::vector<std::unique_ptr<Enemy>>& enemies,
            std::vector<std::unique_ptr<Bullet>>& bullets,
            std::vector<std::unique_ptr<EnemyBullet>>& enemyBullets,
            std::vector<std::unique_ptr<PowerUp>>& powerUps,
            std::vector<Explosion>& explosions,
            std::unique_ptr<Boss>& boss,
            bool bossActive,
            bool isMultiPlayer
        );

        // === Game Object Management ===
        void UpdateBullets(std::vector<std::unique_ptr<Bullet>>& bullets, float deltaTime);
        void UpdateEnemies(std::vector<std::unique_ptr<Enemy>>& enemies,
            std::vector<std::unique_ptr<EnemyBullet>>& enemyBullets,
            float deltaTime, Difficulty difficulty, bool bossActive);
        void UpdateEnemyBullets(std::vector<std::unique_ptr<EnemyBullet>>& enemyBullets, float deltaTime);
        void UpdatePowerUps(std::vector<std::unique_ptr<PowerUp>>& powerUps, float deltaTime);
        void UpdateExplosions(std::vector<Explosion>& explosions, float deltaTime);
        void UpdateBoss(std::unique_ptr<Boss>& boss, std::vector<std::unique_ptr<EnemyBullet>>& enemyBullets,
            std::vector<std::unique_ptr<Player>>& players, bool& bossActive, float deltaTime);

        // === Cleanup ===
        void ClearAllGameObjects(
            std::vector<std::unique_ptr<Enemy>>& enemies,
            std::vector<std::unique_ptr<Bullet>>& bullets,
            std::vector<std::unique_ptr<EnemyBullet>>& enemyBullets,
            std::vector<std::unique_ptr<PowerUp>>& powerUps,
            std::vector<Explosion>& explosions,
            std::unique_ptr<Boss>& boss,
            bool& bossActive
        );

    private:
        // === Kollisions-Hilfsfunktionen ===
        void HandleBulletEnemyCollisions(
            std::vector<std::unique_ptr<Player>>& players,
            std::vector<std::unique_ptr<Enemy>>& enemies,
            std::vector<std::unique_ptr<Bullet>>& bullets,
            std::vector<Explosion>& explosions,
            std::vector<std::unique_ptr<PowerUp>>& powerUps,
            Difficulty difficulty
        );

        void HandleBulletBossCollisions(
            std::vector<std::unique_ptr<Player>>& players,
            std::vector<std::unique_ptr<Bullet>>& bullets,
            std::unique_ptr<Boss>& boss
        );

        void HandlePlayerEnemyCollisions(
            std::vector<std::unique_ptr<Player>>& players,
            std::vector<std::unique_ptr<Enemy>>& enemies,
            bool isMultiPlayer
        );

        void HandlePlayerBulletCollisions(
            std::vector<std::unique_ptr<Player>>& players,
            std::vector<std::unique_ptr<EnemyBullet>>& enemyBullets,
            bool isMultiPlayer
        );

        void HandlePowerUpCollisions(
            std::vector<std::unique_ptr<Player>>& players,
            std::vector<std::unique_ptr<PowerUp>>& powerUps,
            bool& slowMotionActive,
            float& slowMotionTimer
        );

        // === PowerUp Anwendung ===
        void ApplyPowerUpToPlayer(PowerUpType type, Player* player,
            bool& slowMotionActive, float& slowMotionTimer);

        // === Hilfsfunktionen ===
        void CreateExplosion(std::vector<Explosion>& explosions, Vector2 position, float size);

        // === Member Variables für globale Effekte ===
        bool* slowMotionActiveRef = nullptr;
        float* slowMotionTimerRef = nullptr;
    };

} // namespace SpaceInvaders