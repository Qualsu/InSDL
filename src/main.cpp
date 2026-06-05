#include <InSDL.hpp>

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
