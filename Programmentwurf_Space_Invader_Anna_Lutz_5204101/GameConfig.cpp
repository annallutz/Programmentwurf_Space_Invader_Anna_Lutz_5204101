#include "GameConfig.h"

namespace SpaceInvaders {

    // === Schwierigkeitsgrad-abhängige Werte ===
    float GameConfig::GetEnemySpawnInterval(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return ENEMY_SPAWN_INTERVAL_EASY;
        case DIFF_MEDIUM: return ENEMY_SPAWN_INTERVAL_MEDIUM;
        case DIFF_HARD:   return ENEMY_SPAWN_INTERVAL_HARD;
        default:          return ENEMY_SPAWN_INTERVAL_MEDIUM;
        }
    }

    float GameConfig::GetEnemySpeed(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return ENEMY_SPEED_EASY;
        case DIFF_MEDIUM: return ENEMY_SPEED_MEDIUM;
        case DIFF_HARD:   return ENEMY_SPEED_HARD;
        default:          return ENEMY_SPEED_MEDIUM;
        }
    }

    float GameConfig::GetEnemyShootCooldown(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return ENEMY_SHOOT_COOLDOWN_EASY;
        case DIFF_MEDIUM: return ENEMY_SHOOT_COOLDOWN_MEDIUM;
        case DIFF_HARD:   return ENEMY_SHOOT_COOLDOWN_HARD;
        default:          return ENEMY_SHOOT_COOLDOWN_MEDIUM;
        }
    }

    float GameConfig::GetEnemyBulletSpeed(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return ENEMY_BULLET_SPEED_EASY;
        case DIFF_MEDIUM: return ENEMY_BULLET_SPEED_MEDIUM;
        case DIFF_HARD:   return ENEMY_BULLET_SPEED_HARD;
        default:          return ENEMY_BULLET_SPEED_MEDIUM;
        }
    }

    float GameConfig::GetBackgroundScrollSpeed(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return BACKGROUND_SCROLL_SPEED_EASY;
        case DIFF_MEDIUM: return BACKGROUND_SCROLL_SPEED_MEDIUM;
        case DIFF_HARD:   return BACKGROUND_SCROLL_SPEED_HARD;
        default:          return BACKGROUND_SCROLL_SPEED_MEDIUM;
        }
    }

    float GameConfig::GetPlayerShootCooldown(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return PLAYER_SHOOT_COOLDOWN_EASY;
        case DIFF_MEDIUM: return PLAYER_SHOOT_COOLDOWN_MEDIUM;
        case DIFF_HARD:   return PLAYER_SHOOT_COOLDOWN_HARD;
        default:          return PLAYER_SHOOT_COOLDOWN_MEDIUM;
        }
    }

    float GameConfig::GetPlayerMovementSpeed(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return PLAYER_MOVEMENT_SPEED_EASY;
        case DIFF_MEDIUM: return PLAYER_MOVEMENT_SPEED_MEDIUM;
        case DIFF_HARD:   return PLAYER_MOVEMENT_SPEED_HARD;
        default:          return PLAYER_MOVEMENT_SPEED_MEDIUM;
        }
    }

    float GameConfig::GetPowerUpDropChance(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return POWERUP_DROP_CHANCE_EASY;
        case DIFF_MEDIUM: return POWERUP_DROP_CHANCE_MEDIUM;
        case DIFF_HARD:   return POWERUP_DROP_CHANCE_HARD;
        default:          return POWERUP_DROP_CHANCE_MEDIUM;
        }
    }

    float GameConfig::GetBossSpawnTime(Difficulty diff) {
        switch (diff) {
        case DIFF_EASY:   return BOSS_SPAWN_TIME_EASY;
        case DIFF_MEDIUM: return BOSS_SPAWN_TIME_MEDIUM;
        case DIFF_HARD:   return BOSS_SPAWN_TIME_HARD;
        default:          return BOSS_SPAWN_TIME_MEDIUM;
        }
    }

} // namespace SpaceInvaders