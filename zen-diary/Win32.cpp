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

			ZD_STATUS CreateNotifyIcon(HWND hwnd, HICON hIcon, int message_id, const std::string &title)
			{
				NOTIFYICONDATA nid = { 0 };

				nid.cbSize = sizeof(NOTIFYICONDATA);
				nid.hWnd = hwnd;
				nid.uID = 100;
				nid.uVersion = NOTIFYICON_VERSION;
				nid.uCallbackMessage = message_id;
				nid.hIcon = hIcon;
				strcpy_s(nid.szTip, 128, title.c_str());
				nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;

				Shell_NotifyIcon(NIM_ADD, &nid);
				return ZD_NOERROR;
			}

			ZD_STATUS DeleteNotifyIcon(HWND hwnd)
			{
				NOTIFYICONDATA nid = { 0 };

				nid.cbSize = sizeof(NOTIFYICONDATA);
				nid.hWnd = hwnd;
				nid.uID = 100;

				Shell_NotifyIcon(NIM_DELETE, &nid);
				return ZD_NOERROR;
			}			

			ZD_STATUS PopupMenu(HWND hwnd, const std::vector<PopupItemBase*> &items)
			{
				HMENU hMenu = CreatePopupMenu();
				if (hMenu)
				{
					for (auto &i : items)
					{
						if (!i)
						{
							continue;
						}

						if (i->IsDivider())
						{
							MENUITEMINFO separator_item = { 0 };
							separator_item.cbSize = sizeof(MENUITEMINFO);
							separator_item.fMask = MIIM_FTYPE;
							separator_item.fType = MFT_SEPARATOR;

							InsertMenuItem(hMenu, -1, FALSE, &separator_item);
						}
						else
						{
							InsertMenu(hMenu, -1, MF_BYPOSITION, i->GetMessageId(), i->GetTitle().c_str());
						}
					}
				}

				POINT pt = { 0 };
				GetCursorPos(&pt);
				WORD wm_message = TrackPopupMenu(hMenu, TPM_RIGHTBUTTON, pt.x, pt.y, 0, hwnd, nullptr);				
				PostMessage(hwnd, WM_COMMAND, wm_message, 0);				
				DestroyMenu(hMenu);

				for (auto &i : items)
				{
					delete i;
				}

				return ZD_NOERROR;
			}
		}
	}
}