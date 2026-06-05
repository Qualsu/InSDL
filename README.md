<img src="logo.png" width="300px">

# InSDL - a framework for SDL3

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)

You can download the latest release from the [Releases](https://github.com/Qualsu/InSDL/releases) page

For a convenient way to create projects, use [InSDL Create](https://github.com/Qualsu/InSDL_create)

### [English documentation](docs/EN.md) / [Russian documentation](docs/RU.md)

# Usage template

```cpp
#include <InSDL/InSDL.hpp>

int main()
{
    insdl::App myApp;
    myApp.init(700, 700, "My InSDL Window");

    while (!myApp.quit) {
        insdl::handleEvent(myApp);

        // code here

        myApp.update();
    }

    return 0;
}
```
