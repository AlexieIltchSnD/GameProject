#include <iostream>
#include "Controller.h"
// Main function.
int main(const int argc, char *argv[]) {
    std::cout.precision(2);
    std::cout << std::fixed;

    Controller controller;
    controller.run(argc, argv);
    return 0;
}