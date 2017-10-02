// LeakMAPI.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <MAPIX.h>


int main()
{
	printf("Initializing MAPI\r\n");
	MAPIINIT_0 mapiInit = { MAPI_INIT_VERSION, 0 };
	HRESULT hr = ::MAPIInitialize(&mapiInit);

	//printf("Uninitializing MAPI\r\n");
	//::MAPIUninitialize();
	return 0;
}