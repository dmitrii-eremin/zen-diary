#include "stdafx.h"
#include "ZenDiaryApp.h"

namespace ZenDiary
{
	namespace App
	{
		ZenDiaryApp::ZenDiaryApp() : 
			m_core(nullptr),
			m_window(nullptr),
			m_web_session(nullptr)		
		{
			m_terminate.store(false);
		}

		ZenDiaryApp::~ZenDiaryApp()
		{
			
		}

		ZD_STATUS ZenDiaryApp::Initialize(const std::string &argv)
		{
			InitializeCurrentDirectory();

			m_args.Parse(argv);	
			
			m_updater.CheckForUpdates();
			m_updater.DownloadUsersCount();

			Helpers::Serialization::FromFile(m_settings, m_settings_path);			

			srand(static_cast<uint_t>(time(nullptr)));

			ZD_RETURN_IF_FAILED(LoadMimeTypes());

			ZD_RETURN_IF_FAILED(InitializeCryptography());
			ZD_RETURN_IF_FAILED(InitializeDirectories());
			ZD_RETURN_IF_FAILED(InitializeDatabase());			
			ZD_RETURN_IF_FAILED(InitializeWindow());
			ZD_RETURN_IF_FAILED(LoadLocalization());
			ZD_RETURN_IF_FAILED(InitializeJsHandlers());
			ZD_RETURN_IF_FAILED(InitializeDatabaseList());
			
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::Deinitialize()
		{
			m_database.Close();

			DatabaseSettings &db_settings = m_settings.GetDatabaseSettings();
			size_t db_list_size = m_db_list.GetItemsCount();
			db_settings.ClearDbItems();
			for (size_t i = 0; i < db_list_size; i++)
			{
				DatabaseListItem *item = m_db_list.GetItemAt(i);
				if (item)
				{
					db_settings.AddDbItem(*item);
				}
			}

			Helpers::Serialization::ToFile(m_settings, m_settings_path);

			FreeWindow();

			if (m_updater.IsNeedToUpdate())
			{
				std::stringstream update_message;

				update_message << Helpers::String::ConvertUtf8ToMB(m_localization.Get("general.updater.update-available")) << " " << 
					m_updater.GetNewVersion() << ". " << Helpers::String::ConvertUtf8ToMB(m_localization.Get("general.updater.many-new-features-available"));
				if (m_updater.GetChangeLog().length() > 0)
				{
					update_message << ":" << std::endl << std::endl;
					update_message << m_updater.GetChangeLog() << std::endl;
				}
				else
				{
					update_message << "." << std::endl;
				}				
				update_message << Helpers::String::ConvertUtf8ToMB(m_localization.Get("general.updater.download-now"));

				int result = MessageBox(nullptr, update_message.str().c_str(),
					"Zen Diary", MB_YESNO | MB_ICONQUESTION);

				if (result == IDYES)
				{
					m_updater.OpenUpdateLink();
				}
			}
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

		ZD_STATUS ZenDiaryApp::InitializeDatabase()
		{
			const DatabaseSettings &db_settings = m_settings.GetDatabaseSettings();

			if (!m_database.Open(db_settings.GetPath()))
			{
				return ZD_ERROR_FAILED_TO_OPEN;
			}

			std::string db_update_settings;
			ZD_STATUS status = Helpers::Files::GetFileContent(m_db_update_path, db_update_settings);
			if (ZD_SUCCEEDED(status))
			{
				JsonBox::Value root;
				root.loadFromString(db_update_settings);

				DatabaseUpdater db_updater;
				status = db_updater.Update(m_database, root);
				if (ZD_FAILED(status))
				{
					// TODO: Report that database initialization failed
				}
			}
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::InitializeWindow()
		{
			Awesomium::WebConfig config;

			config.log_level = Awesomium::kLogLevel_Verbose;

#ifdef _DEBUG
			config.remote_debugging_port = m_remote_debugging_port;
			config.remote_debugging_host = Awesomium::WSLit("127.0.0.1");
#endif

			const WindowSettings &window_settings = m_settings.GetGuiSettings().GetWindowSettings();

			m_core = Awesomium::WebCore::Initialize(config);

			m_web_session = m_core->CreateWebSession(Awesomium::WSLit("zen-web-session"), Awesomium::WebPreferences());
			m_data_source.SetHomePath(m_httpdocs_path);
			m_web_session->AddDataSource(Awesomium::WSLit("zen-diary"), &m_data_source);

			m_window = WebWindow::Create(window_settings.GetTitle(), 
				window_settings.GetWidth(), window_settings.GetHeight(), m_web_session);

			m_window->EnableDragnDrop(true);

			Awesomium::WebView *view = m_window->GetWebView();

			BindMethods();

			view->set_menu_listener(&m_menu_handler);
			view->set_js_method_handler(&m_method_handler);					

			const std::string start_url("asset://zen-diary/signin.html");			

			view->LoadURL(Awesomium::WebURL(Awesomium::WSLit(start_url.c_str())));
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::FreeWindow()
		{				
			ZD_SAFE_CALL(m_web_session)->Release();

			ZD_SAFE_DELETE(m_window);
			Awesomium::WebCore::Shutdown();

			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::InitializeCryptography()
		{
			aes_init();
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::InitializeDirectories()
		{
			const DatabaseSettings &db_settings = m_settings.GetDatabaseSettings();

			const std::deque<std::string> directories = {
				Helpers::String::ExtractPath(m_settings_path), Helpers::String::ExtractPath(db_settings.GetPath())
			};

			for (auto &path : directories)
			{				
				boost::filesystem::create_directories(boost::filesystem::path(path));
			}

			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::InitializeJsHandlers()
		{
			m_js_handlers.SetZenApp(this);
			m_js_handlers.SetWebWindow(m_window);
			m_js_handlers.SetGlobalSettings(&m_settings);
			m_js_handlers.SetDatabase(&m_database);
			m_js_handlers.SetUpdater(&m_updater);
			m_js_handlers.SetSpellChecker(&m_spell_checker);
			m_js_handlers.SetLocalization(&m_localization);
			m_js_handlers.SetDatabaseList(&m_db_list);
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

			ZD_BIND_JS_HANDLER("toInt", &JSHandlers::OnToInt);

			ZD_BIND_JS_HANDLER("setFileContent", &JSHandlers::OnSetFileContent);
			ZD_BIND_JS_HANDLER("getFileContent", &JSHandlers::OnGetFileContent);

			ZD_BIND_JS_HANDLER("deleteFile", &JSHandlers::OnDeleteFile);

			ZD_BIND_JS_HANDLER("toUpper", &JSHandlers::OnToUpper);
			ZD_BIND_JS_HANDLER("toLower", &JSHandlers::OnToLower);

			ZD_BIND_JS_HANDLER("uriEncode", &JSHandlers::OnUriEncode);
			ZD_BIND_JS_HANDLER("uriDecode", &JSHandlers::OnUriDecode);

			ZD_BIND_JS_HANDLER("alert", &JSHandlers::OnAlert);
			ZD_BIND_JS_HANDLER("shellExecute", &JSHandlers::OnShellExecute);
			ZD_BIND_JS_HANDLER("getTemplate", &JSHandlers::OnGetTemplate);
			ZD_BIND_JS_HANDLER("getVersionString", &JSHandlers::OnGetVersionString);
			ZD_BIND_JS_HANDLER("getUsername", &JSHandlers::OnGetUsername);
			ZD_BIND_JS_HANDLER("isFirstRun", &JSHandlers::OnIsFirstRun);

			ZD_BIND_JS_HANDLER("registerUser", &JSHandlers::OnRegisterUser);
			ZD_BIND_JS_HANDLER("loginUser", &JSHandlers::OnLoginUser);
			ZD_BIND_JS_HANDLER("changeCredits", &JSHandlers::OnChangeCredits);
			ZD_BIND_JS_HANDLER("logoutUser", &JSHandlers::OnLogoutUser);

			ZD_BIND_JS_HANDLER("getDatabasePath", &JSHandlers::OnGetDatabasePath);
			ZD_BIND_JS_HANDLER("setDatabasePath", &JSHandlers::OnSetDatabasePath);

			ZD_BIND_JS_HANDLER("getUseJavascript", &JSHandlers::OnGetUseJavascript);
			ZD_BIND_JS_HANDLER("setUseJavascript", &JSHandlers::OnSetUseJavascript);

			ZD_BIND_JS_HANDLER("getShowLineNumbers", &JSHandlers::OnGetShowLineNumbers);
			ZD_BIND_JS_HANDLER("setShowLineNumbers", &JSHandlers::OnSetShowLineNumbers);

			ZD_BIND_JS_HANDLER("postNote", &JSHandlers::OnPostNote);
			ZD_BIND_JS_HANDLER("updateNote", &JSHandlers::OnUpdateNote);

			ZD_BIND_JS_HANDLER("getNotes", &JSHandlers::OnGetNotes);
			ZD_BIND_JS_HANDLER("getNote", &JSHandlers::OnGetNote);
			ZD_BIND_JS_HANDLER("isNoteEncrypted", &JSHandlers::OnIsNoteEncrypted);

			ZD_BIND_JS_HANDLER("hideNote", &JSHandlers::OnHideNote);
			ZD_BIND_JS_HANDLER("showNote", &JSHandlers::OnShowNote);
			ZD_BIND_JS_HANDLER("deleteNote", &JSHandlers::OnDeleteNote);

			ZD_BIND_JS_HANDLER("openFileDialog", &JSHandlers::OnOpenFileDialog);
			ZD_BIND_JS_HANDLER("saveFileDialog", &JSHandlers::OnSaveFileDialog);
			ZD_BIND_JS_HANDLER("setClipboard", &JSHandlers::OnSetClipboard);

			ZD_BIND_JS_HANDLER("getUsersCount", &JSHandlers::OnGetUsersCount);

			ZD_BIND_JS_HANDLER("toggleFullscreen", &JSHandlers::OnToggleFullscreen);
			ZD_BIND_JS_HANDLER("isFullscreenMode", &JSHandlers::OnIsFullscreenMode);

			ZD_BIND_JS_HANDLER("spellCheck", &JSHandlers::OnSpellCheck);

			ZD_BIND_JS_HANDLER("getPreviewWidth", &JSHandlers::OnGetPreviewWidth);
			ZD_BIND_JS_HANDLER("setPreviewWidth", &JSHandlers::OnSetPreviewWidth);

			ZD_BIND_JS_HANDLER("isAutocloseBrackets", &JSHandlers::OnIsAutocloseBrackets);
			ZD_BIND_JS_HANDLER("setAutocloseBrackets", &JSHandlers::OnSetAutocloseBrackets);

			ZD_BIND_JS_HANDLER("getMimeType", &JSHandlers::OnGetMimeType);

			ZD_BIND_JS_HANDLER("getDatabaseList", &JSHandlers::OnGetDatabaseList);
			ZD_BIND_JS_HANDLER("getDatabaseListSize", &JSHandlers::OnGetDatabaseListSize);
			ZD_BIND_JS_HANDLER("addItemToDatabaseList", &JSHandlers::OnAddItemToDatabaseList);
			ZD_BIND_JS_HANDLER("removeItemFromDatabaseList", &JSHandlers::OnRemoveItemFromDatabaseList);
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::LoadMimeTypes()
		{
			JsonBox::Value root;
			root.loadFromFile(m_mimetypes_path);

			JsonBox::Value mimetypes = root["mimetypes"];
			if (mimetypes.isObject())
			{
				JsonBox::Object mt = mimetypes.getObject();
				for (auto &i : mt)
				{
					if (!i.second.isString())
					{
						continue;
					}

					std::string key = i.first;
					std::string value = i.second.getString();

					m_data_source.AddMimeType(key, value);
					m_js_handlers.AddMimeType(key, value);
				}
			}			
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::LoadLocalization()
		{
			std::vector<WIN32_FIND_DATA> files;
			ZD_STATUS status = Helpers::Files::GetFiles(m_locale_path, files, "*.json");

			if (ZD_SUCCEEDED(status))
			{
				for (auto &i : files)
				{
					std::stringstream fullname;
					fullname << m_locale_path << i.cFileName;

					Language lang;
					status = Helpers::Serialization::FromFile(lang, fullname.str());

					if (ZD_SUCCEEDED(status))
					{
						const std::string &lang_name(lang.GetLanguage());
						m_localization.AddLanguage(lang_name, std::make_shared<Language>(lang));
					}
				}
			}

			m_localization.SetCurrentLanguage(m_settings.GetLocaleSettings().GetLanguage());			

			m_data_source.SetLocalization(&m_localization);
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::InitializeCurrentDirectory()
		{
			char app_filename[MAX_PATH];
			GetModuleFileName(GetModuleHandle(nullptr), app_filename, MAX_PATH);

			std::string app_path = Helpers::String::ExtractPath(app_filename);

			SetCurrentDirectory(app_path.c_str());
			m_data_source.SetCurrentDirectory(app_path);
			return ZD_NOERROR;
		}

		ZD_STATUS ZenDiaryApp::InitializeDatabaseList()
		{
			std::vector<DatabaseListItem> &db_items = m_settings.GetDatabaseSettings().GetDatabaseListItems();

			for (auto &i : db_items)
			{
				m_db_list.AddItem(new DatabaseListItem(i.GetPath(), i.GetName()));
			}
			return ZD_NOERROR;
		}
	}
}