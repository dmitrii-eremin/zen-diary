#include "stdafx.h"
#include "ZenDiaryApp.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	
	std::shared_ptr<ZenDiary::App::ZenDiaryApp> app(std::make_shared<ZenDiary::App::ZenDiaryApp>());
	ZD_STATUS status = app->Initialize(lpCmdLine);
	if (ZD_FAILED(status))
	{
		return status;
	}

	status = app->Run();

	app->Deinitialize();
	return status;
}