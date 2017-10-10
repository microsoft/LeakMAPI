# LeakMAPI
Short test harness to leak MAPI

Calls [MAPIInitialize](https://msdn.microsoft.com/en-us/library/office/cc842343.aspx) and exits without calling [MAPIUninitialize](https://msdn.microsoft.com/en-us/library/office/cc765647.aspx). Useful for demonstrating the dangers of leaking MAPI.

```
C:\src\LeakMAPI\Debug>leakmapi ?
LeakMAPI - MAPI leak tester
   Test various ways of leaking MAPI.

Usage:  LeakMAPI [fastshutdown] [loop] [uninit]

Options:
        fastshutdown Run DoFastShutdown.
        loop Run MAPIInitialize in a loop
        unint Run MAPIUninitialize.
```

# Testing MAPI Leaks
## Scenario 1 - MAPIUninitialize not called
```
C:\src\LeakMAPI\Debug>leakmapi
Initializing MAPI
Exiting
```

## Scenario 2 - DoFastShutdown called
Use (https://msdn.microsoft.com/en-us/library/office/dd941319.aspx)[DoFastShutdown]. According to (https://msdn.microsoft.com/en-us/library/office/dd941346.aspx)[Best Practices], we should not call MAPIUninitialize.
```
C:\src\LeakMAPI\Debug>leakmapi fastshutdown
Will run DoFastShutdown.

Initializing MAPI
Setting up for DoFastShutdown
MAPILogonEx returned 0x00000000
QueryInterface(IID_IMAPIClientShutdown) returned 0x00000000
QueryFastShutdown returned 0x00000000
DoFastShutdown returned 0x00000000
Exiting
```

## Scenario 3 - MAPIUninitialize called
No leak here.
```
C:\src\LeakMAPI\Debug>leakmapi uninit
Will run MAPIUninitialize.

Initializing MAPI
Uninitializing MAPI
Exiting
```

## Scenario 4 - Loop MAPIInitialize calls
Might crash on Windows 7
```
C:\src\LeakMAPI\Debug>LeakMAPI.exe loop
Will run MAPILogonEx in a loop.

Step 1
Initializing MAPI
Uninitializing MAPI
Step 2
Initializing MAPI
Uninitializing MAPI
etc...
```
