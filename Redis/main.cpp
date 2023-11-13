#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <iostream>

#include "Socket.h"

#pragma comment(lib, "Ws2_32.lib")

int main(int argc, char* argv[]) {


	Socket sc1;
	sc1.Initialize();
	sc1.ConfigureConnection();
	sc1.SetupAddress(argv);
	sc1.ConfigureSocket();
	sc1.Connect();
	sc1.SendReceiveData();

	return 0;
}