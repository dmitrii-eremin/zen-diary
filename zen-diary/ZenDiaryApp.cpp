#include "stdafx.h"
#include "ZenDiaryApp.h"

namespace ZenDiary
{
	namespace App
	{
		ZenDiaryApp::ZenDiaryApp() : 
			m_core(nullptr),
			m_window(nullptr)
		{
			m_terminate.store(false);
		}

		ZenDiaryApp::~ZenDiaryApp()
		{

		}

		ZD_STATUS ZenDiaryApp::Initialize(const std::string &argv)
		{
			ZD_STATUS status = InitializeWindow();
			if (ZD_FAILED(status))
			{
				return status;
			}
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::Deinitialize()
		{
			FreeWindow();
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::Run()
		{
			MSG msg = { 0 };

			while (!m_terminate.load())
			{
				while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
				{
					if (msg.message == WM_QUIT)
					{
						Terminate();
					}

					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}

				ZD_SAFE_CALL(m_core)->Update();
				Sleep(1);
			}
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::Terminate()
		{
			m_terminate.store(true);
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::InitializeWindow()
		{
			Awesomium::WebConfig config;

			config.log_level = Awesomium::kLogLevel_Verbose;
			config.remote_debugging_port = 9922;
			config.remote_debugging_host = Awesomium::WSLit("127.0.0.1");

			m_core = Awesomium::WebCore::Initialize(config);
			m_window = WebWindow::Create("Task5", 1200, 700);

			Awesomium::WebView *view = m_window->GetWebView();

			BindMethods();

			view->set_menu_listener(&m_menu_handler);
			view->set_js_method_handler(&m_method_handler);

			view->LoadURL(Awesomium::WebURL(Awesomium::WSLit(
				Helpers::String::FilenameToUrl(GetFullname("signin.html")).c_str())));
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::FreeWindow()
		{
			ZD_SAFE_DELETE(m_window);
			Awesomium::WebCore::Shutdown();

			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::BindMethods()
		{
			if (!m_window)
			{
				return ZD_ERROR_NOT_INITIALIZED;
			}

			Awesomium::WebView *view = m_window->GetWebView();
			if (!view)
			{
				return ZD_ERROR_NOT_INITIALIZED;
			}

			Awesomium::JSValue result = view->CreateGlobalJavascriptObject(Awesomium::WSLit("zen"));
			if (!result.IsObject())
			{
				return ZD_ERROR_FAILED_TO_CREATE;
			}

			Awesomium::JSObject &zen_diary = result.ToObject();

			ZD_BIND_JS_HANDLER("alert", &JSHandlers::OnAlert);
			return ZD_NOERROR;
		}

		std::string ZenDiaryApp::GetFullname(const std::string &filename)
		{
			const size_t buf_size = MAX_PATH;
			char buf[buf_size];

			GetModuleFileName(GetModuleHandle(nullptr), buf, buf_size);

			std::string path = Helpers::String::ExtractPath(buf);

			std::stringstream stream;
			stream << path << m_httpdocs_path << filename;

			return stream.str();
		}
	}
}