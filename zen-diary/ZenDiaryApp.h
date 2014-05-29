#pragma once
#include "WebWindow.h"
#include "MethodHandler.h"
#include "MenuHandler.h"
#include "JSHandlers.h"

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
			
			ZD_STATUS BindMethods();

			std::string GetFullname(const std::string &path);
			
		private:
			const std::string m_httpdocs_path = std::string("../media/httpdocs/");

			std::atomic_bool m_terminate;

			Awesomium::WebCore *m_core;
			WebWindow *m_window;

			MethodHandler m_method_handler;
			MenuHandler m_menu_handler;
			JSHandlers m_js_handlers;
		};
	};
};