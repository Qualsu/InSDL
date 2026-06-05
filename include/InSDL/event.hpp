#ifndef INSDL_EVENT
#define INSDL_EVENT

#include <InSDL/app.hpp>
#include <functional>

namespace insdl {

/**
 * @note Not used
 * @brief Handles the application quit event
 *
 * @param app Reference to your application object
 */
void exitEvent(app& application) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            application.quit = true;
        }
    }
}

/**
 * @brief Handles all main SDL events related to input and quitting
 * 
 * Processes quit events, key down/up events, mouse button presses, and mouse motion.
 * Calls the corresponding bound functions from the app object when events are detected.
 * 
 * @param app Reference to your application object
 */
void handleEvent(app& application) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            application.quit = true;
        } 
         else if (event.type == SDL_EVENT_KEY_DOWN) {
            SDL_Scancode scancode = event.key.scancode;
            for (auto& binding : application.keyBindings) {
                if (binding.key == scancode) {
                    binding.callback();
                    break;
                }
            }
        }
        else if (event.type == SDL_EVENT_KEY_UP) {
            SDL_Scancode scancode = event.key.scancode;
            for (auto &binding : application.keyUpBindings) {
                if (scancode == binding.key) {
                    binding.callback();
                    break;
                }
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            for (const auto& binding : application.mouseBindings) {
                if (binding.button == event.button.button) {
                    binding.callback();
                    break;
                }
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            int x = event.motion.x;
            int y = event.motion.y;
            for (const auto& binding : application.mouseMotionBindings) {
                binding.callback(x, y);
            }
        }
    }
}

} // namespace insdl

#endif
