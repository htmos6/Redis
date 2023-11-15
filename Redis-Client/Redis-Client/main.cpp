#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>

#include "Client.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[]) 
{
	Client cl1;

    if (cl1.Initialize() && cl1.Configure(argv) && cl1.Connect() && cl1.SendReceiveData("Hello, test client!")) 
    {
        // All functions succeeded
    }
    else 
    {
        // At least one function failed
        std::cerr << "Client initialization or connection failed." << std::endl;
        return 1;
    }

	return 0;
}
