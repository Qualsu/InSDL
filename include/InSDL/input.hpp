#ifndef INSDL_INPUT
#define INSDL_INPUT

#include <array>

#include <SDL3/SDL.h>

namespace insdl {

/**
 * @brief Stores keyboard and mouse state for the current frame
 *
 * Call insdl::handleEvent(app) once per frame before reading this state
 */
class inputState {
    private:
        std::array<bool, SDL_SCANCODE_COUNT> keyDown{};
        std::array<bool, SDL_SCANCODE_COUNT> keyPressed{};
        std::array<bool, SDL_SCANCODE_COUNT> keyReleased{};
        float mousePositionX = 0.0f;
        float mousePositionY = 0.0f;

        static bool isValidScancode(SDL_Scancode key) {
            return key >= 0 && key < SDL_SCANCODE_COUNT;
        }

    public:
        /**
         * @brief Starts a new input frame
         */
        void beginFrame() {
            keyPressed.fill(false);
            keyReleased.fill(false);
            SDL_GetMouseState(&mousePositionX, &mousePositionY);
        }

        /**
         * @brief Marks a key as currently held down.
         *
         * @param key SDL scancode, for example SDL_SCANCODE_W.
         */
        void setKeyDown(SDL_Scancode key) {
            if (!isValidScancode(key)) {
                return;
            }

            if (!keyDown[key]) {
                keyPressed[key] = true;
            }
            keyDown[key] = true;
        }

        /**
         * @brief Marks a key as released.
         *
         * @param key SDL scancode, for example SDL_SCANCODE_ESCAPE.
         */
        void setKeyUp(SDL_Scancode key) {
            if (!isValidScancode(key)) {
                return;
            }

            keyDown[key] = false;
            keyReleased[key] = true;
        }

        /**
         * @brief Stores the latest mouse position in window coordinates
         *
         * @param x Mouse X coordinate in pixels.
         * @param y Mouse Y coordinate in pixels.
         */
        void setMousePosition(float x, float y) {
            mousePositionX = x;
            mousePositionY = y;
        }

        /**
         * @brief Checks whether a key is currently held down.
         *
         * @param key SDL scancode, for example SDL_SCANCODE_A.
         * @return true while the key is held down.
         */
        bool isKeyDown(SDL_Scancode key) const {
            return isValidScancode(key) && keyDown[key];
        }

        /**
         * @brief Checks whether a key was pressed during this frame.
         *
         * @param key SDL scancode, for example SDL_SCANCODE_SPACE.
         * @return true only on the first frame of the key press.
         */
        bool isKeyPressed(SDL_Scancode key) const {
            return isValidScancode(key) && keyPressed[key];
        }

        /**
         * @brief Checks whether a key was released during this frame.
         *
         * @param key SDL scancode, for example SDL_SCANCODE_ESCAPE.
         * @return true only on the frame where the key is released.
         */
        bool isKeyReleased(SDL_Scancode key) const {
            return isValidScancode(key) && keyReleased[key];
        }

        /**
         * @brief Returns the current mouse X coordinate as an integer.
         */
        int mouseX() const {
            return static_cast<int>(mousePositionX);
        }

        /**
         * @brief Returns the current mouse Y coordinate as an integer.
         */
        int mouseY() const {
            return static_cast<int>(mousePositionY);
        }

        /**
         * @brief Returns the current mouse X coordinate as a float.
         */
        float mouseXf() const {
            return mousePositionX;
        }

        /**
         * @brief Returns the current mouse Y coordinate as a float.
         */
        float mouseYf() const {
            return mousePositionY;
        }
};

using InputState = inputState;

}

#endif
