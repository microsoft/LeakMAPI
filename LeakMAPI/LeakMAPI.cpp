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

void DisplayUsage()
{
	printf("LeakMAPI - MAPI leak tester\n");
	printf("   Test various ways of leaking MAPI.\n");
	printf("\n");
	printf("Usage:  LeakMAPI [fastshutdown] [uninit]\n");
	printf("\n");
	printf("Options:\n");
	printf("        fastshutdown Run DoFastShutdown.\n");
	printf("        unint Run MAPIUninitialize.\n");
}

void main(int argc, char* argv[])
{
	bool doFastShutdown = false;
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

	printf("Initializing MAPI\r\n");
	MAPIINIT_0 mapiInit = { MAPI_INIT_VERSION, 0 };
	HRESULT hr = ::MAPIInitialize(&mapiInit);
	if (doFastShutdown) DoFastShutdown();

	if (doMAPIUninitialize)
	{
		printf("Uninitializing MAPI\r\n");
		::MAPIUninitialize();
	}

	return;
}