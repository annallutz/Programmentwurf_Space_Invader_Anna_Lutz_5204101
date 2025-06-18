#include "Game.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

namespace SpaceInvaders {

    // === Konstruktor ===
    Game::Game()
        : textureManager(TextureManager::GetInstance())
        , currentState(STATE_MAINMENU)
        , previousState(STATE_MAINMENU)
        , currentDifficulty(DIFF_MEDIUM)
        , isMultiPlayer(false)
        , gameInitialized(false)
        , gameTime(0.0f)
        , enemySpawnTimer(0.0f)
        , backgroundScroll(0.0f)
        , bossSpawnTimer(0.0f)
        , bossActive(false)
        , slowMotionActive(false)
        , slowMotionTimer(0.0f)
        , timeScale(1.0f)
        , spSetupState(SP_NAME_INPUT)
        , selectedAvatar(AVATAR_BLUE)
        , selectedMod(MOD_NONE)
        , player1Avatar(AVATAR_BLUE)
        , player2Avatar(AVATAR_GREEN)
        , coinsAwarded(false)
        , finalScore(0)
        , newHighscore(false)
    {
        srand((unsigned int)time(nullptr));
    }

    // === Destruktor ===
    Game::~Game() {
        Shutdown();
    }

    // === Initialisierung ===
    bool Game::Initialize() {
        InitWindow(GameConfig::SCREEN_WIDTH, GameConfig::SCREEN_HEIGHT, "Space Invaders");
        SetTargetFPS(GameConfig::TARGET_FPS);

        if (!textureManager.LoadAllTextures()) {
            std::cerr << "[FEHLER] Texturen konnten nicht geladen werden!" << std::endl;
            return false;
        }

        // Subsysteme erstellen
        saveSystem = std::make_unique<SaveSystem>();
        menuSystem = std::make_unique<MenuSystem>();
        avatarManager = std::make_unique<AvatarManager>();
        modManager = std::make_unique<ModManager>();
        gameRenderer = std::make_unique<GameRenderer>();
        gameplayManager = std::make_unique<GameplayManager>();

        // Subsysteme initialisieren
        if (!saveSystem->Initialize() || !menuSystem->Initialize() ||
            !avatarManager->Initialize() || !modManager->Initialize()) {
            std::cerr << "[FEHLER] Ein Subsystem konnte nicht initialisiert werden!" << std::endl;
            return false;
        }

        gameInitialized = true;
        std::cout << "[INFO] Spiel erfolgreich initialisiert!" << std::endl;
        return true;
    }

    // === Hauptschleife ===
    void Game::Run() {
        if (!gameInitialized) {
            std::cerr << "[FEHLER] Spiel wurde nicht korrekt initialisiert!" << std::endl;
            return;
        }

        while (!WindowShouldClose()) {
            float deltaTime = GetFrameTime();
            Update(deltaTime);
            Render();
        }
    }

    // === Aufräumen ===
    void Game::Shutdown() {
        gameplayManager->ClearAllGameObjects(enemies, bullets, enemyBullets, powerUps, explosions, boss, bossActive);
        textureManager.UnloadAllTextures();
        CloseWindow();
        std::cout << "[INFO] Spiel beendet." << std::endl;
    }

    // === Update ===
    void Game::Update(float deltaTime) {
        if (inputManager.IsExitPressed()) {
            CloseWindow();
            return;
        }

        switch (currentState) {
        case STATE_MAINMENU: UpdateMainMenu(deltaTime); break;
        case STATE_SINGLEPLAYER_SETUP: UpdateSinglePlayerSetup(deltaTime); break;
        case STATE_MULTIPLAYER_SETUP: UpdateMultiPlayerSetup(deltaTime); break;
        case STATE_DIFFICULTY_SELECTION: UpdateDifficultySelection(deltaTime); break;
        case STATE_STATISTICS: UpdateStatistics(deltaTime); break;
        case STATE_RULES: UpdateRules(deltaTime); break;
        case STATE_GAME: UpdateGameplay(deltaTime); break;
        case STATE_GAMEOVER: UpdateGameOver(deltaTime); break;
        case STATE_PAUSE: UpdatePause(deltaTime); break;
        }
    }

    // === Render ===
    void Game::Render() {
        switch (currentState) {
        case STATE_MAINMENU: RenderMainMenu(); break;
        case STATE_SINGLEPLAYER_SETUP: RenderSinglePlayerSetup(); break;
        case STATE_MULTIPLAYER_SETUP: RenderMultiPlayerSetup(); break;
        case STATE_DIFFICULTY_SELECTION: RenderDifficultySelection(); break;
        case STATE_STATISTICS: RenderStatistics(); break;
        case STATE_RULES: RenderRules(); break;
        case STATE_GAME: RenderGameplay(); break;
        case STATE_GAMEOVER: RenderGameOver(); break;
        case STATE_PAUSE: RenderPause(); break;
        }
    }

    // === State Updates (stark vereinfacht) ===
    void Game::UpdateMainMenu(float deltaTime) {
        int currentCoins = saveSystem->LoadCoins();
        auto result = menuSystem->UpdateMainMenu(deltaTime, currentCoins);

        switch (result) {
        case MenuSystem::MAIN_SINGLEPLAYER:
            currentState = STATE_SINGLEPLAYER_SETUP;
            spSetupState = SP_NAME_INPUT;
            playerName.clear();
            avatarManager->ResetSinglePlayerSelection();
            modManager->ResetSelection();
            break;
        case MenuSystem::MAIN_MULTIPLAYER:
            currentState = STATE_MULTIPLAYER_SETUP;
            avatarManager->ResetMultiPlayerSelection();
            break;
        case MenuSystem::MAIN_RULES: currentState = STATE_RULES; break;
        case MenuSystem::MAIN_STATISTICS: currentState = STATE_STATISTICS; break;
        case MenuSystem::MAIN_DIFFICULTY: currentState = STATE_DIFFICULTY_SELECTION; break;
        case MenuSystem::MAIN_EXIT: CloseWindow(); break;
        default: break;
        }
    }

    void Game::UpdateSinglePlayerSetup(float deltaTime) {
        switch (spSetupState) {
        case SP_NAME_INPUT: {
            inputManager.GetTextInput(playerName, 12);
            if (inputManager.GetNavigationInput() == InputManager::NAV_CONFIRM && !playerName.empty()) {
                spSetupState = SP_AVATAR_SELECTION;
            }
            if (inputManager.GetNavigationInput() == InputManager::NAV_BACK) {
                currentState = STATE_MAINMENU;
            }
            break;
        }
        case SP_AVATAR_SELECTION: {
            int currentCoins = saveSystem->LoadCoins();
            auto result = avatarManager->UpdateSinglePlayerSelection(deltaTime, currentCoins);
            saveSystem->SaveCoins(currentCoins);

            if (result == AvatarManager::SP_AVATAR_CHOSEN) {
                selectedAvatar = avatarManager->GetSelectedAvatar();
                spSetupState = SP_MOD_SELECTION;
            }
            else if (result == AvatarManager::SP_BACK_PRESSED) {
                spSetupState = SP_NAME_INPUT;
            }
            break;
        }
        case SP_MOD_SELECTION: {
            int currentCoins = saveSystem->LoadCoins();
            auto result = modManager->UpdateModSelection(deltaTime, currentCoins);
            saveSystem->SaveCoins(currentCoins);

            if (result == ModManager::MOD_CHOSEN) {
                selectedMod = modManager->GetSelectedMod();
                InitializeSinglePlayer();
                currentState = STATE_GAME;
            }
            else if (result == ModManager::BACK_PRESSED) {
                spSetupState = SP_AVATAR_SELECTION;
            }
            break;
        }
        }
    }

    void Game::UpdateMultiPlayerSetup(float deltaTime) {
        auto result = avatarManager->UpdateMultiPlayerSelection(deltaTime);

        if (result == AvatarManager::MP_BOTH_CHOSEN) {
            player1Avatar = avatarManager->GetPlayer1Avatar();
            player2Avatar = avatarManager->GetPlayer2Avatar();
            InitializeMultiPlayer();
            currentState = STATE_GAME;
        }
        else if (result == AvatarManager::MP_BACK_PRESSED) {
            currentState = STATE_MAINMENU;
        }
    }

    void Game::UpdateDifficultySelection(float deltaTime) {
        auto result = menuSystem->UpdateDifficultySelection(deltaTime);

        if (result == MenuSystem::DIFF_CHOSEN) {
            currentDifficulty = menuSystem->GetSelectedDifficulty();
            currentState = STATE_MAINMENU;
        }
        else if (result == MenuSystem::DIFF_BACK) {
            currentState = STATE_MAINMENU;
        }
    }

    void Game::UpdateStatistics(float deltaTime) {
        if (menuSystem->UpdateStatistics(deltaTime)) {
            currentState = STATE_MAINMENU;
        }
    }

    void Game::UpdateRules(float deltaTime) {
        if (menuSystem->UpdateRules(deltaTime)) {
            currentState = STATE_MAINMENU;
        }
    }

    void Game::UpdateGameplay(float deltaTime) {
        // Pause prüfen
        if (inputManager.IsPausePressed()) {
            previousState = currentState;
            currentState = STATE_PAUSE;
            return;
        }

        // Time Scale für SlowMotion
        timeScale = slowMotionActive ? 0.4f : 1.0f;
        if (slowMotionActive) {
            slowMotionTimer -= deltaTime;
            if (slowMotionTimer <= 0.0f) {
                slowMotionActive = false;
                timeScale = 1.0f;
            }
        }

        float scaledDeltaTime = deltaTime * timeScale;
        gameTime += deltaTime;

        // Hintergrund scrollen
        float scrollSpeed = GameConfig::GetBackgroundScrollSpeed(currentDifficulty);
        backgroundScroll += scrollSpeed * deltaTime;
        if (backgroundScroll >= textureManager.GetBackgroundTexture().height) {
            backgroundScroll = 0.0f;
        }

        // Player Input und Updates
        HandlePlayerInput(deltaTime);

        // Enemy Spawning
        enemySpawnTimer += deltaTime;
        if (enemySpawnTimer >= GameConfig::GetEnemySpawnInterval(currentDifficulty) && !bossActive) {
            gameplayManager->SpawnEnemy(enemies, currentDifficulty);
            enemySpawnTimer = 0.0f;
        }

        // Boss Spawning
        bossSpawnTimer += deltaTime;
        if (!bossActive && bossSpawnTimer >= GameConfig::GetBossSpawnTime(currentDifficulty)) {
            gameplayManager->SpawnBoss(boss, bossActive, currentDifficulty);
            bossSpawnTimer = 0.0f;
        }

        // Gameplay Manager übernimmt komplette Update-Logik
        gameplayManager->UpdateGameplay(players, enemies, bullets, enemyBullets, powerUps, explosions,
            boss, bossActive, deltaTime, scaledDeltaTime, currentDifficulty, isMultiPlayer);

        // Game Over prüfen
        bool gameOver = (!players.empty() && players[0] && players[0]->GetLives() <= 0);
        if (gameOver) {
            finalScore = 0;
            for (const auto& player : players) {
                if (player) finalScore += player->GetScore();
            }

            if (!isMultiPlayer) {
                // Singleplayer: Coins und Statistiken
                int coinsEarned = finalScore / 2;
                int currentCoins = saveSystem->LoadCoins();
                saveSystem->SaveCoins(currentCoins + coinsEarned);

                if (!players.empty() && players[0]) {
                    saveSystem->SaveHighscore(players[0]->GetName(), finalScore,
                        selectedAvatar, selectedMod, currentDifficulty);
                }

                saveSystem->UpdateGameStatistics(finalScore, coinsEarned, 0, false,
                    gameTime, selectedAvatar, currentDifficulty);
            }
            else {
                // Multiplayer: Nur Bestscore
                int currentBest = saveSystem->LoadBestMultiplayerScore();
                if (finalScore > currentBest) {
                    saveSystem->SaveBestMultiplayerScore(finalScore);
                    newHighscore = true;
                }
            }

            currentState = STATE_GAMEOVER;
        }
    }

    void Game::UpdateGameOver(float deltaTime) {
        if (menuSystem->UpdateGameOver(deltaTime, finalScore, isMultiPlayer)) {
            // Reset
            gameplayManager->ClearAllGameObjects(enemies, bullets, enemyBullets, powerUps, explosions, boss, bossActive);
            players.clear();
            gameTime = 0.0f;
            enemySpawnTimer = 0.0f;
            bossSpawnTimer = 0.0f;
            slowMotionActive = false;
            coinsAwarded = false;
            newHighscore = false;
            currentState = STATE_MAINMENU;
        }
    }

    void Game::UpdatePause(float deltaTime) {
        auto result = menuSystem->UpdatePause(deltaTime);

        if (result == MenuSystem::PAUSE_CONTINUE) {
            currentState = previousState;
        }
        else if (result == MenuSystem::PAUSE_MAIN_MENU) {
            gameplayManager->ClearAllGameObjects(enemies, bullets, enemyBullets, powerUps, explosions, boss, bossActive);
            players.clear();
            currentState = STATE_MAINMENU;
        }
    }

    // === Gameplay-Setup (vereinfacht) ===
    void Game::InitializeSinglePlayer() {
        isMultiPlayer = false;

        Vector2 startPos = {
            GameConfig::SCREEN_WIDTH / 2 - Player::AVATAR_WIDTH / 2,
            GameConfig::SCREEN_HEIGHT - Player::AVATAR_HEIGHT - 20
        };

        players.clear();
        auto player = std::make_unique<Player>(selectedAvatar, startPos);
        player->SetName(playerName);
        player->ApplyStartMod(selectedMod);

        if (selectedMod == MOD_SLOWMO) {
            slowMotionActive = true;
            slowMotionTimer = GameConfig::POWERUP_DURATION;
        }

        players.push_back(std::move(player));

        gameplayManager->ClearAllGameObjects(enemies, bullets, enemyBullets, powerUps, explosions, boss, bossActive);
        gameTime = 0.0f;
        enemySpawnTimer = 0.0f;
        bossSpawnTimer = 0.0f;
    }

    void Game::InitializeMultiPlayer() {
        isMultiPlayer = true;

        Vector2 p1StartPos = {
            3 * GameConfig::SCREEN_WIDTH / 4 - Player::AVATAR_WIDTH / 2,
            GameConfig::SCREEN_HEIGHT - Player::AVATAR_HEIGHT - 20
        };
        Vector2 p2StartPos = {
            GameConfig::SCREEN_WIDTH / 4 - Player::AVATAR_WIDTH / 2,
            GameConfig::SCREEN_HEIGHT - Player::AVATAR_HEIGHT - 20
        };

        players.clear();
        auto player1 = std::make_unique<Player>(player1Avatar, p1StartPos);
        auto player2 = std::make_unique<Player>(player2Avatar, p2StartPos);

        player1->SetName("Player 1");
        player2->SetName("Player 2");

        players.push_back(std::move(player1));
        players.push_back(std::move(player2));

        gameplayManager->ClearAllGameObjects(enemies, bullets, enemyBullets, powerUps, explosions, boss, bossActive);
        gameTime = 0.0f;
        enemySpawnTimer = 0.0f;
        bossSpawnTimer = 0.0f;
    }

    void Game::HandlePlayerInput(float deltaTime) {
        // Player 1 Input
        if (!players.empty() && players[0]) {
            auto p1Input = inputManager.GetPlayer1Input();
            bool p1Left = (p1Input == InputManager::PLAYER_LEFT);
            bool p1Right = (p1Input == InputManager::PLAYER_RIGHT);
            bool p1Shoot = (p1Input == InputManager::PLAYER_SHOOT);

            players[0]->HandleInput(p1Left, p1Right, p1Shoot);
            players[0]->Update(deltaTime, currentDifficulty);
        }

        // Player 2 Input (Multiplayer)
        if (isMultiPlayer && players.size() > 1 && players[1]) {
            auto p2Input = inputManager.GetPlayer2Input();
            bool p2Left = (p2Input == InputManager::PLAYER_LEFT);
            bool p2Right = (p2Input == InputManager::PLAYER_RIGHT);
            bool p2Shoot = (p2Input == InputManager::PLAYER_SHOOT);

            players[1]->HandleInput(p2Left, p2Right, p2Shoot);
            players[1]->Update(deltaTime, currentDifficulty);
        }

        // Bullets erstellen (vereinfacht)
        CreatePlayerBullets();
    }

    void Game::CreatePlayerBullets() {
        for (size_t i = 0; i < players.size(); i++) {
            if (!players[i] || !players[i]->WantsToShoot()) continue;

            Vector2 shootPos = {
                players[i]->GetPosition().x + Player::AVATAR_WIDTH / 2,
                players[i]->GetPosition().y
            };

            float bulletSpeed = 500.0f * players[i]->GetBulletSpeedMultiplier();

            if (players[i]->IsTripleShotActive()) {
                bullets.push_back(std::make_unique<Bullet>((int)i, shootPos, bulletSpeed, Bullet::TRIPLE_SHOT));
                bullets.push_back(std::make_unique<Bullet>((int)i, Vector2{ shootPos.x - 30, shootPos.y }, bulletSpeed, Bullet::TRIPLE_SHOT));
                bullets.push_back(std::make_unique<Bullet>((int)i, Vector2{ shootPos.x + 30, shootPos.y }, bulletSpeed, Bullet::TRIPLE_SHOT));
            }
            else {
                Bullet::BulletType type = players[i]->IsFastFireActive() ? Bullet::FAST_FIRE : Bullet::NORMAL;
                bullets.push_back(std::make_unique<Bullet>((int)i, shootPos, bulletSpeed, type));
            }

            players[i]->ResetShootRequest();
        }
    }

    // === Render Functions (delegiert an Systeme) ===
    void Game::RenderMainMenu() {
        int currentCoins = saveSystem->LoadCoins();
        menuSystem->RenderMainMenu(currentCoins);
    }

    void Game::RenderSinglePlayerSetup() {
        BeginDrawing();
        ClearBackground(BLACK);

        switch (spSetupState) {
        case SP_NAME_INPUT: {
            const char* title = "Name eingeben:";
            int titleWidth = MeasureText(title, 40);
            DrawText(title, (GameConfig::SCREEN_WIDTH - titleWidth) / 2, 200, 40, WHITE);

            Rectangle inputBox = { GameConfig::SCREEN_WIDTH / 2 - 200, 300, 400, 50 };
            DrawRectangleRec(inputBox, Fade(DARKGRAY, 0.8f));
            DrawRectangleLinesEx(inputBox, 2, WHITE);

            const char* nameText = playerName.empty() ? "" : playerName.c_str();
            DrawText(nameText, inputBox.x + 10, inputBox.y + 10, 30, WHITE);

            if (fmod(GetTime(), 1.0) < 0.5) {
                int textWidth = MeasureText(nameText, 30);
                DrawText("_", inputBox.x + 10 + textWidth, inputBox.y + 10, 30, WHITE);
            }

            DrawText("ENTER: Bestaetigen | ESC: Zurueck",
                GameConfig::SCREEN_WIDTH / 2 - 150, 400, 20, GRAY);
            break;
        }
        case SP_AVATAR_SELECTION:
            avatarManager->RenderSinglePlayerSelection();
            break;
        case SP_MOD_SELECTION: {
            int currentCoins = saveSystem->LoadCoins();
            modManager->RenderModSelection(currentCoins);
            break;
        }
        }

        EndDrawing();
    }

    void Game::RenderMultiPlayerSetup() {
        avatarManager->RenderMultiPlayerSelection();
    }

    void Game::RenderDifficultySelection() {
        menuSystem->RenderDifficultySelection();
    }

    void Game::RenderStatistics() {
        menuSystem->RenderStatistics();
    }

    void Game::RenderRules() {
        menuSystem->RenderRules();
    }

    void Game::RenderGameplay() {
        gameRenderer->RenderGameplay(players, enemies, bullets, enemyBullets, powerUps, explosions,
            boss, bossActive, slowMotionActive, backgroundScroll,
            currentDifficulty, isMultiPlayer);
    }

    void Game::RenderGameOver() {
        int coinsEarned = isMultiPlayer ? 0 : finalScore / 2;
        menuSystem->RenderGameOver(finalScore, isMultiPlayer, coinsEarned);
    }

    void Game::RenderPause() {
        RenderGameplay(); // Hintergrund
        menuSystem->RenderPause(); // Overlay
    }

} // namespace SpaceInvaders