#ifndef INSDL_EVENT
#define INSDL_EVENT

#include <app.hpp>
#include <functional>

using namespace std;

/**
 * @note Not used
 * @brief Handles the application quit event
 *  
 * @param app Reference to your application object
 */
void exitEvent(app& app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            app.quit = true;
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
void handleEvent(app& app) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            app.quit = true;
        } 
         else if (event.type == SDL_EVENT_KEY_DOWN) {
            SDL_Scancode scancode = event.key.scancode;
            for (auto& binding : app.keyBindings) {
                if (binding.key == scancode) {
                    binding.func();
                    break;
                }
            }
        }
        else if (event.type == SDL_EVENT_KEY_UP) {
            SDL_Scancode scancode = event.key.scancode;
            for (auto &binding : app.keyUpBindings) {
                if (scancode == binding.key) {
                    binding.func();
                    break;
                }
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
            for (const auto& binding : app.mouseBindings) {
                if (binding.button == event.button.button) {
                    binding.func();
                    break;
                }
            }
        }
        else if (event.type == SDL_EVENT_MOUSE_MOTION) {
            int x = event.motion.x;
            int y = event.motion.y;
            for (const auto& binding : app.mouseMotionBindings) {
                binding.func(x, y);
            }
        }
    }
}

#endif