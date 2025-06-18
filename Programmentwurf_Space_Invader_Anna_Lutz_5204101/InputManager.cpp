#include "InputManager.h"
#include <algorithm>
#include <cmath>

namespace SpaceInvaders {

    // === Navigation Input ===
    InputManager::NavInput InputManager::GetNavigationInput() {
        // Tastatur-Navigation
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) return NAV_UP;
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) return NAV_DOWN;
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) return NAV_LEFT;
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) return NAV_RIGHT;
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) return NAV_CONFIRM;
        if (IsKeyPressed(KEY_ESCAPE) || IsKeyPressed(KEY_BACKSPACE)) return NAV_BACK;

        return NAV_NONE;
    }

    // === Player 1 Input (Pfeiltasten) ===
    InputManager::PlayerInput InputManager::GetPlayer1Input() {
        if (IsKeyDown(KEY_LEFT)) return PLAYER_LEFT;
        if (IsKeyDown(KEY_RIGHT)) return PLAYER_RIGHT;
        if (IsKeyDown(KEY_UP) || IsKeyPressed(KEY_UP)) return PLAYER_SHOOT;
        return PLAYER_NONE;
    }

    // === Player 2 Input (WASD) ===
    InputManager::PlayerInput InputManager::GetPlayer2Input() {
        if (IsKeyDown(KEY_A)) return PLAYER_LEFT;
        if (IsKeyDown(KEY_D)) return PLAYER_RIGHT;
        if (IsKeyDown(KEY_W) || IsKeyPressed(KEY_W)) return PLAYER_SHOOT;
        return PLAYER_NONE;
    }

    // === Maus-Unterstützung ===
    bool InputManager::IsMouseOverRectangle(Rectangle rect) {
        Vector2 mousePos = GetMousePosition();
        return CheckCollisionPointRec(mousePos, rect);
    }

    bool InputManager::IsMouseClicked() {
        return IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    }

    Vector2 InputManager::GetMousePosition() {
        return ::GetMousePosition();
    }

    // === Key Hold/Repeat System ===
    bool InputManager::IsKeyPressedOrHeld(int key, float holdDelay, float repeatRate) {
        if (key < 0 || key >= 512) return false;

        bool keyDown = IsKeyDown(key);
        bool keyPressed = IsKeyPressed(key);

        if (keyPressed) {
            keyHoldTimers[key] = 0.0f;
            keyRepeatTimers[key] = 0.0f;
            keyHeldStates[key] = true;
            return true;
        }

        if (keyDown && keyHeldStates[key]) {
            keyHoldTimers[key] += GetFrameTime();

            if (keyHoldTimers[key] >= holdDelay) {
                keyRepeatTimers[key] += GetFrameTime();

                if (keyRepeatTimers[key] >= repeatRate) {
                    keyRepeatTimers[key] = 0.0f;
                    return true;
                }
            }
        }
        else {
            keyHoldTimers[key] = 0.0f;
            keyRepeatTimers[key] = 0.0f;
            keyHeldStates[key] = false;
        }

        return false;
    }

    // === Spezielle Tasten ===
    bool InputManager::IsPausePressed() {
        return IsKeyPressed(KEY_P);
    }

    bool InputManager::IsExitPressed() {
        return WindowShouldClose() || IsKeyPressed(KEY_ESCAPE);
    }

    // === Text-Eingabe ===
    bool InputManager::GetTextInput(std::string& text, int maxLength) {
        bool textChanged = false;

        int key = GetCharPressed();
        while (key > 0) {
            // Druckbare Zeichen hinzufügen
            if ((key >= 32) && (key <= 125) && text.length() < maxLength) {
                text += (char)key;
                textChanged = true;
            }
            key = GetCharPressed();
        }

        // Backspace behandeln
        if (IsKeyPressed(KEY_BACKSPACE) && !text.empty()) {
            text.pop_back();
            textChanged = true;
        }

        return textChanged;
    }

    void InputManager::ResetTextInput() {
        currentTextInput.clear();
    }

    // === Animierte Button-Funktion ===
    bool UpdateAnimatedButton(Rectangle rect, ButtonState& state,
        Color baseColor, Color hoverColor, Color pressedColor,
        float deltaTime) {

        InputManager inputManager;
        bool mouseOver = inputManager.IsMouseOverRectangle(rect);
        bool mouseClicked = inputManager.IsMouseClicked();

        // State Update
        state.isHovered = mouseOver;
        state.isPressed = mouseOver && IsMouseButtonDown(MOUSE_BUTTON_LEFT);

        // Animation der Skalierung (smooth lerp)
        float targetScale = mouseOver ? 1.1f : 1.0f;
        float lerpSpeed = 8.0f;
        state.hoverScale += (targetScale - state.hoverScale) * lerpSpeed * deltaTime;

        // Farb-Animation
        if (state.isPressed) {
            state.currentColor = pressedColor;
        }
        else if (state.isHovered) {
            state.currentColor = hoverColor;
        }
        else {
            state.currentColor = baseColor;
        }

        // Return true wenn Button geklickt wurde
        return mouseOver && mouseClicked;
    }

} // namespace SpaceInvaders