#include "Game.h"
#include <iostream>
#include <exception>

/**
 * Space Invaders - Hauptprogramm
 *
 * Ein modernes Space Invaders Spiel mit:
 * - SinglePlayer & MultiPlayer Modi
 * - 5 Avatare mit speziellen Fähigkeiten
 * - Modifikator-System mit kaufbaren Upgrades
 * - Maus + Tastatur Navigation
 * - Schwierigkeitsgrade
 * - Coin-System und Freischaltungen
 * - Boss-Kämpfe und PowerUps
 * - Vollständiges Save-System
 */

int main() {
    try {
        // Spiel-Instanz erstellen
        SpaceInvaders::Game game;

        // Spiel initialisieren
        if (!game.Initialize()) {
            std::cerr << "[KRITISCHER FEHLER] Spiel konnte nicht initialisiert werden!" << std::endl;
            return -1;
        }

        std::cout << "=== SPACE INVADERS ===" << std::endl;
        std::cout << "Spiel gestartet. Viel Spass!" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "STEUERUNG:" << std::endl;
        std::cout << "- Maus oder Pfeiltasten: Navigation" << std::endl;
        std::cout << "- ENTER: Bestaetigen" << std::endl;
        std::cout << "- ESC: Zurueck/Beenden" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "SINGLEPLAYER:" << std::endl;
        std::cout << "- Pfeiltasten Links/Rechts: Bewegen" << std::endl;
        std::cout << "- Pfeil Hoch: Schiessen" << std::endl;
        std::cout << "- P: Pause" << std::endl;
        std::cout << "" << std::endl;
        std::cout << "MULTIPLAYER:" << std::endl;
        std::cout << "- Player 1: A/D bewegen, W schiessen" << std::endl;
        std::cout << "- Player 2: Pfeiltasten bewegen, Pfeil Hoch schiessen" << std::endl;
        std::cout << "- P: Pause" << std::endl;
        std::cout << "======================" << std::endl;

        // Hauptschleife starten
        game.Run();

        std::cout << "[INFO] Spiel ordnungsgemäß beendet." << std::endl;
        return 0;

    }
    catch (const std::exception& e) {
        std::cerr << "[AUSNAHME] Unerwarteter Fehler: " << e.what() << std::endl;
        return -2;

    }
    catch (...) {
        std::cerr << "[KRITISCHER FEHLER] Unbekannter Fehler aufgetreten!" << std::endl;
        return -3;
    }
}