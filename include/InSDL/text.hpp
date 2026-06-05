#ifndef INSDL_TEXT
#define INSDL_TEXT

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>

using namespace std;

/**
 * @brief Class for working with text using SDL_ttf
 * 
 * Allows creating text objects with a specified font, color, and content,
 * as well as modifying the text and color
 */
class text {
    private:
        struct textData {
            SDL_Surface *surface;
            SDL_Texture *texture;
            TTF_Font* font;
            SDL_Color color = {255, 255, 255, 255};
            string text;
            string path;
        };

        SDL_Renderer *Render;
        textData data;
    public:
        /**
         * @brief Constructor that initializes text with a specified font, text, and color
         * 
         * Loads the font, creates a surface and texture for rendering the text
         * To specify the font from the app object, provide the fontpath argument as - your_app_obj.font
         * 
         * @param render SDL_Renderer for creating textures
         * @param text String of text to display
         * @param fontpath Path to the font file
         * @param r Red component of color (0-255)
         * @param g Green component of color (0-255)
         * @param b Blue component of color (0-255)
         */
        text(SDL_Renderer *render, const string& text, string fontpath, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255) {
            data.color = {r, g, b, 255};
            data.font = TTF_OpenFont(fontpath.c_str(), 128);
            data.surface = TTF_RenderText_Solid(data.font, text.c_str(), text.length(), data.color);
            data.texture = SDL_CreateTextureFromSurface(render, data.surface);
            Render = render;
            data.text = text;
            data.path = fontpath;
        }

        /**
         * @brief Frees the resources of the surface and texture
         */
        void destroy() {
            SDL_DestroySurface(data.surface);
            SDL_DestroyTexture(data.texture);
        }

        /**
         * @brief Updates the text by recreating the surface and texture with new content
         * 
         * @param newText New string of text
         */
        void setText(const string& newText) {
            destroy();

            data.surface = TTF_RenderText_Solid(data.font, newText.c_str(), newText.length(), data.color);
            data.texture = SDL_CreateTextureFromSurface(Render, data.surface);
        }
        
        /**
         * @brief Changes the text color and updates the surface and texture
         * 
         * If color components are not provided, the current values are used.
         * Or if a specific color needs to be kept, specify -1
         * 
         * @param r Red component of color (0-255)
         * @param g Green component of color (0-255)
         * @param b Blue component of color (0-255)
         */
        void setColor(Uint8 r = -1, Uint8 g = -1, Uint8 b = -1) {
            destroy();

            r = r == -1 ? data.color.r : r;
            b = b == -1 ? data.color.b : b;
            g = g == -1 ? data.color.g : g;

            data.color.r = r;
            data.color.g = g;
            data.color.b = b;

            data.color = {r, g, b, 255};
            data.surface = TTF_RenderText_Solid(data.font, data.text.c_str(), data.text.length(), data.color);
            data.texture = SDL_CreateTextureFromSurface(Render, data.surface);
        }

        /**
         * @brief Returns the structure with the current text data
         * 
         * @return textData Structure with text parameters
         */
        textData get() {
            return data;
        }

        /**
         * @brief Overload of the output operator to display information about the text
         * 
         * Outputs the text string, font path, and color in RGB format
         */
        friend ostream& operator<<(ostream& os, const text& t) {
            os << "Text(text: \"" << t.data.text << "\", path: " << t.data.path 
               << ", r: " << static_cast<int>(t.data.color.r) 
               << ", g: " << static_cast<int>(t.data.color.g) 
               << ", b: " << static_cast<int>(t.data.color.b) << ")";
            return os;
        }
};

#endif