#include "MPRDBC.h"
#include <stdexcept>
#include <iostream>

int main(int argc,char **argv)
{
    try
    {
        MPRDBC mprdbc;
        mprdbc.init(argc,argv);
        mprdbc.launch();
    }
    catch (const std::runtime_error &error)
    {
        std::cerr << error.what() << std::endl;
        return 1;
    }


    return 0;
}
