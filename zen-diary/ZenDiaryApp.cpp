#include "stdafx.h"
#include "ZenDiaryApp.h"

namespace ZenDiary
{
	namespace App
	{
		ZenDiaryApp::ZenDiaryApp()
		{
			m_terminate.store(false);
		}

		ZenDiaryApp::~ZenDiaryApp()
		{

		}

		ZD_STATUS ZenDiaryApp::Initialize(const std::string &argv)
		{
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::Deinitialize()
		{
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::Run()
		{
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::Terminate()
		{
			m_terminate.store(true);
			return ZD_NOERROR;
		}
	}
}