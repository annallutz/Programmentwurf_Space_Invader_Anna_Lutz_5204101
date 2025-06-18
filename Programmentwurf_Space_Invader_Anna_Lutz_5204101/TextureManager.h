#pragma once

#include "raylib.h"
#include "GameConfig.h"
#include <unordered_map>
#include <string>

namespace SpaceInvaders {

    /**
     * @brief Zentrale Klasse für das Laden, Speichern und Verwalten aller Texturen
     *
     * Der TextureManager stellt sicher, dass alle Texturen nur einmal geladen werden
     * und einheitlich über das gesamte Spiel verwendet werden können.
     */
    class TextureManager {
    public:
        // === Singleton Pattern ===
        static TextureManager& GetInstance();

        // === Destruktor ===
        ~TextureManager();

        // === Haupt-Funktionen ===
        bool LoadAllTextures();
        void UnloadAllTextures();

        // === Avatar Texturen ===
        Texture2D GetAvatarTexture(AvatarType type) const;

        // === Enemy Texturen ===
        Texture2D GetEnemyTexture(int enemyType) const; // 0-4 für verschiedene Enemy-Typen

        // === UI Texturen ===
        Texture2D GetModIconTexture(ModType type) const;
        Texture2D GetLifeTexture() const;
        Texture2D GetCoinTexture() const;
        Texture2D GetExplosionTexture() const;

        // === Background & Boss ===
        Texture2D GetBackgroundTexture() const;
        Texture2D GetBossTexture() const;

        // === Hilfsfunktion für skaliertes Zeichnen ===
        static void DrawTextureScaled(const Texture2D& texture, float x, float y, float w, float h, Color tint = WHITE);

    private:
        // === Singleton - Private Konstruktor ===
        TextureManager() = default;
        TextureManager(const TextureManager&) = delete;
        TextureManager& operator=(const TextureManager&) = delete;

        // === Textur-Container ===
        std::unordered_map<std::string, Texture2D> textures;

        // === Private Hilfsfunktionen ===
        bool LoadTexture(const std::string& key, const std::string& filename);
        Texture2D GetTexture(const std::string& key) const;

        // === Avatar Dateinamen ===
        std::string GetAvatarFilename(AvatarType type) const;

        // === Enemy Dateinamen ===
        std::string GetEnemyFilename(int enemyType) const;

        // === Mod Icon Dateinamen ===
        std::string GetModIconFilename(ModType type) const;
    };

} // namespace SpaceInvaders