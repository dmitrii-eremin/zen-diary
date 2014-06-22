#include "stdafx.h"
#include "WebWindow.h"
#include "resource.h"

namespace ZenDiary
{
	namespace App
	{		
		const std::string WebWindow::m_window_class = "Zen diary Window Class";

		std::vector<Awesomium::WebView*> WebWindow::m_views;
		std::vector<WebWindow*> WebWindow::m_windows;
		Localization *WebWindow::m_localization = nullptr;

		WebWindow::~WebWindow()
		{
			auto i = std::find(m_views.begin(), m_views.end(), m_web_view);
			if (i != m_views.end())
			{
				m_views.erase(i);
			}
			m_web_view->Destroy();

			m_windows.erase(std::remove_if(m_windows.begin(), m_windows.end(), 
				[this](WebWindow *wnd) -> bool
				{
					if (wnd == this)
					{
						return true;
					}
					return false;
				}), m_windows.end());
		}

		ZD_STATUS WebWindow::EnableDragnDrop(bool enable)
		{
			DragAcceptFiles(m_hwnd, enable ? TRUE : FALSE);
			return ZD_NOERROR;
		}

		WebWindow *WebWindow::Create(const std::string &title, size_t width, size_t height, Awesomium::WebSession *session)
		{
			return new WebWindow(title, width, height, session);
		}

		ZD_STATUS WebWindow::SetLocalization(Localization *loc)
		{
			m_localization = loc;
			return ZD_NOERROR;
		}

		Awesomium::WebView *WebWindow::GetWebView()
		{
			return m_web_view;
		}

		HWND WebWindow::GetHwnd()
		{
			return m_hwnd;
		}

		ZD_STATUS WebWindow::ToggleFullscreen()
		{
			if (!m_fullscreen)
			{
				RECT window_rect = { 0 };
				GetWindowRect(m_hwnd, &window_rect);

				m_old_window_pos = window_rect;

				long old_style = GetWindowLong(m_hwnd, GWL_STYLE);

				old_style &= ~WS_OVERLAPPEDWINDOW;
				old_style |= WS_POPUP;

				SetWindowLong(m_hwnd, GWL_STYLE, old_style);

				POINT screen_size = {
					GetSystemMetrics(SM_CXSCREEN),
					GetSystemMetrics(SM_CYSCREEN)
				};

				window_rect.left = 0;
				window_rect.top = 0;
				window_rect.right = screen_size.x;
				window_rect.bottom = screen_size.y;

				MoveWindow(m_hwnd, window_rect.left, window_rect.top, window_rect.right, window_rect.bottom, TRUE);
			}
			else
			{
				long old_style = GetWindowLong(m_hwnd, GWL_STYLE);

				old_style &= ~WS_POPUP;
				old_style |= WS_OVERLAPPEDWINDOW;

				SetWindowLong(m_hwnd, GWL_STYLE, old_style);

				POINT screen_size = {
					GetSystemMetrics(SM_CXSCREEN),
					GetSystemMetrics(SM_CYSCREEN)
				};

				RECT window_rect = m_old_window_pos;

				MoveWindow(m_hwnd, window_rect.left, window_rect.top, 
					window_rect.right - window_rect.left, 
					window_rect.bottom - window_rect.top, TRUE);
			}

			m_fullscreen = !m_fullscreen;
			return ZD_NOERROR;
		}

		bool WebWindow::IsFullscreenMode() const
		{
			return m_fullscreen;
		}

		void WebWindow::ShowWindow()
		{
			::ShowWindow(m_hwnd, SW_SHOWNORMAL);
		}

		void WebWindow::HideWindow()
		{
			::ShowWindow(m_hwnd, SW_HIDE);
		}

		void WebWindow::SetForegroundWindow()
		{
			::SetForegroundWindow(m_hwnd);
		}

		void WebWindow::OnChangeTitle(Awesomium::WebView* caller,
			const Awesomium::WebString& title) { }

		void WebWindow::OnChangeAddressBar(Awesomium::WebView* caller,
			const Awesomium::WebURL& url) { }

		void WebWindow::OnChangeTooltip(Awesomium::WebView* caller,
			const Awesomium::WebString& tooltip) { }

		void WebWindow::OnChangeTargetURL(Awesomium::WebView* caller,
			const Awesomium::WebURL& url) { }

		void WebWindow::OnChangeCursor(Awesomium::WebView* caller,
			Awesomium::Cursor cursor) { }

		void WebWindow::OnChangeFocus(Awesomium::WebView* caller,
			Awesomium::FocusedElementType focused_type) { }

		void WebWindow::OnShowCreatedWebView(Awesomium::WebView* caller,
			Awesomium::WebView* new_view, const Awesomium::WebURL& opener_url,
			const Awesomium::WebURL& target_url, const Awesomium::Rect& initial_pos,
			bool is_popup) { }

		void WebWindow::OnAddConsoleMessage(Awesomium::WebView* caller,
			const Awesomium::WebString& message, int line_number,
			const Awesomium::WebString& source) { }

		WebWindow::WebWindow(const std::string &title, size_t width, size_t height, Awesomium::WebSession *session) :
			m_web_view(nullptr),
			m_fullscreen(false)			
		{
			m_old_window_pos = { 0 };

			PlatformInit();

			m_web_view = Awesomium::WebCore::instance()->CreateWebView(width * 2, height * 2, session,
				Awesomium::kWebViewType_Window);

			m_views.push_back(m_web_view);

			m_web_view->set_view_listener(this);

			HINSTANCE hInstance = GetModuleHandle(nullptr);

			int window_width = width;
			int window_height = height;

			int window_left = GetSystemMetrics(SM_CXSCREEN) / 2 - window_width / 2;
			int window_top = GetSystemMetrics(SM_CYSCREEN) / 2 - window_height / 2;

			m_hwnd = CreateWindow(m_window_class.c_str(), title.c_str(),
				WS_OVERLAPPEDWINDOW, window_left, window_top,
				window_width, window_height, nullptr, nullptr, hInstance, nullptr);

			m_web_view->set_parent_window(m_hwnd);

			ShowWindow();
			UpdateWindow(m_hwnd);		

			m_windows.push_back(this);
		}

		void WebWindow::PlatformInit()
		{
			static bool already_initialized = false;
			if (already_initialized)
			{
				return;
			}

			already_initialized = true;

			WNDCLASSEX wc = { 0 };
			wc.cbSize = sizeof(WNDCLASSEX);
			wc.style = 0;
			wc.lpfnWndProc = &WebWindow::WndProc;
			wc.cbWndExtra = 0;
			wc.cbClsExtra = 0;
			wc.hInstance = GetModuleHandle(nullptr);
			wc.hIcon = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1));
			wc.hIconSm = LoadIcon(GetModuleHandle(nullptr), MAKEINTRESOURCE(IDI_ICON1));
			wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wc.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
			wc.lpszMenuName = nullptr;
			wc.lpszClassName = m_window_class.c_str();

			RegisterClassEx(&wc);			
		}

		LRESULT CALLBACK WebWindow::WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
		{
			switch (msg)
			{
			case WM_CLOSE:
				PostQuitMessage(0);
				break;
			case WM_SIZE:
				{
					int width = LOWORD(lParam);
					int height = HIWORD(lParam);

					for (auto &i : m_views)
					{
						i->Resize(width, height);					
					}
				}
				break;
			case  WM_DROPFILES:
				HandleDragnDrop(wParam);
				break;
			case WM_COMMAND:
				HandlePopupMenu(LOWORD(wParam));
				break;			
			case ZD_WM_NOTIFY_ICON:
				switch (lParam)
				{
				case WM_LBUTTONDBLCLK:
					HandlePopupMenu(ZD_WM_SHOW_WINDOW);
					break;
				case WM_RBUTTONUP:
					{
						std::string show_window("Show window");
						std::string exit("Exit");

						if (m_localization)
						{
							show_window = Helpers::String::ConvertUtf8ToMB(m_localization->Get("ui.popup.show-window"));
							exit = Helpers::String::ConvertUtf8ToMB(m_localization->Get("ui.popup.exit"));
						}

						for (auto &i : m_windows)
						{
							if (!i || !i->GetHwnd())
							{
								continue;
							}

							Helpers::Win32::PopupMenu(i->GetHwnd(), { new Helpers::Win32::PopupItem(ZD_WM_SHOW_WINDOW, show_window),
								new Helpers::Win32::PopupDivider(), new Helpers::Win32::PopupItem(ZD_WM_EXIT, exit) });
						}
					}					
					break;
				default:
					break;
				}				
				break;			
			default:
				break;
			}
		
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}

		LRESULT WebWindow::HandleDragnDrop(WPARAM wParam)
		{
			HDROP hDrop = reinterpret_cast<HDROP>(wParam);
			
			int count = DragQueryFile(hDrop, 0xFFFFFFFF, nullptr, 0);

			for (int i = 0; i < count; i++)
			{
				char cfname[MAX_PATH];
				DragQueryFile(hDrop, i, cfname, MAX_PATH);

				std::string fname(Helpers::String::Replace(cfname, "\\", "\\\\"));

				std::stringstream js_handler;
				js_handler << "zen_handlers.drag_n_drop(\"" << Helpers::String::ToUtf8(Helpers::String::strtowstr(fname)) << "\");";

				for (auto &j : m_views)
				{					
					j->ExecuteJavascript(Awesomium::WSLit(js_handler.str().c_str()), Awesomium::WSLit(""));
				}
			}

			DragFinish(hDrop);
			return 0;
		}	

		LRESULT WebWindow::HandlePopupMenu(WORD message_id)
		{
			std::stringstream js_handler;
			js_handler << "zen_handlers.handle_popup_menu(" << message_id << ");";
			for (auto &j : m_views)
			{
				j->ExecuteJavascript(Awesomium::WSLit(js_handler.str().c_str()), Awesomium::WSLit(""));
			}
			return 0;
		}
	}
}