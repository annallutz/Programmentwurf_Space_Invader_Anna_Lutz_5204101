#pragma once

#include "raylib.h"
#include <string>

namespace SpaceInvaders {

    /**
     * Button-State f�r animierte Buttons
     */
    struct ButtonState {
        bool isHovered = false;
        bool isPressed = false;
        float hoverScale = 1.0f;
        Color currentColor = WHITE;
    };

    /**
     * @brief Zentrale Klasse f�r einheitliche Input-Behandlung (Tastatur + Maus)
     *
     * Der InputManager erm�glicht es, sowohl Tastatur- als auch Maus-Eingaben
     * einheitlich zu behandeln und stellt Hilfsfunktionen f�r Men�-Navigation bereit.
     */
    class InputManager {
    public:
        // === Navigation Input ===
        enum NavInput {
            NAV_UP,
            NAV_DOWN,
            NAV_LEFT,
            NAV_RIGHT,
            NAV_CONFIRM,
            NAV_BACK,
            NAV_NONE
        };

        // === Player Input ===
        enum PlayerInput {
            PLAYER_LEFT,
            PLAYER_RIGHT,
            PLAYER_SHOOT,
            PLAYER_NONE
        };

        // === Konstruktor ===
        InputManager() = default;

        // === Navigation Input (f�r Men�s) ===
        NavInput GetNavigationInput();

        // === Player Input (f�r Gameplay) ===
        PlayerInput GetPlayer1Input();
        PlayerInput GetPlayer2Input();

        // === Maus-Unterst�tzung f�r Men�s ===
        bool IsMouseOverRectangle(Rectangle rect);
        bool IsMouseClicked();
        Vector2 GetMousePosition();

        // === Spezielle Input-Funktionen ===
        bool IsKeyPressedOrHeld(int key, float holdDelay = 0.5f, float repeatRate = 0.1f);
        bool IsPausePressed();
        bool IsExitPressed();

        // === Text-Eingabe ===
        bool GetTextInput(std::string& text, int maxLength = 10);
        void ResetTextInput();

    private:
        // === Timer f�r Key-Repeat ===
        float keyHoldTimers[512] = { 0 };
        float keyRepeatTimers[512] = { 0 };
        bool keyHeldStates[512] = { false };

        // === Text-Eingabe State ===
        std::string currentTextInput = "";
    };

    /**
     * @brief Hilfsfunktion f�r animierte Buttons
     */
    bool UpdateAnimatedButton(Rectangle rect, ButtonState& state,
        Color baseColor = WHITE,
        Color hoverColor = YELLOW,
        Color pressedColor = ORANGE,
        float deltaTime = 0.0f);

} // namespace SpaceInvaders