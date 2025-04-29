#include <iostream>
//
// Created by gaeqs on 28/04/25.
//
extern char** environ;

int main()
{
    auto env = environ;
    auto cPath = getenv("BLUE_CONFIG_DATA");
    if (cPath == nullptr) {
        std::cout << "BLUE_CONFIG_DATA environment variable not set." << std::endl;;
        return 0;
    }


    auto test = environ;
    std::cout << "Hello world!" << std::endl;
    return 0;
}