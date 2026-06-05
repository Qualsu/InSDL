#ifndef INSDL_TEXTURE
#define INSDL_TEXTURE

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>
#include <utility>

using namespace std;

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
            string path;
        };

        textureData data;
    public:
        /**
         * @brief Constructor, loads an image from a file and creates a texture
         * 
         * @param render SDL_Renderer for creating the texture
         * @param file Path to the image file
         */
        texture(SDL_Renderer *render, string file) {
            data.surface = IMG_Load(file.c_str());
            data.texture = SDL_CreateTextureFromSurface(render, data.surface);
            data.path = file;
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
        friend ostream& operator<<(ostream& os, const texture& t) {
            os << "Texture(path: " << t.data.path << ")";
            return os;
        }
};

#endif
