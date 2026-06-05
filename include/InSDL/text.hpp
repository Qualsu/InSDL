#ifndef INSDL_TEXT
#define INSDL_TEXT

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <iostream>
#include <string>
#include <utility>

namespace insdl {

/**
 * @brief Class for working with text using SDL_ttf
 *
 * Allows creating text objects with a specified font, color, and content,
 * as well as modifying the text and color
 */
class text {
    private:
        struct textData {
            SDL_Surface *surface = nullptr;
            SDL_Texture *texture = nullptr;
            TTF_Font* font = nullptr;
            SDL_Color color = {255, 255, 255, 255};
            std::string text;
            std::string path;
        };

        SDL_Renderer *renderer = nullptr;
        textData data;

        void destroyRendered() {
            if (data.surface) {
                SDL_DestroySurface(data.surface);
                data.surface = nullptr;
            }
            if (data.texture) {
                SDL_DestroyTexture(data.texture);
                data.texture = nullptr;
            }
        }
    public:
        /**
         * @brief Constructor that initializes text with a specified font, text, and color
         * 
         * Loads the font, creates a surface and texture for rendering the text
         * To specify the font from the app object, provide the fontPath argument as - yourApp.font
         * 
         * @param render SDL_Renderer for creating textures
         * @param text String of text to display
         * @param fontPath Path to the font file
         * @param r Red component of color (0-255)
         * @param g Green component of color (0-255)
         * @param b Blue component of color (0-255)
         */
        text(SDL_Renderer *renderer, const std::string& textContent, std::string fontPath, Uint8 r = 255, Uint8 g = 255, Uint8 b = 255) {
            data.color = {r, g, b, 255};
            data.font = TTF_OpenFont(fontPath.c_str(), 128);
            data.surface = TTF_RenderText_Solid(data.font, textContent.c_str(), textContent.length(), data.color);
            data.texture = SDL_CreateTextureFromSurface(renderer, data.surface);
            this->renderer = renderer;
            data.text = textContent;
            data.path = fontPath;
        }

        text(const text&) = delete;
        text& operator=(const text&) = delete;

        text(text&& other) noexcept
            : renderer(std::exchange(other.renderer, nullptr)),
              data(std::exchange(other.data, textData{})) {}

        text& operator=(text&& other) noexcept {
            if (this != &other) {
                destroy();
                renderer = std::exchange(other.renderer, nullptr);
                data = std::exchange(other.data, textData{});
            }
            return *this;
        }

        ~text() {
            destroy();
        }

        /**
         * @brief Frees the resources of the surface, texture and font
         */
        void destroy() {
            destroyRendered();
            if (data.font) {
                TTF_CloseFont(data.font);
                data.font = nullptr;
            }
        }

        /**
         * @brief Updates the text by recreating the surface and texture with new content
         * 
         * @param newText New string of text
         */
        void setText(const std::string& newText) {
            destroyRendered();

            data.surface = TTF_RenderText_Solid(data.font, newText.c_str(), newText.length(), data.color);
            data.texture = SDL_CreateTextureFromSurface(renderer, data.surface);
            data.text = newText;
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
            destroyRendered();

            r = r == -1 ? data.color.r : r;
            b = b == -1 ? data.color.b : b;
            g = g == -1 ? data.color.g : g;

            data.color.r = r;
            data.color.g = g;
            data.color.b = b;

            data.color = {r, g, b, 255};
            data.surface = TTF_RenderText_Solid(data.font, data.text.c_str(), data.text.length(), data.color);
            data.texture = SDL_CreateTextureFromSurface(renderer, data.surface);
        }

        /**
         * @brief Returns the structure with the current text data
         * 
         * @return textData Structure with text parameters
         */
        textData get() const {
            return data;
        }

        /**
         * @brief Overload of the output operator to display information about the text
         * 
         * Outputs the text string, font path, and color in RGB format
         */
        friend std::ostream& operator<<(std::ostream& os, const text& t) {
            os << "Text(text: \"" << t.data.text << "\", path: " << t.data.path 
               << ", r: " << static_cast<int>(t.data.color.r) 
               << ", g: " << static_cast<int>(t.data.color.g) 
               << ", b: " << static_cast<int>(t.data.color.b) << ")";
            return os;
        }
};

using Text = text;

} // namespace insdl

#endif
