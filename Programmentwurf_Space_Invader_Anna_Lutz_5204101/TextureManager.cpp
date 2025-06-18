#include "TextureManager.h"
#include <iostream>

namespace SpaceInvaders {

    // === Singleton Instance ===
    TextureManager& TextureManager::GetInstance() {
        static TextureManager instance;
        return instance;
    }

    // === Destruktor ===
    TextureManager::~TextureManager() {
        UnloadAllTextures();
    }

    // === Alle Texturen laden ===
    bool TextureManager::LoadAllTextures() {
        bool allLoaded = true;

        // === Avatar Texturen ===
        allLoaded &= LoadTexture("avatar_blue", "grafiken/avatar_blue.png");
        allLoaded &= LoadTexture("avatar_green", "grafiken/avatar_green.png");
        allLoaded &= LoadTexture("avatar_red", "grafiken/avatar_red.png");
        allLoaded &= LoadTexture("avatar_yellow", "grafiken/avatar_yellow.png");
        allLoaded &= LoadTexture("avatar_purple", "grafiken/avatar_purple.png");

        // === Enemy Texturen ===
        allLoaded &= LoadTexture("enemy1", "grafiken/enemy1.png");
        allLoaded &= LoadTexture("enemy2", "grafiken/enemy2.png");
        allLoaded &= LoadTexture("enemy3", "grafiken/enemy3.png");
        allLoaded &= LoadTexture("enemy4", "grafiken/enemy4.png");
        allLoaded &= LoadTexture("enemy5", "grafiken/enemy5.png");

        // === Mod Icon Texturen ===
        allLoaded &= LoadTexture("mod_none", "grafiken/none.png");
        allLoaded &= LoadTexture("mod_fastfire", "grafiken/fastfire.png");
        allLoaded &= LoadTexture("mod_shield", "grafiken/shield.png");
        allLoaded &= LoadTexture("mod_tripleshot", "grafiken/triple_shot.png");
        allLoaded &= LoadTexture("mod_slowmo", "grafiken/slowmotion.png");

        // === UI Texturen ===
        allLoaded &= LoadTexture("life", "grafiken/life.png");
        allLoaded &= LoadTexture("coin", "grafiken/coin.png");
        allLoaded &= LoadTexture("explosion", "grafiken/explosion.png");

        // === Background & Boss ===
        allLoaded &= LoadTexture("background", "grafiken/background.png");
        allLoaded &= LoadTexture("boss", "grafiken/boss.png");

        if (!allLoaded) {
            std::cerr << "[FEHLER] Nicht alle Texturen konnten geladen werden!" << std::endl;
        }

        return allLoaded;
    }

    // === Alle Texturen entladen ===
    void TextureManager::UnloadAllTextures() {
        for (auto& pair : textures) {
            UnloadTexture(pair.second);
        }
        textures.clear();
    }

    // === Avatar Textur abrufen ===
    Texture2D TextureManager::GetAvatarTexture(AvatarType type) const {
        return GetTexture(GetAvatarFilename(type));
    }

    // === Enemy Textur abrufen ===
    Texture2D TextureManager::GetEnemyTexture(int enemyType) const {
        return GetTexture(GetEnemyFilename(enemyType));
    }

    // === Mod Icon Textur abrufen ===
    Texture2D TextureManager::GetModIconTexture(ModType type) const {
        return GetTexture(GetModIconFilename(type));
    }

    // === UI Texturen abrufen ===
    Texture2D TextureManager::GetLifeTexture() const {
        return GetTexture("life");
    }

    Texture2D TextureManager::GetCoinTexture() const {
        return GetTexture("coin");
    }

    Texture2D TextureManager::GetExplosionTexture() const {
        return GetTexture("explosion");
    }

    // === Background & Boss Texturen ===
    Texture2D TextureManager::GetBackgroundTexture() const {
        return GetTexture("background");
    }

    Texture2D TextureManager::GetBossTexture() const {
        return GetTexture("boss");
    }

    // === Skaliertes Zeichnen ===
    void TextureManager::DrawTextureScaled(const Texture2D& texture, float x, float y, float w, float h, Color tint) {
        Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
        Rectangle dest = { x, y, w, h };
        Vector2 origin = { 0, 0 };
        DrawTexturePro(texture, source, dest, origin, 0.0f, tint);
    }

    // === Private Hilfsfunktionen ===
    bool TextureManager::LoadTexture(const std::string& key, const std::string& filename) {
        Texture2D texture = ::LoadTexture(filename.c_str());

        if (texture.width == 0 || texture.height == 0) {
            std::cerr << "[FEHLER] Textur konnte nicht geladen werden: " << filename << std::endl;
            return false;
        }

        textures[key] = texture;
        return true;
    }

    Texture2D TextureManager::GetTexture(const std::string& key) const {
        auto it = textures.find(key);
        if (it != textures.end()) {
            return it->second;
        }

        // Fallback: Leere Textur zurückgeben
        std::cerr << "[WARNUNG] Textur nicht gefunden: " << key << std::endl;
        return { 0 };
    }

    // === Avatar Dateinamen ===
    std::string TextureManager::GetAvatarFilename(AvatarType type) const {
        switch (type) {
        case AVATAR_BLUE:   return "avatar_blue";
        case AVATAR_GREEN:  return "avatar_green";
        case AVATAR_RED:    return "avatar_red";
        case AVATAR_YELLOW: return "avatar_yellow";
        case AVATAR_PURPLE: return "avatar_purple";
        default:            return "avatar_blue";
        }
    }

    // === Enemy Dateinamen ===
    std::string TextureManager::GetEnemyFilename(int enemyType) const {
        if (enemyType >= 1 && enemyType <= 5) {
            return "enemy" + std::to_string(enemyType);
        }
        return "enemy1"; // Fallback
    }

    // === Mod Icon Dateinamen ===
    std::string TextureManager::GetModIconFilename(ModType type) const {
        switch (type) {
        case MOD_NONE:       return "mod_none";
        case MOD_FASTFIRE:   return "mod_fastfire";
        case MOD_SHIELD:     return "mod_shield";
        case MOD_TRIPLESHOT: return "mod_tripleshot";
        case MOD_SLOWMO:     return "mod_slowmo";
        default:             return "mod_none";
        }
    }

} // namespace SpaceInvaders