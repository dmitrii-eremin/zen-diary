#pragma once
#include "GlobalSettings.h"
#include "MethodHandler.h"
#include "ZenDataSource.h"
#include "CmdArguments.h"
#include "MenuHandler.h"
#include "JSHandlers.h"
#include "WebWindow.h"
#include "Updater.h"
#include "SQLite.h"

namespace ZenDiary
{
	namespace App
	{
		class ZenDiaryApp : public Interfaces::IApplication
		{
		public:
			ZenDiaryApp();
			virtual ~ZenDiaryApp();

			ZenDiaryApp(const ZenDiaryApp &a) = delete;
			ZenDiaryApp &operator = (const ZenDiaryApp &a) = delete;

			virtual ZD_STATUS Initialize(const std::string &argv) override final;
			virtual ZD_STATUS Deinitialize() override final;
			virtual ZD_STATUS Run() override final;

			virtual ZD_STATUS Terminate() override final;

		private:
			ZD_STATUS InitializeWindow();
			ZD_STATUS FreeWindow();

			ZD_STATUS InitializeCryptography();
			ZD_STATUS InitializeDirectories();
			ZD_STATUS InitializeJsHandlers();
			ZD_STATUS InitializeDatabase();
			ZD_STATUS BindMethods();

			ZD_STATUS LoadMimeTypes();
			
			ZD_STATUS InitializeCurrentDirectory();
			
		private:
			const std::string m_httpdocs_path = std::string("../media/httpdocs/");
			const std::string m_settings_path = std::string("../media/settings/settings.json");
			const std::string m_mimetypes_path = std::string("../media/settings/mime-types.json");

			const std::string m_database_path = std::string("../media/db/zen-diary.sqlite3");
			const std::string m_database_initialization_path = std::string("../media/db/initialization.sql");

			ZD_BOOL_PROPERTY(m_logged_in, LoggedIn);

			const ushort_t m_remote_debugging_port = 9922;

			std::atomic_bool m_terminate;

			CmdArguments m_args;

			Updater m_updater;			

			Awesomium::WebCore *m_core;
			Awesomium::WebSession *m_web_session;
			WebWindow *m_window;
			
			ZenDataSource m_data_source;

			MethodHandler m_method_handler;
			MenuHandler m_menu_handler;
			JSHandlers m_js_handlers;

			GlobalSettings m_settings;

			SQLiteDatabase m_database;
		};
	};
};
