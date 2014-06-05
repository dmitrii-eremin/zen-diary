#include <windows.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	UINT res = WinExec(".\\bin\\zen-diary.exe", SW_SHOW);	
	int err = GetLastError();
	return 0;
}