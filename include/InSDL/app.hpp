#ifndef APP
#define APP

#include <string>
#include <iostream>
#include <map>
#include <functional>
#include <vector>
#include <direct.h>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <texture.hpp>

using namespace std;

/**
 * @brief Class for managing an SDL application
 * 
 * Responsible for creating a window, rendering, input, and other basic operations
 */
class app {
    private:
        struct keyBindStruct {
            SDL_Scancode key;
            function<void()> func;
        };

        struct mouseBindStruct {
            Uint8 button;
            function<void()> func;
        };

        struct mouseMotionBindStruct {
            function<void(int, int)> func;
        };

        struct colorStruct {
            Uint8 r = 0;
            Uint8 g = 0;
            Uint8 b = 0;
        };

        struct windowStruct {
            int width;
            int height;
            string name;
        };
        
        /**
         * @brief Creates an SDL window with the specified parameters
         * 
         * @param width Width of the window
         * @param height Height of the window
         * @param name Name of the window
         */
        void createWindow(int width, int height, string name){
            SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
            window.width = width;
            window.height = height;
            window.name = name;

            Window = SDL_CreateWindow(name.c_str(), width, height, 0);
        }

        char buffer[1024];
        char* pathname = getcwd(buffer, 1024);
    public:
        SDL_Window *Window;
        SDL_Surface *Surface = nullptr;
        SDL_Renderer *Render = nullptr;
        bool quit = false; // flag for quitting the application
        vector<keyBindStruct> keyBindings;
        vector<keyBindStruct> keyUpBindings;
        vector<mouseBindStruct> mouseBindings;
        vector<mouseMotionBindStruct> mouseMotionBindings;
        colorStruct color;
        windowStruct window;
        string font = string(buffer) + "\\include\\InSDL\\font.ttf"; // default font for text rendering

        /**
         * @brief Initializes the application: creates a window, sets the rendering mode, and the default font
         * 
         * @param width Width of the window
         * @param height Height of the window
         * @param name Name of the window
         * @param render Use Renderer (true) or Surface (false)
         * @param fontpath Path to the font file (optional)
         */
        void init(int width, int height, string name, bool surface = false, string fontpath = "") {
            createWindow(width, height, name);
            font = fontpath.empty() ? font : fontpath;

            if (surface) {
                Surface = SDL_GetWindowSurface(Window);
                SDL_FillSurfaceRect(Surface, NULL, SDL_MapSurfaceRGB(Surface, 0, 0, 0));
            } else {
                Render = SDL_CreateRenderer(Window, NULL);
                TTF_Init();
            }
        }

        /**
         * @brief Fills the window with color
         * 
         * If parameters are not specified, the current color is used.
         * Or if a specific color needs to be kept, specify -1
         * 
         * @param r Red component (0-255)
         * @param g Green component (0-255)
         * @param b Blue component (0-255)
         */
        void fill(Uint8 r = -1, Uint8 g = -1, Uint8 b = -1) {
            r = r == -1 ? color.r : r;
            b = b == -1 ? color.b : b;
            g = g == -1 ? color.g : g;

            color.r = r;
            color.g = g;
            color.b = b;

            Render != nullptr 
                ? (SDL_SetRenderDrawColor(Render, r, g, b, 255), SDL_RenderClear(Render))
                : SDL_FillSurfaceRect(Surface, NULL, SDL_MapSurfaceRGB(Surface, r, g, b));
        }

        /**
         * @brief Updates the contents of the window
         * 
         * Depending on the rendering mode, either the Renderer or Surface will be updated
         */
        void update() {
            Render != nullptr ? SDL_RenderPresent(Render) :  SDL_UpdateWindowSurface(Window);
        }
        
        /**
         * @brief Sets the window icon in the console
         * 
         * @param icon Texture of the icon
         */
        void setIcon(texture icon) {
            SDL_Surface *iconSurface = icon.get().surface;
            SDL_SetWindowIcon(Window, iconSurface);
        }

        /**
         * @brief Sets the path to the font
         * 
         * @param fontpath Path to the font file
         */
        void setFont(string fontpath){
            font = fontpath;
        }

        /**
         * @brief Changes the size and title of the window
         * 
         * @param width New width of the window (optional, to skip — -1)
         * @param height New height of the window (optional, to skip — -1)
         * @param name New title of the window (optional)
         */
        void change(int width = -1, int height = -1, string name = "") {
            window.width = width == -1 ? window.width : width;
            window.height = height == -1 ? window.height : height;
            window.name = name.empty() ? window.name : name;

            SDL_SetWindowSize(Window, window.width, window.height);
            SDL_SetWindowTitle(Window, window.name.c_str());
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
        void bindKey(SDL_Scancode key, Func func, FuncUp funcup = [](){}) {
            keyBindings.push_back({ key, func });
            keyUpBindings.push_back({ key, funcup });
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
            SDL_DestroyRenderer(Render); 
            SDL_DestroyWindow(Window);
            TTF_Quit();
            SDL_Quit();
        }
};

#endif
