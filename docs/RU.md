# Документация InSDL

InSDL - это небольшой C++ фреймворк поверх SDL3, SDL3_ttf и SDL3_image. Он упрощает запуск приложения, ввод, отрисовку, работу с текстом, текстурами и WAV-аудио.

## Содержание

- [Обзор](#обзор)
- [Установка](#установка)
- [Быстрый старт](#быстрый-старт)
- [Режимы рендеринга](#режимы-рендеринга)
- [Основной цикл](#основной-цикл)
- [API](#api)
- [Примеры](#примеры)
- [Замечания и ограничения](#замечания-и-ограничения)

## Обзор

Фреймворк находится в пространстве имен `insdl` и предоставляет удобные псевдонимы:

- `insdl::App`
- `insdl::Rect`
- `insdl::Texture`
- `insdl::Text`
- `insdl::Audio`
- `insdl::InputState`

Главный заголовок:

```cpp
#include <InSDL/InSDL.hpp>
```

Он подключает все публичные заголовки фреймворка и определения SDL scancode.

## Установка

В репозитории уже есть нужные заголовки и библиотеки SDL в `include/` и `lib/`.

### Сборка через make

Файл `makefile` собирает пример из `src/main.cpp`:

```bash
make
```

Полезные цели:

- `make compile` собирает исполняемый файл
- `make run` запускает его
- `make clean` удаляет артефакт сборки

Если нужно собрать другой файл, укажите `SRC`:

```bash
make SRC=examples/my_game.cpp
```

### Пути подключения

Если вы используете InSDL в своем проекте, добавьте `include/` в пути компилятора и линкуйте:

- `SDL3`
- `SDL3_ttf`
- `SDL3_image`

## Быстрый старт

Минимальное приложение:

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

Что здесь происходит:

- создается окно
- настраивается SDL-рендеринг
- каждый кадр обрабатываются события
- очищается экран
- показывается кадр

## Режимы рендеринга

`app.init()` поддерживает два режима:

```cpp
app.init(width, height, title, surface = false, resizeable = false, fontPath = "");
```

- `surface = false` использует `SDL_Renderer`
- `surface = true` использует `SDL_Surface`

### Режим Renderer

Рекомендуется для большинства задач.

Плюсы:

- работа с текстурами
- отрисовка текста
- прямоугольники с вещественными координатами
- поворот и отражение текстур

### Режим Surface

Подходит для более простого «software-style» рисования.

Плюсы:

- прямое заполнение поверхности
- для базовой заливки не нужен renderer

Ограничения:

- helpers для текста и текстур рассчитаны на renderer-режим

## Основной цикл

Фреймворк ожидает такой порядок внутри игрового цикла:

```cpp
while (!app.quit) {
    insdl::handleEvent(app);

    // логика игры

    app.update();
}
```

Важно:

- `handleEvent(app)` обновляет состояние ввода и вызывает привязанные обработчики
- `app.update()` показывает кадр и обновляет delta time
- вызывать `handleEvent(app)` нужно один раз за кадр перед чтением input

## API

### `insdl::App`

Главный объект приложения, который хранит окно, renderer или surface, состояние ввода и таблицы привязок.

#### Создание и жизненный цикл

```cpp
insdl::App app;
```

Класс нельзя копировать и перемещать. Деструктор автоматически вызывает `exit()`.

#### `void init(int width, int height, std::string name, bool surface = false, bool resizeable = false, std::string fontPath = "")`

Инициализирует SDL, создает окно и настраивает либо renderer, либо window surface.

- `width`, `height`: размер окна
- `name`: заголовок окна
- `surface`: `true` для surface-режима, `false` для renderer-режима
- `resizeable`: включает изменение размера окна
- `fontPath`: необязательный путь к шрифту для текста

Если `fontPath` пустой, InSDL использует встроенный шрифт по умолчанию.

#### `bool ok() const`

Возвращает `true`, если приложение успешно инициализировалось.

#### `std::string error() const`

Возвращает последнее сообщение об ошибке фреймворка.

#### `SDL_Window* window() const`

Возвращает указатель на SDL-окно.

#### `SDL_Renderer* renderer() const`

Возвращает renderer в renderer-режиме, либо `nullptr` в surface-режиме.

#### `SDL_Surface* surface() const`

Возвращает window surface в surface-режиме, либо `nullptr` в renderer-режиме.

#### `void clear()`

Очищает кадр текущим сохраненным цветом.

#### `void fill(Uint8 r, Uint8 g, Uint8 b)`

Устанавливает текущий цвет и заливает фон кадра.

#### `void fill()`

Псевдоним для `clear()`.

#### `void update()`

Показывает текущий кадр.

- в renderer-режиме вызывает `SDL_RenderPresent()`
- в surface-режиме вызывает `SDL_UpdateWindowSurface()`
- обновляет delta time кадра

#### `float deltaTime() const`

Возвращает время в секундах между двумя последними вызовами `update()`.

#### `void setIcon(const texture& icon)`

Устанавливает иконку окна из объекта `texture`.

#### `void setFont(std::string fontPath)`

Меняет путь к шрифту, который будет использоваться для новых объектов текста.

#### `void change(int width = -1, int height = -1, std::string name = "")`

Меняет сохраненные размер окна и/или заголовок.

- `-1` оставляет размер без изменений
- пустое имя сохраняет текущий заголовок

#### `void bindKey(SDL_Scancode key, Func func, FuncUp funcUp = [](){})`

Привязывает функции к нажатию и отпусканию клавиши.

- `func` вызывается при нажатии
- `funcUp` вызывается при отпускании

#### `void bindMouseButton(Uint8 button, Func func)`

Привязывает функцию к нажатию кнопки мыши.

#### `void bindMouseMotion(Func func)`

Привязывает функцию к движению мыши.

Колбэк получает `int x, int y`.

#### `void exit()`

Освобождает SDL-ресурсы и завершает приложение.

#### Публичные поля

- `bool quit`
- `std::vector<...> keyBindings`
- `std::vector<...> keyUpBindings`
- `std::vector<...> mouseBindings`
- `std::vector<...> mouseMotionBindings`
- `insdl::inputState input`
- `colorStruct color`
- `windowStruct windowData`
- `std::string font`

Обычно напрямую нужны только `quit`, `input` и иногда `font`.

### `insdl::handleEvent(app&)`

Обрабатывает SDL-события:

- выход из приложения
- нажатие клавиш
- отпускание клавиш
- нажатие кнопок мыши
- движение мыши

Также обновляет `app.input`.

### `insdl::exitEvent(app&)`

Устаревший helper, который проверяет только `SDL_EVENT_QUIT`.

В обычной работе не нужен. Используйте `handleEvent(app)`.

### `insdl::InputState`

Состояние ввода по кадрам.

#### `void beginFrame()`

Сбрасывает флаги pressed/released для нового кадра и обновляет позицию мыши.

#### `void setKeyDown(SDL_Scancode key)`

Помечает клавишу как удерживаемую и выставляет pressed в первый кадр нажатия.

#### `void setKeyUp(SDL_Scancode key)`

Помечает клавишу как отпущенную и выставляет released в текущем кадре.

#### `void setMousePosition(float x, float y)`

Сохраняет последние координаты мыши.

#### `bool isKeyDown(SDL_Scancode key) const`

Возвращает `true`, пока клавиша удерживается.

#### `bool isKeyPressed(SDL_Scancode key) const`

Возвращает `true` только в кадр, когда клавиша нажата.

#### `bool isKeyReleased(SDL_Scancode key) const`

Возвращает `true` только в кадр, когда клавиша отпущена.

#### `int mouseX() const`, `int mouseY() const`

Возвращают координаты мыши как целые числа.

#### `float mouseXf() const`, `float mouseYf() const`

Возвращают координаты мыши как `float`.

### `insdl::Rect`

Помощник для прямоугольников, который автоматически подстраивается под surface- или renderer-режим.

#### Конструктор

```cpp
insdl::Rect rect(app, x, y, w, h);
```

Создает прямоугольник, связанный с указанным приложением.

#### Методы геометрии

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

#### Методы чтения

- `int getX()`
- `int getY()`
- `int getWidth()`
- `int getHeight()`
- `rectData getData()`

`getData()` возвращает копию внутренней структуры.

#### Методы отрисовки

##### `void fill(int r = -1, int g = -1, int b = -1)`

Заполняет прямоугольник цветом.

- если компонент равен `-1`, используется сохраненное значение

##### `void update()`

В surface-режиме перерисовывает прямоугольник его сохраненным цветом.

##### `void fillTexture(texture* textureObject, double deg = 0, SDL_FlipMode mode = SDL_FLIP_NONE, SDL_FPoint point = {0, 0})`

Рисует текстуру внутри прямоугольника в renderer-режиме.

##### `void fillText(text* textObject)`

Рисует текст внутри прямоугольника в renderer-режиме.

#### Помощники взаимодействия

##### `bool onTouch(rect& otherRect)`

Возвращает `true`, если два прямоугольника пересекаются.

##### `bool onHover()`

Возвращает `true`, если курсор мыши находится внутри прямоугольника.

#### Вывод в поток

`operator<<` печатает геометрию и цвет.

### `insdl::Texture`

RAII-обертка над SDL surface и texture, загруженными из файла изображения.

#### Конструктор

```cpp
insdl::Texture texture(app.renderer(), "assets/image.png");
```

Конструктор требует валидный renderer.

#### `bool ok() const`

Возвращает `true`, если изображение и texture успешно загружены.

#### `std::string error() const`

Возвращает последнюю ошибку текстуры.

#### `void destroy()`

Освобождает surface и texture.

#### `textureData get() const`

Возвращает копию внутренней структуры.

#### Вывод в поток

`operator<<` печатает путь к файлу.

### `insdl::Text`

RAII-обертка для текста, отрисованного через SDL_ttf.

#### Конструктор

```cpp
insdl::Text title(app.renderer(), "Hello", app.font, 255, 255, 255);
```

Параметры:

- `renderer`: валидный SDL renderer
- `textContent`: строка текста
- `fontPath`: путь к `.ttf` шрифту
- `r`, `g`, `b`: цвет текста

#### `bool ok() const`

Возвращает `true`, если шрифт, surface и texture готовы.

#### `std::string error() const`

Возвращает последнюю ошибку текста.

#### `void destroy()`

Освобождает surface, texture и шрифт.

#### `void setText(const std::string& newText)`

Перерисовывает текст с новым содержимым.

#### `void setColor(Uint8 r = -1, Uint8 g = -1, Uint8 b = -1)`

Меняет цвет текста и перерисовывает его.

- `-1` сохраняет текущий канал

#### `textData get() const`

Возвращает копию внутренней структуры текста.

#### Вывод в поток

`operator<<` печатает строку, путь к шрифту и RGB-цвет.

### `insdl::Audio`

RAII-обертка для воспроизведения WAV.

#### Конструктор

```cpp
insdl::Audio sound("assets/click.wav");
```

Загрузчик ожидает WAV-файл.

#### `bool ok() const`

Возвращает `true`, если WAV-данные, audio stream и устройство готовы.

#### `std::string error() const`

Возвращает последнюю ошибку аудио.

#### `void play()`

Добавляет загруженные WAV-данные в очередь воспроизведения.

#### `void resume()`

Возобновляет audio device.

#### `void pause()`

Ставит audio device на паузу.

#### `void stop()`

Останавливает воспроизведение и освобождает ресурсы.

#### `audioData get() const`

Возвращает копию внутренней структуры аудио.

#### Вывод в поток

`operator<<` печатает путь к WAV-файлу.

### `insdl::delayMs`, `delaySec`, `delayMin`, `delayHour`

Помощники для сна потока через `std::this_thread::sleep_for`.

Примеры:

```cpp
insdl::delayMs(16);
insdl::delaySec(1);
```

## Примеры

### Ввод с клавиатуры

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
            // движение вверх
        }

        app.update();
    }
}
```

### Отрисовка прямоугольника

```cpp
insdl::Rect box(app, 50, 50, 200, 120);
box.fill(255, 80, 80);
```

### Отрисовка текста

```cpp
insdl::Text label(app.renderer(), "Hello SDL", app.font, 255, 255, 255);
insdl::Rect area(app, 20, 20, 300, 80);
area.fillText(&label);
```

### Отрисовка текстуры

```cpp
insdl::Texture image(app.renderer(), "assets/logo.png");
insdl::Rect slot(app, 100, 100, 256, 256);
slot.fillTexture(&image);
```

### Воспроизведение аудио

```cpp
insdl::Audio click("assets/click.wav");
if (click.ok()) {
    click.play();
}
```

## Замечания и ограничения

- `Texture`, `Text` и `Audio` являются объектами только для перемещения.
- `Text` использует `TTF_RenderText_Solid`, поэтому лучше всего подходит для простого одноцветного текста.
- `Audio` сейчас рассчитан на WAV-файлы.
- `handleEvent(app)` нужно вызывать один раз за кадр перед чтением состояния ввода.
- `exitEvent(app)` оставлен для совместимости, но в обычной работе не нужен.