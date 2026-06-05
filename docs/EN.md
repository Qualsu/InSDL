# InSDL Documentation

InSDL is a small C++ framework built on top of SDL3, SDL3_ttf, and SDL3_image. It wraps the most common startup, input, drawing, text, texture, and audio tasks into a compact API.

## Contents

- [Overview](#overview)
- [Installation](#installation)
- [Quick Start](#quick-start)
- [Rendering Modes](#rendering-modes)
- [Core Loop](#core-loop)
- [API Reference](#api-reference)
- [Examples](#examples)
- [Notes and Limitations](#notes-and-limitations)

## Overview

The framework lives inside the `insdl` namespace and exposes convenient aliases:

- `insdl::App`
- `insdl::Rect`
- `insdl::Texture`
- `insdl::Text`
- `insdl::Audio`
- `insdl::InputState`

The main header is:

```cpp
#include <InSDL/InSDL.hpp>
```

That header pulls in all of the public framework headers and SDL scancode definitions.

## Installation

The repository already contains the required SDL libraries and headers in `include/` and `lib/`.

### Build with make

The provided `makefile` builds the sample application from `src/main.cpp`:

```bash
make
```

Useful targets:

- `make compile` builds the executable
- `make run` launches the executable
- `make clean` removes the build artifact

If you want to build another file, set `SRC`:

```bash
make SRC=examples/my_game.cpp
```

### Include path

When using InSDL in your own project, add the `include/` directory to your compiler's include paths and link against:

- `SDL3`
- `SDL3_ttf`
- `SDL3_image`

## Quick Start

Minimal application:

```cpp
#include <InSDL/InSDL.hpp>

int main() {
    insdl::App app;
    app.init(800, 600, "My InSDL App");

    while (!app.quit) {
        insdl::handleEvent(app);

        app.clear();
        app.update();
    }

    return 0;
}
```

What this does:

- creates a window
- sets up SDL rendering
- processes events every frame
- clears the screen
- presents the frame

## Rendering Modes

`app.init()` supports two rendering modes:

```cpp
app.init(width, height, title, surface = false, resizeable = false, fontPath = "");
```

- `surface = false` uses `SDL_Renderer`
- `surface = true` uses `SDL_Surface`

### Renderer mode

Recommended for most use cases.

Advantages:

- texture rendering
- text rendering
- rectangle rendering with floating-point geometry
- rotation and flipping support for textures

### Surface mode

Useful for simpler software-style drawing.

Advantages:

- direct surface filling
- no renderer required for basic rectangle fills

Limitations:

- texture and text rendering helpers are renderer-based and should be used in renderer mode

## Core Loop

The framework expects the following order inside your game loop:

```cpp
while (!app.quit) {
    insdl::handleEvent(app);

    // game logic

    app.update();
}
```

Important details:

- `handleEvent(app)` updates input state and triggers bindings
- `app.update()` presents the frame and updates delta time
- call `handleEvent(app)` once per frame before reading input state

## API Reference

### `insdl::App`

Main application object that owns the window, renderer or surface, input state, and binding tables.

#### Construction and lifetime

```cpp
insdl::App app;
```

The class is non-copyable and non-movable. It automatically calls `exit()` in its destructor.

#### `void init(int width, int height, std::string name, bool surface = false, bool resizeable = false, std::string fontPath = "")`

Initializes SDL, creates the window, and sets up either a renderer or a window surface.

- `width`, `height`: window size
- `name`: window title
- `surface`: `true` for surface mode, `false` for renderer mode
- `resizeable`: enables SDL window resizing
- `fontPath`: optional font path for text rendering

If `fontPath` is empty, InSDL uses its bundled default font.

#### `bool ok() const`

Returns `true` when the application initialized successfully.

#### `std::string error() const`

Returns the last framework-level error message.

#### `SDL_Window* window() const`

Returns the underlying SDL window pointer.

#### `SDL_Renderer* renderer() const`

Returns the renderer pointer in renderer mode, or `nullptr` in surface mode.

#### `SDL_Surface* surface() const`

Returns the window surface pointer in surface mode, or `nullptr` in renderer mode.

#### `void clear()`

Clears the frame using the currently stored color.

#### `void fill(Uint8 r, Uint8 g, Uint8 b)`

Sets the current fill color and clears/fills the frame background.

#### `void fill()`

Alias for `clear()`.

#### `void update()`

Presents the current frame.

- calls `SDL_RenderPresent()` in renderer mode
- calls `SDL_UpdateWindowSurface()` in surface mode
- updates the frame delta time

#### `float deltaTime() const`

Returns the time in seconds between the previous two `update()` calls.

#### `void setIcon(const texture& icon)`

Sets the window icon from a `texture` object.

#### `void setFont(std::string fontPath)`

Overrides the current font path used by future text objects.

#### `void change(int width = -1, int height = -1, std::string name = "")`

Changes the stored window size and/or title.

- pass `-1` to keep a dimension unchanged
- pass an empty title to keep the current title

#### `void bindKey(SDL_Scancode key, Func func, FuncUp funcUp = [](){})`

Binds callbacks to key press and key release.

- `func` runs on key down
- `funcUp` runs on key up

#### `void bindMouseButton(Uint8 button, Func func)`

Binds a callback to a mouse button press.

#### `void bindMouseMotion(Func func)`

Binds a callback to mouse movement.

The callback receives `int x, int y`.

#### `void exit()`

Releases SDL resources and closes the application.

#### Public fields

- `bool quit`
- `std::vector<...> keyBindings`
- `std::vector<...> keyUpBindings`
- `std::vector<...> mouseBindings`
- `std::vector<...> mouseMotionBindings`
- `insdl::inputState input`
- `colorStruct color`
- `windowStruct windowData`
- `std::string font`

You usually only need to access `quit`, `input`, and sometimes `font`.

### `insdl::handleEvent(app&)`

Processes SDL events for:

- quit events
- key down
- key up
- mouse button down
- mouse motion

It also updates `app.input`.

### `insdl::exitEvent(app&)`

Legacy helper that only checks for `SDL_EVENT_QUIT`.

It is not needed in normal usage. Prefer `handleEvent(app)`.

### `insdl::InputState`

Frame-based input state tracker.

#### `void beginFrame()`

Resets pressed/released flags for the new frame and refreshes the mouse position.

#### `void setKeyDown(SDL_Scancode key)`

Marks a key as held down and sets the pressed flag on the first frame of the press.

#### `void setKeyUp(SDL_Scancode key)`

Marks a key as released and sets the released flag for the current frame.

#### `void setMousePosition(float x, float y)`

Stores the latest mouse coordinates.

#### `bool isKeyDown(SDL_Scancode key) const`

Returns `true` while a key is held.

#### `bool isKeyPressed(SDL_Scancode key) const`

Returns `true` only on the frame when the key transitions from up to down.

#### `bool isKeyReleased(SDL_Scancode key) const`

Returns `true` only on the frame when the key transitions from down to up.

#### `int mouseX() const`, `int mouseY() const`

Return mouse coordinates as integers.

#### `float mouseXf() const`, `float mouseYf() const`

Return mouse coordinates as floats.

### `insdl::Rect`

Rectangle helper that adapts to renderer mode or surface mode automatically.

#### Constructor

```cpp
insdl::Rect rect(app, x, y, w, h);
```

Creates a rectangle bound to the given application.

#### Geometry methods

- `setRect(int x, int y, int w, int h)`
- `setPosition(int x, int y)`
- `setSize(int w, int h)`
- `setX(int x)`
- `setY(int y)`
- `setWidth(int w)`
- `setHeight(int h)`
- `addX(int x)`
- `addY(int y)`
- `addWidth(int w)`
- `addHeight(int h)`
- `subX(int x)`
- `subY(int y)`
- `subWidth(int w)`
- `subHeight(int h)`

#### Query methods

- `int getX()`
- `int getY()`
- `int getWidth()`
- `int getHeight()`
- `rectData getData()`

`getData()` returns a copy of the internal data structure.

#### Drawing methods

##### `void fill(int r = -1, int g = -1, int b = -1)`

Fills the rectangle with a color.

- if a color component is `-1`, the previously stored value is reused

##### `void update()`

In surface mode, redraws the rectangle using its stored color.

##### `void fillTexture(texture* textureObject, double deg = 0, SDL_FlipMode mode = SDL_FLIP_NONE, SDL_FPoint point = {0, 0})`

Draws a texture inside the rectangle in renderer mode.

##### `void fillText(text* textObject)`

Draws text inside the rectangle in renderer mode.

#### Interaction helpers

##### `bool onTouch(rect& otherRect)`

Returns `true` when two rectangles intersect.

##### `bool onHover()`

Returns `true` when the mouse cursor is inside the rectangle.

#### Stream output

`operator<<` prints rectangle geometry and color.

### `insdl::Texture`

RAII wrapper around an SDL surface and texture loaded from an image file.

#### Constructor

```cpp
insdl::Texture texture(app.renderer(), "assets/image.png");
```

The constructor requires a valid renderer.

#### `bool ok() const`

Returns `true` when the image and texture loaded successfully.

#### `std::string error() const`

Returns the last texture error.

#### `void destroy()`

Releases the surface and texture resources.

#### `textureData get() const`

Returns a copy of the internal texture data.

#### Stream output

`operator<<` prints the file path.

### `insdl::Text`

RAII wrapper for text rendered with SDL_ttf.

#### Constructor

```cpp
insdl::Text title(app.renderer(), "Hello", app.font, 255, 255, 255);
```

Parameters:

- `renderer`: valid SDL renderer
- `textContent`: string to render
- `fontPath`: path to a `.ttf` font
- `r`, `g`, `b`: text color

#### `bool ok() const`

Returns `true` when the font, surface, and texture are ready.

#### `std::string error() const`

Returns the last text error.

#### `void destroy()`

Releases the text surface, texture, and font.

#### `void setText(const std::string& newText)`

Re-renders the text with new content.

#### `void setColor(Uint8 r = -1, Uint8 g = -1, Uint8 b = -1)`

Changes the text color and re-renders it.

- `-1` keeps the current channel value

#### `textData get() const`

Returns a copy of the internal text data.

#### Stream output

`operator<<` prints the string, font path, and RGB color.

### `insdl::Audio`

RAII wrapper for WAV playback.

#### Constructor

```cpp
insdl::Audio sound("assets/click.wav");
```

The loader expects a WAV file.

#### `bool ok() const`

Returns `true` when the WAV data, audio stream, and device are ready.

#### `std::string error() const`

Returns the last audio error.

#### `void play()`

Queues the loaded WAV data for playback.

#### `void resume()`

Resumes the audio device.

#### `void pause()`

Pauses the audio device.

#### `void stop()`

Stops playback and frees resources.

#### `audioData get() const`

Returns a copy of the internal audio data.

#### Stream output

`operator<<` prints the WAV file path.

### `insdl::delayMs`, `delaySec`, `delayMin`, `delayHour`

Sleep helpers based on `std::this_thread::sleep_for`.

Examples:

```cpp
insdl::delayMs(16);
insdl::delaySec(1);
```

## Examples

### Keyboard input

```cpp
#include <InSDL/InSDL.hpp>

int main() {
    insdl::App app;
    app.init(800, 600, "Input Demo");

    while (!app.quit) {
        insdl::handleEvent(app);

        if (app.input.isKeyPressed(SDL_SCANCODE_SPACE)) {
            app.fill(30, 30, 30);
        }

        if (app.input.isKeyDown(SDL_SCANCODE_W)) {
            // move up
        }

        app.update();
    }
}
```

### Rectangle drawing

```cpp
insdl::Rect box(app, 50, 50, 200, 120);
box.fill(255, 80, 80);
```

### Text drawing

```cpp
insdl::Text label(app.renderer(), "Hello SDL", app.font, 255, 255, 255);
insdl::Rect area(app, 20, 20, 300, 80);
area.fillText(&label);
```

### Texture drawing

```cpp
insdl::Texture image(app.renderer(), "assets/logo.png");
insdl::Rect slot(app, 100, 100, 256, 256);
slot.fillTexture(&image);
```

### Audio playback

```cpp
insdl::Audio click("assets/click.wav");
if (click.ok()) {
    click.play();
}
```

## Notes and Limitations

- `Texture`, `Text`, and `Audio` are move-only objects.
- `Text` uses `TTF_RenderText_Solid`, so it is optimized for simple solid-color text.
- `Audio` currently targets WAV files.
- `handleEvent(app)` should be called once per frame before reading input state.
- `exitEvent(app)` exists for compatibility but is not needed in normal usage.