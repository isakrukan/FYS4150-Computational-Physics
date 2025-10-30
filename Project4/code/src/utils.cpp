
#include "utils.hpp"

#include <iostream>

int mod(int a, int b)
{
    if (b <= 0)
    {
        std::cerr << "Error: divisor must be > 0 in mod()\n";
        std::exit(EXIT_FAILURE);
    }
    return (a + b) % b;
}