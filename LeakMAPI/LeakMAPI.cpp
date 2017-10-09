#include "stdafx.h"
#define USES_IID_IMAPIClientShutdown
#include <initguid.h>
#include <MAPIX.h>
#include <string>

void DoFastShutdown()
{
	printf("Setting up for DoFastShutdown\r\n");
	LPMAPISESSION lpMAPISession = nullptr;

	HRESULT hRes = ::MAPILogonEx(
		NULL,
		nullptr,
		nullptr,
		MAPI_LOGON_UI | MAPI_EXTENDED | MAPI_NEW_SESSION,
		&lpMAPISession);
	printf("MAPILogonEx returned 0x%08X\r\n", hRes);
	if (SUCCEEDED(hRes) && lpMAPISession)
	{
		LPMAPICLIENTSHUTDOWN lpMAPIClientShutdown = nullptr;
		hRes = lpMAPISession->QueryInterface(IID_IMAPIClientShutdown, (LPVOID*)&lpMAPIClientShutdown);
		printf("QueryInterface(IID_IMAPIClientShutdown) returned 0x%08X\r\n", hRes);
		if (SUCCEEDED(hRes) && lpMAPIClientShutdown)
		{
			hRes = lpMAPIClientShutdown->QueryFastShutdown();
			printf("QueryFastShutdown returned 0x%08X\r\n", hRes);

			if (SUCCEEDED(hRes))
			{
				hRes = lpMAPIClientShutdown->DoFastShutdown();
				printf("DoFastShutdown returned 0x%08X\r\n", hRes);
			}
		}

		if (lpMAPIClientShutdown) lpMAPIClientShutdown->Release();
	}

	if (lpMAPISession) lpMAPISession->Release();
}

void DoMAPILogonExLoop()
{
	for (int i = 0; i < 5; i++) {
		printf("Step %d\r\n", i + 1);
		printf("Initializing MAPI\r\n");
		MAPIINIT_0 mapiInit = { MAPI_INIT_VERSION, 0 };
		HRESULT hRes = ::MAPIInitialize(&mapiInit);

		if (!FAILED(hRes))
		{
			LPMAPISESSION lpSession = 0;
			printf("MAPILogonEx started\r\n");

			hRes = MAPILogonEx(NULL, 0, 0, MAPI_EXTENDED | MAPI_USE_DEFAULT, &lpSession);
			if (FAILED(hRes))
			{
				printf("MAPILogonEx Failed\r\n");
				return;
			}

			printf("MAPILogonEx Ended\r\n");

			if (lpSession)
			{
				printf("Logging off\r\n");
				lpSession->Logoff(0, 0, 0); // critical to the crash
				printf("Releasing session\r\n");
				lpSession->Release();
				lpSession = NULL;
			}
		}

		printf("Uninitializing MAPI\r\n");
		MAPIUninitialize();
		Sleep(1000);
	}

	MAPIUninitialize();
}

void DisplayUsage()
{
	printf("LeakMAPI - MAPI leak tester\n");
	printf("   Test various ways of leaking MAPI.\n");
	printf("\n");
	printf("Usage:  LeakMAPI [fastshutdown] [loop] [uninit]\n");
	printf("\n");
	printf("Options:\n");
	printf("        fastshutdown Run DoFastShutdown.\n");
	printf("        loop Run MAPILogonEx in a loop\n");
	printf("        unint Run MAPIUninitialize.\n");
}

void main(int argc, char* argv[])
{
	bool doFastShutdown = false;
	bool doLoop = false;
	bool doMAPIUninitialize = false;
	if (argc >= 2)
	{
		for (auto i = 1; i < argc; i++)
		{
			if (std::string("fastshutdown") == argv[i])
			{
				doFastShutdown = true;
				printf("Will run DoFastShutdown.\r\n\r\n");
			}
			else if (std::string("loop") == argv[i])
			{
				doLoop = true;
				printf("Will run MAPILogonEx in a loop.\r\n\r\n");
			}
			else if (std::string("uninit") == argv[i])
			{
				doMAPIUninitialize = true;
				printf("Will run MAPIUninitialize.\r\n\r\n");
			}
			else
			{
				DisplayUsage();
				return;
			}
		}
	}

	if (doLoop) {
		DoMAPILogonExLoop();
	}
	else
	{
		printf("Initializing MAPI\r\n");
		MAPIINIT_0 mapiInit = { MAPI_INIT_VERSION, 0 };
		HRESULT hRes = ::MAPIInitialize(&mapiInit);

		if (doFastShutdown) DoFastShutdown();

		if (doMAPIUninitialize)
		{
			printf("Uninitializing MAPI\r\n");
			::MAPIUninitialize();
		}
	}

	printf("Exiting\r\n");
	return;
}