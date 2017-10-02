# LeakMAPI
Short test harness to leak MAPI

Calls [MAPIInitialize](https://msdn.microsoft.com/en-us/library/office/cc842343.aspx) and exits without calling [MAPIUninitialize](https://msdn.microsoft.com/en-us/library/office/cc765647.aspx). Useful for demonstrating the dangers of leaking MAPI.
