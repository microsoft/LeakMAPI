# LeakMAPI
Short test harness to leak MAPI

Calls [MAPIInitialize](https://msdn.microsoft.com/en-us/library/office/cc842343.aspx) and exits without calling [MAPIUninitialize](https://msdn.microsoft.com/en-us/library/office/cc765647.aspx). Useful for demonstrating the dangers of leaking MAPI.

```
C:\src\LeakMAPI\Debug>leakmapi ?
LeakMAPI - MAPI leak tester
   Test various ways of leaking MAPI.

Usage:  LeakMAPI [fastshutdown] [uninit]

Options:
        fastshutdown Run DoFastShutdown.
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
C:\src\LeakMAPI\Debug>leakmapi
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
