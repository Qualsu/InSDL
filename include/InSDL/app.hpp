#ifndef INSDL_APP
#define INSDL_APP

#include <string>
#include <iostream>
#include <map>
#include <functional>
#include <vector>
#include <filesystem>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <InSDL/input.hpp>
#include <InSDL/texture.hpp>

namespace insdl {

/**
 * @brief Class for managing an SDL application
 *
 * Responsible for creating a window, rendering, input, and other basic operations
 */
class app {
    private:
        static std::string defaultFontPath() {
            return (std::filesystem::path(__FILE__).parent_path() / "font.ttf").string();
        }

        struct keyBindStruct {
            SDL_Scancode key;
            std::function<void()> callback;
        };

        struct mouseBindStruct {
            Uint8 button;
            std::function<void()> callback;
        };

        struct mouseMotionBindStruct {
            std::function<void(int, int)> callback;
        };

        struct colorStruct {
            Uint8 r = 0;
            Uint8 g = 0;
            Uint8 b = 0;
        };

        struct windowStruct {
            int width;
            int height;
            std::string name;
        };
        
        /**
         * @brief Creates an SDL window with the specified parameters
         * 
         * @param width Width of the window
         * @param height Height of the window
         * @param name Name of the window
         */
        void createWindow(int width, int height, std::string name){
            SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
            window.width = width;
            window.height = height;
            window.name = name;

            windowHandle = SDL_CreateWindow(name.c_str(), width, height, 0);
        }
        Uint64 lastFrameTicksNs = 0;
        float deltaTimeSeconds = 0.0f;
    public:
        SDL_Window *windowHandle = nullptr;
        SDL_Surface *windowSurface = nullptr;
        SDL_Renderer *renderer = nullptr;
        bool quit = false; // flag for quitting the application
        std::vector<keyBindStruct> keyBindings;
        std::vector<keyBindStruct> keyUpBindings;
        std::vector<mouseBindStruct> mouseBindings;
        std::vector<mouseMotionBindStruct> mouseMotionBindings;
        inputState input;
        colorStruct color;
        windowStruct window;
        std::string font = defaultFontPath(); // default font for text rendering

        /**
         * @brief Initializes the application: creates a window, sets the rendering mode, and the default font
         * 
         * @param width Width of the window
         * @param height Height of the window
         * @param name Name of the window
         * @param surface Use Renderer (true) or Surface (false)
         * @param fontPath Path to the font file (optional)
         */
        void init(int width, int height, std::string name, bool surface = false, std::string fontPath = "") {
            createWindow(width, height, name);
            font = fontPath.empty() ? font : fontPath;

            if (surface) {
                windowSurface = SDL_GetWindowSurface(windowHandle);
                SDL_FillSurfaceRect(windowSurface, NULL, SDL_MapSurfaceRGB(windowSurface, 0, 0, 0));
            } else {
                renderer = SDL_CreateRenderer(windowHandle, NULL);
                TTF_Init();
            }

            lastFrameTicksNs = SDL_GetTicksNS();
            deltaTimeSeconds = 0.0f;
        }

        app() = default;
        app(const app&) = delete;
        app& operator=(const app&) = delete;
        app(app&&) = delete;
        app& operator=(app&&) = delete;

        ~app() {
            exit();
        }

        /**
         * @brief Clears the window with the currently stored color
         */
        void clear() {
            fill(color.r, color.g, color.b);
        }

        /**
         * @brief Fill the window with the specified color
         * 
         * @param r Red component (0-255)
         * @param g Green component (0-255)
         * @param b Blue component (0-255)
         */
        void fill(Uint8 r, Uint8 g, Uint8 b) {
            color.r = r;
            color.g = g;
            color.b = b;

            renderer != nullptr
                ? (SDL_SetRenderDrawColor(renderer, r, g, b, 255), SDL_RenderClear(renderer))
                : SDL_FillSurfaceRect(windowSurface, NULL, SDL_MapSurfaceRGB(windowSurface, r, g, b));
        }

        /**
         * @brief Backward-compatible alias for clear()
         */
        void fill() {
            clear();
        }
        
        /**
         * @brief Updates the contents of the window
         * 
         * Depending on the rendering mode, either the Renderer or Surface will be updated
         */
        void update() {
            renderer != nullptr ? SDL_RenderPresent(renderer) :  SDL_UpdateWindowSurface(windowHandle);

            const Uint64 now = SDL_GetTicksNS();
            deltaTimeSeconds = static_cast<float>(now - lastFrameTicksNs) / 1000000000.0f;
            lastFrameTicksNs = now;
        }

        /**
         * @brief Returns the duration of the previous frame in seconds
         */
        float deltaTime() const {
            return deltaTimeSeconds;
        }
        
        /**
         * @brief Sets the window icon in the console
         * 
         * @param icon Texture of the icon
         */
        void setIcon(const texture& icon) {
            SDL_Surface *iconSurface = icon.get().surface;
            SDL_SetWindowIcon(windowHandle, iconSurface);
        }

        /**
         * @brief Sets the path to the font
         * 
         * @param fontPath Path to the font file
         */
        void setFont(std::string fontPath){
            font = fontPath;
        }

        /**
         * @brief Changes the size and title of the window
         * 
         * @param width New width of the window (optional, to skip — -1)
         * @param height New height of the window (optional, to skip — -1)
         * @param name New title of the window (optional)
         */
        void change(int width = -1, int height = -1, std::string name = "") {
            window.width = width == -1 ? window.width : width;
            window.height = height == -1 ? window.height : height;
            window.name = name.empty() ? window.name : name;

            SDL_SetWindowSize(windowHandle, window.width, window.height);
            SDL_SetWindowTitle(windowHandle, window.name.c_str());
        }

        /**
         * @brief Binds a function to key press and release (binds keys)
         * 
         * @tparam Func Type of the press function
         * @tparam FuncUp Type of the release function
         * @param key Key code (SDL_Scancode)
         * @param func Function called on press
         * @param funcup Function called on release (optional)
         */
        template<typename Func, typename FuncUp>
        void bindKey(SDL_Scancode key, Func func, FuncUp funcUp = [](){}) {
            keyBindings.push_back({ key, func });
            keyUpBindings.push_back({ key, funcUp });
        }

        /**
         * @brief Binds a function to mouse button press
         * 
         * @tparam Func Type of the function
         * @param button Mouse button code
         * @param func Function called on press
         */
        template<typename Func>
        void bindMouseButton(Uint8 button, Func func) {
            mouseBindings.push_back({ button, func });
        }

        /**
         * @brief Binds a function to mouse movement
         * 
         * @tparam Func Type of the function
         * @param func Function with parameters (int x, int y)
         */
        template<typename Func>
        void bindMouseMotion(Func func) {
            mouseMotionBindings.push_back({ func });
        }

        /**
         * @brief Terminates the application, frees SDL resources
         */
        void exit() {
            if (renderer) {
                SDL_DestroyRenderer(renderer);
                renderer = nullptr;
            }
            if (windowHandle) {
                SDL_DestroyWindow(windowHandle);
                windowHandle = nullptr;
            }
            windowSurface = nullptr;
            lastFrameTicksNs = 0;
            deltaTimeSeconds = 0.0f;
            TTF_Quit();
            SDL_Quit();
        }
};

using App = app;

} // namespace insdl

#endif
