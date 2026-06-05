#ifndef INSDL_TEXTURE
#define INSDL_TEXTURE

#include <SDL3/SDL.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

using namespace std;

/**
 * @brief Class for working with textures
 * 
 * Allows loading an image from a file and creating a texture for rendering
*/
class texture {
    private:
        struct textureData {
            SDL_Surface *surface;
            SDL_Texture *texture;
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

        /**
         * @brief Frees the resources of the surface and texture
         */
        void destroy() {
            SDL_DestroySurface(data.surface);
            SDL_DestroyTexture(data.texture);
        }

        /**
         * @brief Returns the structure with the current texture data
         * 
         * @return textureData Structure with texture parameters
         */
        textureData get() {
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