#include "stdafx.h"

#pragma comment(lib, "awesomium.lib")
#pragma comment(lib, "sqlite3.lib")
#pragma comment(lib, "wininet.lib")

#ifdef _DEBUG
#pragma comment(lib, "JsonBox-d.lib")
#pragma comment(lib, "aes-d.lib")
#else
#pragma comment(lib, "JsonBox.lib")
#pragma comment(lib, "aes.lib")
#endif