#ifndef RECT
#define RECT
#include <SDL3/SDL.h>
#include <iostream>
#include <variant>
#include <app.hpp>
#include <texture.hpp>
#include <text.hpp>
using namespace std;

/**
* @brief Class for representing and managing a rectangle with color
*
* Allows setting position, size, color, checking intersections, and interactions with the mouse
* Automatically adapts between SDL_Rect (for Surface rendering) and SDL_FRect (for Renderer rendering)
*/
class rect {
    private:
        struct colorStruct {
            Uint8 r = 0;
            Uint8 g = 0;
            Uint8 b = 0;
        };

        struct rectData {
            variant<SDL_Rect, SDL_FRect> rect;
            colorStruct color;
        };

        bool surface = false;
        rectData data;
        app application;

    public:
        /**
        * @brief Constructor with position and size parameters
        *
        * Automatically selects SDL_Rect or SDL_FRect based on the application's rendering mode
        *
        * @param app Reference to the parent application object
        * @param x X coordinate of the top-left corner (default: 0)
        * @param y Y coordinate of the top-left corner (default: 0)
        * @param w Width of the rectangle (default: 0)
        * @param h Height of the rectangle (default: 0)
        */
        rect(app app, int x = 0, int y = 0, int w = 0, int h = 0) {
            application = app;
            surface = (app.Surface != nullptr);
            if (surface) {
                data.rect = SDL_Rect{ x, y, w, h };
            } else {
                data.rect = SDL_FRect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h) };
            }
        }

        /**
        * @brief Sets the position and size of the rectangle
        *
        * @param x X coordinate of the top-left corner
        * @param y Y coordinate of the top-left corner
        * @param w Width of the rectangle
        * @param h Height of the rectangle
        */
        void setRect(int x, int y, int w, int h) {
            if (surface) {
                data.rect = SDL_Rect{ x, y, w, h };
            } else {
                data.rect = SDL_FRect{ static_cast<float>(x), static_cast<float>(y), static_cast<float>(w), static_cast<float>(h) };
            }
        }

        /**
        * @brief Sets the position of the rectangle
        *
        * @param x X coordinate of the top-left corner
        * @param y Y coordinate of the top-left corner
        */
        void setPosition(int x, int y) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.x = x; r.y = y;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.x = static_cast<float>(x); r.y = static_cast<float>(y);
                data.rect = r;
            }
        }

        /**
        * @brief Sets the size of the rectangle
        *
        * @param w Width of the rectangle
        * @param h Height of the rectangle
        */
        void setSize(int w, int h) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.w = w; r.h = h;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.w = static_cast<float>(w); r.h = static_cast<float>(h);
                data.rect = r;
            }
        }

        /** 
        * @brief Sets the X coordinate of the rectangle
        *
        * @param x X coordinate of the top-left corner
        */
        void setX(int x) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.x = x;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.x = static_cast<float>(x);
                data.rect = r;
            }
        }

        /** 
        * @brief Sets the Y coordinate of the rectangle
        *
        * @param y Y coordinate of the top-left corner
        */
        void setY(int y) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.y = y;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.y = static_cast<float>(y);
                data.rect = r;
            }
        }

        /** 
        * @brief Sets the width of the rectangle
        *
        * @param w Width of the rectangle
        */
        void setWidth(int w) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.w = w;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.w = static_cast<float>(w);
                data.rect = r;
            }
        }

        /** 
        * @brief Sets the height of the rectangle
        *
        * @param h Height of the rectangle
        */
        void setHeight(int h) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.h = h;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.h = static_cast<float>(h);
                data.rect = r;
            }
        }

        /** 
        * @brief Increases the X coordinate by the specified value
        *
        * @param x Value to add to the current X coordinate
        */
        void addX(int x) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.x += x;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.x += static_cast<float>(x);
                data.rect = r;
            }
        }

        /** 
        * @brief Increases the Y coordinate by the specified value
        *
        * @param y Value to add to the current Y coordinate
        */
        void addY(int y) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.y += y;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.y += static_cast<float>(y);
                data.rect = r;
            }
        }

        /** 
        * @brief Increases the width by the specified value
        *
        * @param w Value to add to the current width (can be negative)
        */
        void addWidth(int w) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.w += w;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.w += static_cast<float>(w);
                data.rect = r;
            }
        }

        /** 
        * @brief Increases the height by the specified value
        *
        * @param h Value to add to the current height (can be negative)
        */
        void addHeight(int h) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.h += h;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.h += static_cast<float>(h);
                data.rect = r;
            }
        }

        /** 
        * @brief Decreases the X coordinate by the specified value
        *
        * @param x Value to subtract from the current X coordinate
        */
        void subX(int x) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.x -= x;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.x -= static_cast<float>(x);
                data.rect = r;
            }
        }

        /** 
        * @brief Decreases the Y coordinate by the specified value
        *
        * @param y Value to subtract from the current Y coordinate
        */
        void subY(int y) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.y -= y;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.y -= static_cast<float>(y);
                data.rect = r;
            }
        }

        /** 
        * @brief Decreases the width by the specified value
        *
        * @param w Value to subtract from the current width (can be negative)
        */
        void subWidth(int w) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.w -= w;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.w -= static_cast<float>(w);
                data.rect = r;
            }
        }

        /** 
        * @brief Decreases the height by the specified value
        * 
        * @param h Value to subtract from the current height (can be negative)
        */
        void subHeight(int h) {
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                r.h -= h;
                data.rect = r;
            } else {
                auto r = get<SDL_FRect>(data.rect);
                r.h -= static_cast<float>(h);
                data.rect = r;
            }
        }

        /** 
        * @brief Returns the X coordinate of the top-left corner
        */
        int getX() {
            return surface
                ? get<SDL_Rect>(data.rect).x
                : static_cast<int>(get<SDL_FRect>(data.rect).x);
        }

        /** 
        * @brief Returns the Y coordinate of the top-left corner
        */
        int getY() {
            return surface
                ? get<SDL_Rect>(data.rect).y
                : static_cast<int>(get<SDL_FRect>(data.rect).y);
        }

        /** 
        * @brief Returns the width of the rectangle
        */
        int getWidth() {
            return surface
                ? get<SDL_Rect>(data.rect).w
                : static_cast<int>(get<SDL_FRect>(data.rect).w);
        }

        /** 
        * @brief Returns the height of the rectangle
        */
        int getHeight() {
            return surface
                ? get<SDL_Rect>(data.rect).h
                : static_cast<int>(get<SDL_FRect>(data.rect).h);
        }

        /**
        * @brief Returns the rectData structure with the rectangle's parameters
        *
        * @note Returns a copy, not a reference to internal data.
        */
        rectData _get() {
            return data;
        }

        /**
        * @brief Fills the rectangle area with the specified color
        *
        * If color parameters are -1, the current stored color is used.
        *
        * @param r Red color component (0-255), -1 to keep current value (default: -1)
        * @param g Green color component (0-255), -1 to keep current value (default: -1)
        * @param b Blue color component (0-255), -1 to keep current value (default: -1)
        */
        void fill(int r = -1, int g = -1, int b = -1) {
            r = (r == -1) ? data.color.r : static_cast<Uint8>(r);
            g = (g == -1) ? data.color.g : static_cast<Uint8>(g);
            b = (b == -1) ? data.color.b : static_cast<Uint8>(b);
            data.color.r = r;
            data.color.g = g;
            data.color.b = b;
            if (surface) {
                SDL_FillSurfaceRect(application.Surface, &get<SDL_Rect>(data.rect), SDL_MapSurfaceRGB(application.Surface, r, g, b));
            } else {
                SDL_SetRenderDrawColor(application.Render, r, g, b, 255);
                SDL_RenderFillRect(application.Render, &get<SDL_FRect>(data.rect));
            }
        }

        /**
        * @brief Renders a texture inside the rectangle area with rotation and flipping options
        *
        * Only available in Renderer mode (not Surface mode)
        *
        * @param texture Pointer to the texture object to render
        * @param deg Rotation angle in degrees (clockwise, default: 0.0)
        * @param mode Flip mode: SDL_FLIP_NONE, SDL_FLIP_HORIZONTAL, or SDL_FLIP_VERTICAL (default: SDL_FLIP_NONE)
        * @param point Rotation center point relative to the rectangle (default: {0, 0} = top-left corner)
        */
        void fillTexture(texture *texture, double deg = 0, SDL_FlipMode mode = SDL_FLIP_NONE, SDL_FPoint point = {0, 0}) {
            SDL_RenderTextureRotated(application.Render, texture->get().texture, nullptr, &get<SDL_FRect>(data.rect), deg, &point, mode);
        }

        /**
        * @brief Renders text inside the rectangle area
        *
        * @param text Pointer to the text object to render
        */
        void fillText(text *text) {
            SDL_RenderTexture(application.Render, text->get().texture, nullptr, &get<SDL_FRect>(data.rect));
        }

        /**
        * @brief Updates the rectangle area on the surface with the object's current color
        *
        * Only applicable in Surface rendering mode
        */
        void update() {
            SDL_FillSurfaceRect(application.Surface, &get<SDL_Rect>(data.rect), SDL_MapSurfaceRGB(application.Surface, data.color.r, data.color.g, data.color.b));
        }

        /**
        * @brief Checks if the current rectangle intersects with another rectangle
        *
        * @param otrect Reference to another rect object for intersection check
        */
        bool onTouch(rect& otrect) {
            if (surface) {
                auto a = get<SDL_Rect>(data.rect);
                auto b = get<SDL_Rect>(otrect.data.rect);
                return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
            } else {
                auto a = get<SDL_FRect>(data.rect);
                auto b = get<SDL_FRect>(otrect.data.rect);
                return (a.x < b.x + b.w && a.x + a.w > b.x && a.y < b.y + b.h && a.y + a.h > b.y);
            }
        }

        /**
        * @brief Checks if the mouse cursor is inside the rectangle
        */
        bool onHover() {
            float mouseX, mouseY;
            SDL_GetMouseState(&mouseX, &mouseY);
            if (surface) {
                auto r = get<SDL_Rect>(data.rect);
                return (mouseX >= r.x && mouseX <= r.x + r.w && mouseY >= r.y && mouseY <= r.y + r.h);
            } else {
                auto r = get<SDL_FRect>(data.rect);
                return (mouseX >= r.x && mouseX <= r.x + r.w && mouseY >= r.y && mouseY <= r.y + r.h);
            }
        }

        /**
        * @brief Overloads the output operator for convenient display of rectangle parameters
        *
        * Outputs rectangle geometry (position and size) and RGB color values to the output stream
        */
        friend ostream& operator<<(ostream& os, rect& r) {
            if (r.surface) {
                auto d = get<SDL_Rect>(r.data.rect);
                os << "Rect(x: " << d.x << ", y: " << d.y
                << ", w: " << d.w << ", h: " << d.h;
            } else {
                auto d = get<SDL_FRect>(r.data.rect);
                os << "Rect(x: " << d.x << ", y: " << d.y
                << ", w: " << d.w << ", h: " << d.h;
            }
            os << ", r: " << static_cast<int>(r.data.color.r)
            << ", g: " << static_cast<int>(r.data.color.g)
            << ", b: " << static_cast<int>(r.data.color.b)
            << ")";
            return os;
        }
};
#endif