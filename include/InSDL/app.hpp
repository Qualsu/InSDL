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
            if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO)) {
                setError("SDL_Init");
                return;
            }
            windowData.width = width;
            windowData.height = height;
            windowData.name = name;

            windowHandle = SDL_CreateWindow(name.c_str(), width, height, 0);
            if (!windowHandle) {
                setError("SDL_CreateWindow");
            }
        }

        void setError(const std::string& context) {
            const char* sdlError = SDL_GetError();
            errorMessage = context + " failed";
            if (sdlError && sdlError[0] != '\0') {
                errorMessage += ": ";
                errorMessage += sdlError;
            }
        }

        Uint64 lastFrameTicksNs = 0;
        float deltaTimeSeconds = 0.0f;
        std::string errorMessage;
        SDL_Window *windowHandle = nullptr;
        SDL_Surface *windowSurface = nullptr;
        SDL_Renderer *renderer = nullptr;
    public:
        bool quit = false; // flag for quitting the application
        std::vector<keyBindStruct> keyBindings;
        std::vector<keyBindStruct> keyUpBindings;
        std::vector<mouseBindStruct> mouseBindings;
        std::vector<mouseMotionBindStruct> mouseMotionBindings;
        inputState input;
        colorStruct color;
        windowStruct windowData;
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
            errorMessage.clear();
            createWindow(width, height, name);
            if (!windowHandle) {
                return;
            }

            font = fontPath.empty() ? font : fontPath;

            if (surface) {
                windowSurface = SDL_GetWindowSurface(windowHandle);
                if (!windowSurface) {
                    setError("SDL_GetWindowSurface");
                    return;
                }
                if (!SDL_FillSurfaceRect(windowSurface, NULL, SDL_MapSurfaceRGB(windowSurface, 0, 0, 0))) {
                    setError("SDL_FillSurfaceRect");
                    return;
                }
            } else {
                renderer = SDL_CreateRenderer(windowHandle, NULL);
                if (!renderer) {
                    setError("SDL_CreateRenderer");
                    return;
                }
                if (!TTF_Init()) {
                    setError("TTF_Init");
                    return;
                }
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
         * @brief Returns true if the application was initialized successfully.
         */
        bool ok() const {
            return errorMessage.empty() && windowHandle && (renderer || windowSurface);
        }

        /**
         * @brief Returns the last SDL-related error message.
         */
        std::string error() const {
            return errorMessage;
        }

        /**
         * @brief Get SDL_Window
         */
        SDL_Window* window() const { return windowHandle; }

        /**
         * @brief Get SDL_Renderer
         */
        SDL_Renderer* renderer() const { return renderer; }

        /**
         * @brief Get SDL_Surface
         */
        SDL_Surface* surface() const { return windowSurface; }

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

            if (renderer) {
                if (!SDL_SetRenderDrawColor(renderer, r, g, b, 255)) {
                    setError("SDL_SetRenderDrawColor");
                    return;
                }
                if (!SDL_RenderClear(renderer)) {
                    setError("SDL_RenderClear");
                }
            } else if (windowSurface) {
                if (!SDL_FillSurfaceRect(windowSurface, NULL, SDL_MapSurfaceRGB(windowSurface, r, g, b))) {
                    setError("SDL_FillSurfaceRect");
                }
            } else {
                errorMessage = "Cannot fill: app is not initialized";
            }
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
            if (renderer) {
                if (!SDL_RenderPresent(renderer)) {
                    setError("SDL_RenderPresent");
                }
            } else if (windowHandle) {
                if (!SDL_UpdateWindowSurface(windowHandle)) {
                    setError("SDL_UpdateWindowSurface");
                }
            } else {
                errorMessage = "Cannot update: app is not initialized";
            }

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
            if (!windowHandle) {
                errorMessage = "Cannot set icon: app is not initialized";
                return;
            }
            if (!iconSurface) {
                errorMessage = "Cannot set icon: texture surface is empty";
                return;
            }
            if (!SDL_SetWindowIcon(windowHandle, iconSurface)) {
                setError("SDL_SetWindowIcon");
            }
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
            windowData.width = width == -1 ? windowData.width : width;
            windowData.height = height == -1 ? windowData.height : height;
            windowData.name = name.empty() ? windowData.name : name;

            if (!windowHandle) {
                errorMessage = "Cannot change window: app is not initialized";
                return;
            }
            if (!SDL_SetWindowSize(windowHandle, windowData.width, windowData.height)) {
                setError("SDL_SetWindowSize");
                return;
            }
            if (!SDL_SetWindowTitle(windowHandle, windowData.name.c_str())) {
                setError("SDL_SetWindowTitle");
            }
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
