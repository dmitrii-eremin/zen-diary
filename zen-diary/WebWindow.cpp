#include "stdafx.h"
#include "WebWindow.h"

namespace ZenDiary
{
	namespace App
	{
		const std::string WebWindow::m_window_class = "Zen diary Window Class";

		std::vector<Awesomium::WebView*> WebWindow::m_views;

		WebWindow::~WebWindow()
		{
			auto i = std::find(m_views.begin(), m_views.end(), m_web_view);
			if (i != m_views.end())
			{
				m_views.erase(i);
			}
			m_web_view->Destroy();
		}

		WebWindow *WebWindow::Create(const std::string &title, size_t width, size_t height, Awesomium::WebSession *session)
		{
			return new WebWindow(title, width, height, session);
		}

		Awesomium::WebView *WebWindow::GetWebView()
		{
			return m_web_view;
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
			m_web_view(nullptr)
		{
			PlatformInit();

			m_web_view = Awesomium::WebCore::instance()->CreateWebView(width * 2, height * 2, session,
				Awesomium::kWebViewType_Window);

			m_views.push_back(m_web_view);

			m_web_view->set_view_listener(this);

			HINSTANCE hInstance = GetModuleHandle(nullptr);

			int window_width = width + 20;
			int window_height = height + 40;

			int window_left = GetSystemMetrics(SM_CXSCREEN) / 2 - window_width / 2;
			int window_top = GetSystemMetrics(SM_CYSCREEN) / 2 - window_height / 2;

			m_hwnd = CreateWindow(m_window_class.c_str(), title.c_str(),
				WS_OVERLAPPEDWINDOW, window_left, window_top,
				window_width, window_height, nullptr, nullptr, hInstance, nullptr);

			m_web_view->set_parent_window(m_hwnd);

			ShowWindow(m_hwnd, SW_SHOWNORMAL);
			UpdateWindow(m_hwnd);
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
			wc.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
			wc.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);
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
			default:
				break;
			}
			return DefWindowProc(hwnd, msg, wParam, lParam);
		}
	}
}