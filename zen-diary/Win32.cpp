#include "stdafx.h"
#include "ZenDiary.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Win32
		{
			ZD_STATUS SetClipboardText(const std::string &text)
			{
				if (OpenClipboard(nullptr))
				{
					EmptyClipboard();
					HGLOBAL clipbuffer = GlobalAlloc(GMEM_DDESHARE, text.length() + 1);
					char *buffer = static_cast<char*>(GlobalLock(clipbuffer));
					strcpy_s(buffer, text.length() + 1, text.c_str());
					GlobalUnlock(clipbuffer);
					SetClipboardData(CF_TEXT, clipbuffer);
					CloseClipboard();
				}

				return ZD_NOERROR;
			}
		}
	}
}