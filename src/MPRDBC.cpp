#include <iostream>
#include "MPRDBC.h"

void MPRDBC::printHelp()
{
    std::cout << "Usage: prd-client [COMMAND] [OPTIONS] \n"
              << "Available commands: \n"
              << "  new \n"
              << "  delete \n"
              << "  import \n"
              << "\n"
              << "Option: \n"
              << "  -h, --help              Display this help and exit \n"
              << "  --key        [value]                               \n"
              << "  --player     [value]                               \n"
              << "  --remote     [value]                               \n"
              << "  --comment    [value]                               \n"
              << "  --banList    [value]                               \n"
              << "  --submit     [value]                               \n"
              << std::endl;
}

MPRDBC::MPRDBC()
{

}


