#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include "TextureManager.h"
#include "InputManager.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Bullet.h"
#include "EnemyBullet.h"
#include "PowerUp.h"
#include "MenuSystem.h"
#include "SaveSystem.h"
#include "AvatarManager.h"
#include "ModManager.h"
#include "GameRenderer.h"
#include "GameplayManager.h"
#include <vector>
#include <memory>

namespace SpaceInvaders {

    /**
     * @brief Haupt-Spiel-Klasse - Koordiniert alle Subsysteme
     *
     * Das Game-Objekt ist verantwortlich für:
     * - Game Loop Management
     * - State Management (Menü, Spiel, Pause, etc.)
     * - Koordination aller Subsysteme
     * - Input-Handling auf höchster Ebene
     *
     * Gameplay-Details werden an GameplayManager delegiert,
     * Rendering wird an GameRenderer delegiert.
     */
    class Game {
    public:
        // === Konstruktor & Destruktor ===
        Game();
        ~Game();

        // === Haupt-Funktionen ===
        bool Initialize();
        void Run();
        void Shutdown();

    private:
        // === Core Update & Render ===
        void Update(float deltaTime);
        void Render();

        // === State-spezifische Updates (schlanker) ===
        void UpdateMainMenu(float deltaTime);
        void UpdateSinglePlayerSetup(float deltaTime);
        void UpdateMultiPlayerSetup(float deltaTime);
        void UpdateDifficultySelection(float deltaTime);
        void UpdateStatistics(float deltaTime);
        void UpdateRules(float deltaTime);
        void UpdateGameplay(float deltaTime);
        void UpdateGameOver(float deltaTime);
        void UpdatePause(float deltaTime);

        // === State-spezifische Renders (delegiert an Renderer) ===
        void RenderMainMenu();
        void RenderSinglePlayerSetup();
        void RenderMultiPlayerSetup();
        void RenderDifficultySelection();
        void RenderStatistics();
        void RenderRules();
        void RenderGameplay();
        void RenderGameOver();
        void RenderPause();

        // === Gameplay-Setup (vereinfacht) ===
        void InitializeSinglePlayer();
        void InitializeMultiPlayer();
        void HandlePlayerInput(float deltaTime);
        void CreatePlayerBullets();

        // === Core Systeme ===
        TextureManager& textureManager;
        InputManager inputManager;
        std::unique_ptr<MenuSystem> menuSystem;
        std::unique_ptr<SaveSystem> saveSystem;
        std::unique_ptr<AvatarManager> avatarManager;
        std::unique_ptr<ModManager> modManager;
        std::unique_ptr<GameRenderer> gameRenderer;
        std::unique_ptr<GameplayManager> gameplayManager;

        // === Game State ===
        GameState currentState;
        GameState previousState;
        Difficulty currentDifficulty;
        bool isMultiPlayer;
        bool gameInitialized;

        // === Timing ===
        float gameTime;
        float enemySpawnTimer;
        float backgroundScroll;
        float bossSpawnTimer;

        // === Players (vereinfacht zu Vector) ===
        std::vector<std::unique_ptr<Player>> players;

        // === Game Objects (delegiert an GameplayManager) ===
        std::vector<std::unique_ptr<Enemy>> enemies;
        std::vector<std::unique_ptr<Bullet>> bullets;
        std::vector<std::unique_ptr<EnemyBullet>> enemyBullets;
        std::vector<std::unique_ptr<PowerUp>> powerUps;
        std::vector<Explosion> explosions;

        // === Boss System ===
        std::unique_ptr<Boss> boss;
        bool bossActive;

        // === PowerUp System ===
        bool slowMotionActive;
        float slowMotionTimer;
        float timeScale;

        // === SinglePlayer Setup State ===
        enum SinglePlayerSetupState {
            SP_NAME_INPUT,
            SP_AVATAR_SELECTION,
            SP_MOD_SELECTION
        };
        SinglePlayerSetupState spSetupState;
        std::string playerName;
        AvatarType selectedAvatar;
        ModType selectedMod;

        // === MultiPlayer Setup State ===
        AvatarType player1Avatar;
        AvatarType player2Avatar;

        // === Game Over State ===
        bool coinsAwarded;
        int finalScore;
        bool newHighscore;
    };

} // namespace SpaceInvadersenderStatistics();
void RenderRules();
void RenderGameplay();
void RenderGameOver();
void RenderPause();

// === Gameplay-spezifische Funktionen ===
void InitializeSinglePlayer();
void InitializeMultiPlayer();
void SpawnEnemy();
void SpawnPowerUp(Vector2 position);
void HandleCollisions();
void HandlePlayerEnemyCollisions();
void HandleBulletEnemyCollisions();
void HandlePlayerBulletCollisions();
void HandlePowerUpCollisions();
void UpdateExplosions(float deltaTime);
void ClearAllGameObjects();

// === Boss System ===
void UpdateBoss(float deltaTime);
void SpawnBoss();

// === Rendering Hilfsfunktionen ===
void RenderBackground();
void RenderUI();
void RenderPowerUpIndicators();
void RenderCoinsDisplay(int x, int y, int fontSize);

// === Core Systeme ===
TextureManager& textureManager;
InputManager inputManager;
std::unique_ptr<MenuSystem> menuSystem;
std::unique_ptr<SaveSystem> saveSystem;
std::unique_ptr<AvatarManager> avatarManager;
std::unique_ptr<ModManager> modManager;

// === Game State ===
GameState currentState;
GameState previousState;
Difficulty currentDifficulty;
bool isMultiPlayer;
bool gameInitialized;

// === Timing ===
float gameTime;
float enemySpawnTimer;
float backgroundScroll;

// === Players ===
std::unique_ptr<Player> player1;
std::unique_ptr<Player> player2;

// === Game Objects ===
std::vector<std::unique_ptr<Enemy>> enemies;
std::vector<std::unique_ptr<Bullet>> bullets;
std::vector<std::unique_ptr<EnemyBullet>> enemyBullets;
std::vector<std::unique_ptr<PowerUp>> powerUps;
std::vector<Explosion> explosions;

// === Boss System ===
std::unique_ptr<Boss> boss;
bool bossActive;
float bossSpawnTimer;

// === PowerUp System ===
bool slowMotionActive;
float slowMotionTimer;
float timeScale;

// === SinglePlayer Setup State ===
enum SinglePlayerSetupState {
    SP_NAME_INPUT,
    SP_AVATAR_SELECTION,
    SP_MOD_SELECTION
};
SinglePlayerSetupState spSetupState;
std::string playerName;
AvatarType selectedAvatar;
ModType selectedMod;

// === MultiPlayer Setup State ===
enum MultiPlayerSetupState {
    MP_PLAYER1_AVATAR,
    MP_PLAYER2_AVATAR,
    MP_READY_TO_START
};
MultiPlayerSetupState mpSetupState;
AvatarType player1Avatar;
AvatarType player2Avatar;

// === UI State ===
ButtonState menuButtonStates[10]; // Für verschiedene Menü-Buttons
int currentMenuSelection;
bool waitingForKeyRelease;

// === Game Over State ===
bool coinsAwarded;
int finalScore;
bool newHighscore;
    };

} // namespace SpaceInvaders