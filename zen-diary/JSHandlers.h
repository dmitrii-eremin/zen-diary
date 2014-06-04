#pragma once
#include "GlobalSettings.h"
#include "Updater.h"
#include "SQLite.h"

#define ZD_BIND_JS_HANDLER(N, H)						m_method_handler.BindWithRetval(zen_diary, Awesomium::WSLit(N), JSDelegateWithRetval(&m_js_handlers, H))
#define ZD_BIND_JS_HANDLER_WO_RETVAL(N, H)				m_method_handler.Bind(zen_diary, Awesomium::WSLit(N), JSDelegate(&m_js_handlers, H));	\
	zen_diary.SetCustomMethod(Awesomium::WSLit(N), false);


namespace ZenDiary
{
	namespace App
	{
		class ZenDiaryApp;

		class JSHandlers
		{
		public:
			JSHandlers();
			~JSHandlers();

			JSHandlers(const JSHandlers &a) = default;
			JSHandlers &operator = (const JSHandlers &a) = default;	

			ZD_STATUS SetGlobalSettings(GlobalSettings *settings);
			ZD_STATUS SetZenApp(ZenDiaryApp *app);
			ZD_STATUS SetDatabase(SQLiteDatabase *db);
			ZD_STATUS SetUpdater(Updater *updater);

			/* Javascript function handlers */
			Awesomium::JSValue OnToInt(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnAlert(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnGetTemplate(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnGetVersionString(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnGetUsername(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnIsFirstRun(Awesomium::WebView *caller, const Awesomium::JSArray &args);

			Awesomium::JSValue OnRegisterUser(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnLoginUser(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnChangeCredits(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnLogoutUser(Awesomium::WebView *caller, const Awesomium::JSArray &args);

			Awesomium::JSValue OnPostNote(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnUpdateNote(Awesomium::WebView *caller, const Awesomium::JSArray &args);

			Awesomium::JSValue OnGetNote(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnGetNotes(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnIsNoteEncrypted(Awesomium::WebView *caller, const Awesomium::JSArray &args);

			Awesomium::JSValue OnHideNote(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnShowNote(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnDeleteNote(Awesomium::WebView *caller, const Awesomium::JSArray &args);

			Awesomium::JSValue OnOpenFileDialog(Awesomium::WebView *caller, const Awesomium::JSArray &args);
			Awesomium::JSValue OnSetClipboard(Awesomium::WebView *caller, const Awesomium::JSArray &args);

			Awesomium::JSValue OnGetUsersCount(Awesomium::WebView *caller, const Awesomium::JSArray &args);

		private:
			static Awesomium::JSObject CreateAnswerObject(bool success, const std::wstring &message = std::wstring());

		private:
			GlobalSettings *m_settings;
			ZenDiaryApp *m_zen_app;
			SQLiteDatabase *m_database;
			Updater *m_updater;
		};
	}
}