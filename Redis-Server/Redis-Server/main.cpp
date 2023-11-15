#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>

#include "Server.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[]) 
{
	Server sr1;

    if (sr1.Initialize() && sr1.Configure() && sr1.Connect() && sr1.Listen() && sr1.SendReceiveData()) 
    {
        // All functions succeeded
    }
    else 
    {
        std::cerr << "Initialization or connection failed." << std::endl;
        return 1;
    }


	return 0;
}
