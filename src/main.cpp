#include <InSDL.hpp>

int main()
{
    insdl::App myapp;
    myapp.init(700, 700, "My InSDL Window");

    while (!myapp.quit) {
        insdl::handleEvent(myapp);
        
        // code here

        myapp.update();
    }

    myapp.exit();
    return 0;
}
