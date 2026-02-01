<img src="logo.png" width="300px">

# InSDL — фреймворк для SDL3

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

Скачать для добавление в проект можно во вкладке [Релизов](https://github.com/Qualsu/InSDL/releases)

Для удобной работы с проектами используйте [InSDL Create](https://github.com/Qualsu/InSDL_create)


# Шаблон использования
```cpp
#include <InSDL.hpp>

int main()
{
    app myapp;
    myapp.init(700, 700, "My InSDL Window");

    while (!myapp.quit) {
        handleEvent(myapp);
        
        // code here

        myapp.update();
    }

    myapp.exit();
    return 0;
}
```