#ifndef INSDL_TEXTURE
#define INSDL_TEXTURE

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <string>
#include <utility>

namespace insdl {

/**
 * @brief Class for working with textures
 *
 * Allows loading an image from a file and creating a texture for rendering
*/
class texture {
    private:
        struct textureData {
            SDL_Surface *surface = nullptr;
            SDL_Texture *texture = nullptr;
            std::string path;
        };

        textureData data;
        std::string errorMessage;

        void setError(const std::string& context) {
            const char* sdlError = SDL_GetError();
            errorMessage = context + " failed";
            if (sdlError && sdlError[0] != '\0') {
                errorMessage += ": ";
                errorMessage += sdlError;
            }
        }
    public:
        /**
         * @brief Constructor, loads an image from a file and creates a texture
         * 
         * @param render SDL_Renderer for creating the texture
         * @param file Path to the image file
         */
        texture(SDL_Renderer *renderer, std::string filePath) {
            data.path = filePath;

            if (!renderer) {
                errorMessage = "Cannot load texture \"" + filePath + "\": renderer is null";
                return;
            }

            data.surface = IMG_Load(filePath.c_str());
            if (!data.surface) {
                setError("IMG_Load(\"" + filePath + "\")");
                return;
            }

            data.texture = SDL_CreateTextureFromSurface(renderer, data.surface);
            if (!data.texture) {
                setError("SDL_CreateTextureFromSurface(\"" + filePath + "\")");
            }
        }

        texture(const texture&) = delete;
        texture& operator=(const texture&) = delete;

        texture(texture&& other) noexcept
            : data(std::exchange(other.data, textureData{})) {}

        texture& operator=(texture&& other) noexcept {
            if (this != &other) {
                destroy();
                data = std::exchange(other.data, textureData{});
            }
            return *this;
        }

        ~texture() {
            destroy();
        }

        /**
         * @brief Returns true if the image and SDL texture were loaded successfully.
         */
        bool ok() const {
            return errorMessage.empty() && data.surface && data.texture;
        }

        /**
         * @brief Returns the last SDL-related error message.
         */
        std::string error() const {
            return errorMessage;
        }

        /**
         * @brief Frees the resources of the surface and texture
         */
        void destroy() {
            if (data.surface) {
                SDL_DestroySurface(data.surface);
                data.surface = nullptr;
            }
            if (data.texture) {
                SDL_DestroyTexture(data.texture);
                data.texture = nullptr;
            }
        }

        /**
         * @brief Returns the structure with the current texture data
         * 
         * @return textureData Structure with texture parameters
         */
        textureData get() const {
            return data;
        }

        /**
         * @brief Overload of the output operator to display information about the texture
         * 
         * Outputs the path to the image file
         */
        friend std::ostream& operator<<(std::ostream& os, const texture& t) {
            os << "Texture(path: " << t.data.path << ")";
            return os;
        }
};

using Texture = texture;

} // namespace insdl

#endif
