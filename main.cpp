#include <iostream>

#include "test.h"

int main(int argc, char** argv)
{
    if(test_internal()) {
        std::cout << "Internal Test OK!" << std::endl;
    } else {
        std::cout << "Error: Internal Test Failed!" << std::endl;
    }

    return 0;
}
