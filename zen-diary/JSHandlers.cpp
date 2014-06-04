#include "stdafx.h"
#include "JSHandlers.h"
#include "ZenDiaryApp.h"

namespace ZenDiary
{
	namespace App
	{
		JSHandlers::JSHandlers() : 
			m_settings(nullptr),
			m_zen_app(nullptr),
			m_database(nullptr),
			m_updater(nullptr)
		{

		}

		JSHandlers::~JSHandlers()
		{

		}

		ZD_STATUS JSHandlers::SetGlobalSettings(GlobalSettings *settings)
		{
			m_settings = settings;
			return ZD_NOERROR;
		}

		ZD_STATUS JSHandlers::SetZenApp(ZenDiaryApp *app)
		{
			m_zen_app = app;
			return ZD_NOERROR;
		}

		ZD_STATUS JSHandlers::SetDatabase(SQLiteDatabase *db)
		{
			m_database = db;
			return ZD_NOERROR;
		}

		ZD_STATUS JSHandlers::SetUpdater(Updater *updater)
		{
			m_updater = updater;
			return ZD_NOERROR;
		}

		Awesomium::JSValue JSHandlers::OnToInt(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (args.size() == 0)
			{
				return Awesomium::JSValue::Undefined();
			}

			Awesomium::JSValue arg(args.At(0));
			if (!arg.IsString())
			{
				return Awesomium::JSValue::Undefined();
			}

			std::string s = Awesomium::ToString(arg.ToString());
			return Awesomium::JSValue(atoi(s.c_str()));
		}

		Awesomium::JSValue JSHandlers::OnAlert(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (args.size() < 1)
			{
				return Awesomium::JSValue::Undefined();
			}

			uint_t size = args.size();

			std::stringstream message;
			for (uint_t i = 0; i < size; i++)
			{
				Awesomium::JSValue val = args.At(i);
				if (val.IsString())
				{
					message << Awesomium::ToString(val.ToString());
				}
				else if (val.IsInteger())
				{
					message << val.ToInteger();
				}
				else if (val.IsDouble())
				{
					message << val.ToDouble();
				}
			}

			MessageBox(nullptr, message.str().c_str(), "Zen Diary | ��� ������ �������", MB_OK);

			return Awesomium::JSValue::Undefined();
		}

		Awesomium::JSValue JSHandlers::OnGetTemplate(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			Awesomium::JSValue result(Awesomium::WSLit(""));

			if (args.size() == 0)
			{
				return result;
			}

			Awesomium::JSValue js_filename = args.At(0);
			if (!js_filename.IsString())
			{
				return result;
			}

			std::string filename = Awesomium::ToString(js_filename.ToString());

			if (!Helpers::Files::IsFileExist(filename))
			{
				return result;
			}

			std::string data;

			ZD_STATUS status = Helpers::Files::GetFileContent(filename, data);
			if (ZD_FAILED(status))
			{
				return result;
			}

			result = Awesomium::JSValue(Awesomium::WSLit(data.c_str()));

			return result;
		}

		Awesomium::JSValue JSHandlers::OnGetVersionString(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			std::stringstream stream;
			stream << ZD_VERSION_HI(ZD_VERSION) << "." << ZD_VERSION_MID(ZD_VERSION) << "." << ZD_VERSION_LOW(ZD_VERSION) << " " << ZD_VERSION_STATUS;
			
			return Awesomium::JSValue(Awesomium::WSLit(stream.str().c_str()));
		}

		Awesomium::JSValue JSHandlers::OnGetUsername(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_settings)
			{
				return Awesomium::JSValue::Undefined();
			}

			const AuthSettings &auth = m_settings->GetAuthSettings();

			const std::string &login = auth.GetLogin();

			return Awesomium::JSValue(Awesomium::WSLit(login.c_str()));
		}

		Awesomium::JSValue JSHandlers::OnIsFirstRun(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			bool first_run = true;

			if (m_settings)
			{
				const AuthSettings &auth_settings = m_settings->GetAuthSettings();
				first_run = auth_settings.IsFirstRun();
			}

			return Awesomium::JSValue(first_run);
		}

		Awesomium::JSValue JSHandlers::OnRegisterUser(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_settings)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}

			AuthSettings &auth = m_settings->GetAuthSettings();

			if (!auth.IsFirstRun() || auth.GetLogin().length() > 0 || auth.GetPasshash().length() > 0)
			{
				return CreateAnswerObject(false, L"��������� ������ ������������� ��������, ���������� � ������������.");
			}

			if (args.size() < 2)
			{
				return CreateAnswerObject(false, L"����������� ����������. ������� �������� ������������ ����������, ���������� � ������������.");
			}

			Awesomium::JSValue js_login = args.At(0);
			Awesomium::JSValue js_password = args.At(1);

			if (!js_login.IsString() || !js_password.IsString())
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ��������� ����, ���������� � ������������.");
			}					

			std::string login(Awesomium::ToString(js_login.ToString()));
			std::string password(Awesomium::ToString(js_password.ToString()));

			std::string salt(Helpers::String::GenerateString());
			std::string passhash(Helpers::Crypto::md5(salt + password));				

			auth.SetLogin(login);
			auth.SetPasshash(passhash);
			auth.SetSalt(salt);
			auth.SetFirstRun(false);

			ZD_SAFE_CALL(m_zen_app)->SetLoggedIn(true);

			return CreateAnswerObject(true);
		}

		Awesomium::JSValue JSHandlers::OnLoginUser(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_settings)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}

			AuthSettings &auth = m_settings->GetAuthSettings();

			if (args.size() < 2)
			{
				return CreateAnswerObject(false, L"����������� ����������. ������� �������� ������������ ����������, ���������� � ������������.");
			}

			Awesomium::JSValue js_login = args.At(0);
			Awesomium::JSValue js_password = args.At(1);

			if (!js_login.IsString() || !js_password.IsString())
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ��������� ����, ���������� � ������������.");
			}

			std::string login(Awesomium::ToString(js_login.ToString()));
			std::string password(Awesomium::ToString(js_password.ToString()));

			std::string saved_login(auth.GetLogin());
			std::string saved_passhash(auth.GetPasshash());
			std::string salt(auth.GetSalt());

			std::string passhash(Helpers::Crypto::md5(salt + password));

			if (passhash != saved_passhash || login != saved_login)
			{
				return CreateAnswerObject(false, L"�������� ��� ������������ ��� ������.");
			}

			ZD_SAFE_CALL(m_zen_app)->SetLoggedIn(true);

			return CreateAnswerObject(true);
		}

		Awesomium::JSValue JSHandlers::OnChangeCredits(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_settings)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}			

			AuthSettings &auth = m_settings->GetAuthSettings();

			if (args.size() < 2)
			{
				return CreateAnswerObject(false, L"��������� �������� ����������. ������� �������� ������������ ����������, ���������� � ������������.");
			}

			Awesomium::JSValue js_login = args.At(0);
			Awesomium::JSValue js_password = args.At(1);

			if (!js_login.IsString() || !js_password.IsString())
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ��������� ����, ���������� � ������������.");
			}

			std::string login(Awesomium::ToString(js_login.ToString()));
			std::string password(Awesomium::ToString(js_password.ToString()));

			std::string salt(Helpers::String::GenerateString());
			std::string passhash(Helpers::Crypto::md5(salt + password));

			auth.SetLogin(login);
			auth.SetPasshash(passhash);
			auth.SetSalt(salt);

			ZD_SAFE_CALL(m_zen_app)->SetLoggedIn(true);

			return CreateAnswerObject(true);
		}

		Awesomium::JSValue JSHandlers::OnLogoutUser(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			ZD_SAFE_CALL(m_zen_app)->SetLoggedIn(false);

			return CreateAnswerObject(true);
		}

		Awesomium::JSValue JSHandlers::OnPostNote(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_database)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}

			if (args.size() < 2)
			{
				return CreateAnswerObject(false, L"������� �������� ������������ ����������, ���������� � ������������.");
			}

			Awesomium::JSValue js_title = args.At(0);
			Awesomium::JSValue js_text = args.At(1);

			Awesomium::JSValue js_password(Awesomium::WSLit(""));
			if (args.size() >= 3)
			{
				js_password = args.At(2);
			}

			if (!js_title.IsString() || !js_text.IsString() ||
				(!js_password.IsString() && !js_password.IsUndefined()))
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ��������� ����, ���������� � ������������.");
			}

			std::string title(Awesomium::ToString(js_title.ToString()));
			std::string text(Awesomium::ToString(js_text.ToString()));

			std::string password;
			if (js_password.IsString())
			{
				password = Awesomium::ToString(js_password.ToString());
			}

			bool use_password = password.length() > 0;

			std::string hash = Helpers::Crypto::md5(text);

			if (use_password)
			{
				char *encrypted_data = nullptr;
				size_t encrypted_data_size = 0;

				Helpers::Crypto::EncryptString(text, password, &encrypted_data, encrypted_data_size);

				text = Helpers::Crypto::Base64Encode(encrypted_data, encrypted_data_size);

				delete []encrypted_data;
			}

			std::stringstream query;
			query << "INSERT INTO `notes` (`title`, `note`, `hash`, `encrypted`, `created`, `updated`) VALUES('" << 
				title << "', '" << text << "', '" << hash << "', " << (use_password ? 1 : 0) << ", datetime('now', 'localtime'), datetime('now', 'localtime'));";

			uint_t inserted = m_database->Execute(query.str());

			if (inserted == 0)
			{
				return CreateAnswerObject(false, L"�� ������� �������� ������ � ��, �������� ������ � ���� ������, ���������� � ������������.");
			}

			Awesomium::JSObject answer(CreateAnswerObject(true));

			int last_row_id = m_database->GetLastInsertId();

			answer.SetProperty(Awesomium::WSLit("id"), Awesomium::JSValue(last_row_id));

			return answer;
		}

		Awesomium::JSValue JSHandlers::OnUpdateNote(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_database)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}

			if (args.size() < 3)
			{
				return CreateAnswerObject(false, L"������� �������� ������������ ����������, ���������� � ������������.");
			}

			Awesomium::JSValue js_id = args.At(0);
			Awesomium::JSValue js_title = args.At(1);
			Awesomium::JSValue js_text = args.At(2);

			Awesomium::JSValue js_password(Awesomium::WSLit(""));
			if (args.size() >= 4)
			{
				js_password = args.At(3);
			}

			if (!js_id.IsInteger() || !js_title.IsString() || !js_text.IsString() ||
				(!js_password.IsString() && !js_password.IsUndefined()))
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ��������� ����, ���������� � ������������.");
			}

			int id = js_id.ToInteger();
			std::string title(Awesomium::ToString(js_title.ToString()));
			std::string text(Awesomium::ToString(js_text.ToString()));

			std::string password;
			if (js_password.IsString())
			{
				password = Awesomium::ToString(js_password.ToString());
			}

			bool use_password = password.length() > 0;

			std::string hash = Helpers::Crypto::md5(text);

			if (use_password)
			{
				char *encrypted_data = nullptr;
				size_t encrypted_data_size = 0;

				Helpers::Crypto::EncryptString(text, password, &encrypted_data, encrypted_data_size);

				text = Helpers::Crypto::Base64Encode(encrypted_data, encrypted_data_size);

				delete[]encrypted_data;
			}

			std::stringstream query;

			query << "UPDATE `notes` SET `title` = '" << title << "', `note` = '" << text
				<< "', `hash` = '" << hash << "', `encrypted` = " << (use_password ? 1 : 0) << ", `updated` = datetime('now', 'localtime') WHERE `id` = " << id << ";";;

			uint_t updated = m_database->Execute(query.str());

			if (updated == 0)
			{
				return CreateAnswerObject(false, L"�� ������� �������� ������ � ��, �������� ������ � ���� ������, ���������� � ������������.");
			}

			Awesomium::JSObject answer(CreateAnswerObject(true));

			int last_row_id = m_database->GetLastInsertId();

			answer.SetProperty(Awesomium::WSLit("id"), Awesomium::JSValue(last_row_id));

			return answer;
		}

		Awesomium::JSValue JSHandlers::OnGetNote(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (args.size() < 1)
			{
				return CreateAnswerObject(false, L"������� �������� �� ��� ���������, ���������� � ������������.");
			}

			if (!m_database)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}

			Awesomium::JSValue js_id(args.At(0));

			bool use_password = false;

			Awesomium::JSValue js_password;
			if (args.size() >= 2)
			{
				js_password = args.At(1);

				use_password = true;
			}

			if ((!js_id.IsInteger() && !js_id.IsString()) ||
				(args.size() >= 2 && !js_password.IsString()))
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ������������� ����, ���������� � ������������.");
			}

			int id = 0;
			if (js_id.IsInteger())
			{
				id = js_id.ToInteger();
			}
			else if (js_id.IsString())
			{
				id = atoi(Awesomium::ToString(js_id.ToString()).c_str());
			}

			std::stringstream query;
			query << "SELECT `title`, `note`, `hash`, `encrypted`, `created`, `updated`, `hidden` FROM `notes` WHERE `id` = " << id << " AND `deleted` = 0;";

			IDatabaseResult *stmt = m_database->ExecuteSelect(query.str());

			const wchar_t *error_not_exist = L"�� ������� �������� ������� �� ���� ������, �������� ��� ���� �������.";

			if (!stmt || !stmt->Next())
			{
				ZD_SAFE_CALL(stmt)->Release();
				return CreateAnswerObject(false, error_not_exist);
			}

			const char *ctitle = stmt->ColumnData(0);
			const char *cnote = stmt->ColumnData(1);
			const char *chash = stmt->ColumnData(2);
			const char *cencrypted = stmt->ColumnData(3);
			const char *ccreated = stmt->ColumnData(4);
			const char *cupdated = stmt->ColumnData(5);
			const char *chidden = stmt->ColumnData(6);

			if (!ctitle || !cnote || !chash || !cencrypted || 
				!ccreated || !cupdated || !chidden)
			{
				ZD_SAFE_CALL(stmt)->Release();
				return CreateAnswerObject(false, error_not_exist);
			}

			std::string title(ctitle);
			std::string note(cnote);
			std::string hash(chash);
			bool encrypted = atoi(cencrypted) > 0;
			bool hidden = atoi(chidden) > 0;
			std::string created(ccreated);
			std::string updated(cupdated);			
			
			ZD_SAFE_CALL(stmt)->Release();

			if (encrypted && !use_password)
			{
				return CreateAnswerObject(false, L"���������� ������� ������������� ������� ��� ������.");
			}

			Awesomium::JSObject answer = CreateAnswerObject(true);

			answer.SetProperty(Awesomium::WSLit("id"), Awesomium::JSValue(id));
			answer.SetProperty(Awesomium::WSLit("title"), Awesomium::JSValue(Awesomium::WSLit(title.c_str())));
			answer.SetProperty(Awesomium::WSLit("hash"), Awesomium::JSValue(Awesomium::WSLit(hash.c_str())));
			answer.SetProperty(Awesomium::WSLit("encrypted"), Awesomium::JSValue(encrypted));
			answer.SetProperty(Awesomium::WSLit("hidden"), Awesomium::JSValue(hidden));
			answer.SetProperty(Awesomium::WSLit("created"), Awesomium::JSValue(Awesomium::WSLit(created.c_str())));
			answer.SetProperty(Awesomium::WSLit("updated"), Awesomium::JSValue(Awesomium::WSLit(updated.c_str())));	

			if (encrypted)
			{
				std::string password(Awesomium::ToString(js_password.ToString()));

				char *encoded_bytes = nullptr;
				size_t encoded_size = 0;

				Helpers::Crypto::Base64Decode(note, &encoded_bytes, encoded_size);
				std::string decoded_note = Helpers::Crypto::DecryptString(encoded_bytes, encoded_size, password);

				delete []encoded_bytes;
				encoded_bytes = nullptr;

				std::string decoded_note_hash = Helpers::Crypto::md5(decoded_note);
				if (decoded_note_hash != hash)
				{					
					return CreateAnswerObject(false, L"�� ������� ������������ �������, �� ����� �������� ������.");
				}

				answer.SetProperty(Awesomium::WSLit("note"), Awesomium::JSValue(Awesomium::WSLit(decoded_note.c_str())));
			}
			else
			{
				answer.SetProperty(Awesomium::WSLit("note"), Awesomium::JSValue(Awesomium::WSLit(note.c_str())));
			}

			return answer;
		}

		Awesomium::JSValue JSHandlers::OnGetNotes(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_database)
			{
				return Awesomium::JSValue::Undefined();
			}			

			Awesomium::JSArray result;

			const size_t args_count = args.size();
			Awesomium::JSValue js_day;
			Awesomium::JSValue js_month;
			Awesomium::JSValue js_year;

			bool use_day_filter = false;

			SYSTEMTIME day = { 0 };

			if (args_count == 3)
			{
				js_day = args.At(0);
				js_month = args.At(1);
				js_year = args.At(2);

				if (js_day.IsInteger() && js_month.IsInteger() && js_year.IsInteger())
				{
					use_day_filter = true;
					day.wDay = js_day.ToInteger();
					day.wMonth = js_month.ToInteger();
					day.wYear = js_year.ToInteger();
				}
			}

			std::stringstream query;
			if (!use_day_filter)
			{
				query << "SELECT `id`, `title`, `encrypted`, `created`, `updated`, `note`, `hash`, `hidden` FROM `notes` WHERE `deleted` = 0;";
			}
			else
			{
				SYSTEMTIME from_time(day);

				from_time.wHour = 0;
				from_time.wMinute = 0;
				from_time.wSecond = 0;

				SYSTEMTIME to_time(day);

				to_time.wHour = 23;
				to_time.wMinute = 59;
				to_time.wSecond = 59;

				std::string from = Helpers::String::FormatTime(from_time);
				std::string to = Helpers::String::FormatTime(to_time);

				query << "SELECT `id`, `title`, `encrypted`, `created`, `updated`, `note`, `hash`, `hidden` FROM `notes` WHERE `deleted` = 0 AND `updated` > '" <<
					from << "' AND `updated` < '" << to << "';";
			}
			

			IDatabaseResult *stmt = m_database->ExecuteSelect(query.str());
			while (stmt && stmt->Next())
			{
				const char *cid = stmt->ColumnData(0);
				const char *ctitle = stmt->ColumnData(1);
				const char *cencrypted = stmt->ColumnData(2);
				const char *ccreated = stmt->ColumnData(3);
				const char *cupdated = stmt->ColumnData(4);
				const char *cnote = stmt->ColumnData(5);
				const char *chash = stmt->ColumnData(6);
				const char *chidden = stmt->ColumnData(7);

				if (!cid || !ctitle || !cencrypted || 
					!ccreated || !cupdated || !cnote || 
					!chash || !chidden)
				{
					continue;
				}

				int id = atoi(cid);
				bool encrypted = atoi(cencrypted) != 0;
				bool hidden = atoi(chidden) != 0;
				
				std::string title(ctitle);				
				std::string created(ccreated);
				std::string updated(cupdated);

				std::string note(cnote);
				std::string hash(chash);

				Awesomium::JSObject item;
				item.SetProperty(Awesomium::WSLit("id"), Awesomium::JSValue(id));
				item.SetProperty(Awesomium::WSLit("encrypted"), Awesomium::JSValue(encrypted));
				item.SetProperty(Awesomium::WSLit("hidden"), Awesomium::JSValue(hidden));
				item.SetProperty(Awesomium::WSLit("title"), Awesomium::JSValue(Awesomium::WSLit(title.c_str())));
				item.SetProperty(Awesomium::WSLit("note"), Awesomium::JSValue(Awesomium::WSLit(note.c_str())));
				item.SetProperty(Awesomium::WSLit("hash"), Awesomium::JSValue(Awesomium::WSLit(hash.c_str())));
				item.SetProperty(Awesomium::WSLit("created"), Awesomium::JSValue(Awesomium::WSLit(created.c_str())));
				item.SetProperty(Awesomium::WSLit("updated"), Awesomium::JSValue(Awesomium::WSLit(updated.c_str())));

				result.Push(item);
			}

			ZD_SAFE_CALL(stmt)->Release();

			return result;
		}

		Awesomium::JSValue JSHandlers::OnIsNoteEncrypted(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_database || args.size() < 1)
			{
				return Awesomium::JSValue(false);
			}

			Awesomium::JSValue js_id(args.At(0));

			if (!js_id.IsInteger() && !js_id.IsString())
			{
				return Awesomium::JSValue(false);
			}

			int id = 0;
			if (js_id.IsInteger())
			{
				id = js_id.ToInteger();
			}
			else if (js_id.IsString())
			{
				id = atoi(Awesomium::ToString(js_id.ToString()).c_str());
			}

			std::stringstream query;
			query << "SELECT `encrypted` FROM `notes` WHERE `deleted` = 0 AND `id` = " << id << ";";

			IDatabaseResult *stmt = m_database->ExecuteSelect(query.str());
			if (!stmt)
			{
				return Awesomium::JSValue::Undefined();
			}

			bool encrypted = false;

			if (stmt->Next())
			{
				const char *cencrypted = stmt->ColumnData(0);
				if (cencrypted)
				{
					int enc = atoi(cencrypted);

					encrypted = enc > 0;
				}
			}

			stmt->Release();

			return Awesomium::JSValue(encrypted);
		}

		Awesomium::JSValue JSHandlers::OnHideNote(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (args.size() < 1)
			{
				return CreateAnswerObject(false, L"������� �������� �� ��� ���������, ���������� � ������������.");
			}

			if (!m_database)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}

			Awesomium::JSValue js_id(args.At(0));

			bool use_password = false;

			Awesomium::JSValue js_password;
			if (args.size() >= 2)
			{
				js_password = args.At(1);

				use_password = true;
			}

			if (!js_id.IsInteger() && !js_id.IsString())
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ������������� ����, ���������� � ������������.");
			}

			int id = 0;
			if (js_id.IsInteger())
			{
				id = js_id.ToInteger();
			}
			else if (js_id.IsString())
			{
				id = atoi(Awesomium::ToString(js_id.ToString()).c_str());
			}

			std::stringstream query;
			query << "UPDATE `notes` SET `hidden` = 1 WHERE `id` = " << id << ";";

			int updated = m_database->Execute(query.str());

			if (updated == 0)
			{
				return CreateAnswerObject(false, L"�� ������� ������ �������, �������� ID �������.");
			}

			return CreateAnswerObject(true);
		}

		Awesomium::JSValue JSHandlers::OnShowNote(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (args.size() < 1)
			{
				return CreateAnswerObject(false, L"������� �������� �� ��� ���������, ���������� � ������������.");
			}

			if (!m_database)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}

			Awesomium::JSValue js_id(args.At(0));

			bool use_password = false;

			Awesomium::JSValue js_password;
			if (args.size() >= 2)
			{
				js_password = args.At(1);

				use_password = true;
			}

			if (!js_id.IsInteger() && !js_id.IsString())
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ������������� ����, ���������� � ������������.");
			}

			int id = 0;
			if (js_id.IsInteger())
			{
				id = js_id.ToInteger();
			}
			else if (js_id.IsString())
			{
				id = atoi(Awesomium::ToString(js_id.ToString()).c_str());
			}

			std::stringstream query;
			query << "UPDATE `notes` SET `hidden` = 0 WHERE `id` = " << id << ";";

			int updated = m_database->Execute(query.str());

			if (updated == 0)
			{
				return CreateAnswerObject(false, L"�� ������� ������ �������, �������� ID �������.");
			}

			return CreateAnswerObject(true);
		}

		Awesomium::JSValue JSHandlers::OnDeleteNote(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (args.size() < 1)
			{
				return CreateAnswerObject(false, L"������� �������� �� ��� ���������, ���������� � ������������.");
			}

			if (!m_database)
			{
				return CreateAnswerObject(false, L"���������� javascript ��������� �� ���������������, ���������� � ������������.");
			}

			Awesomium::JSValue js_id(args.At(0));

			bool use_password = false;

			Awesomium::JSValue js_password;
			if (args.size() >= 2)
			{
				js_password = args.At(1);

				use_password = true;
			}

			if (!js_id.IsInteger() && !js_id.IsString())
			{
				return CreateAnswerObject(false, L"������� �������� ��������� ������������� ����, ���������� � ������������.");
			}

			int id = 0;
			if (js_id.IsInteger())
			{
				id = js_id.ToInteger();
			}
			else if (js_id.IsString())
			{
				id = atoi(Awesomium::ToString(js_id.ToString()).c_str());
			}

			std::stringstream query;
			query << "DELETE FROM `notes` WHERE `id` = " << id << ";";

			int updated = m_database->Execute(query.str());

			if (updated == 0)
			{
				return CreateAnswerObject(false, L"�� ������� ������� �������, �������� ID �������.");
			}

			return CreateAnswerObject(true);
		}

		Awesomium::JSValue JSHandlers::OnOpenFileDialog(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			wchar_t filename[MAX_PATH] = { 0 };

			OPENFILENAMEW ofn = { 0 };

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = GetDesktopWindow();
			ofn.lpstrFilter = L"��� ����� (*.*)\0*.*\0";
			ofn.lpstrFile = filename;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST;
			ofn.lpstrDefExt = L"";

			if (GetOpenFileNameW(&ofn) == FALSE)
			{
				return Awesomium::JSValue::Undefined();
			}
			
			return Awesomium::JSValue(Awesomium::WSLit(Helpers::String::ToUtf8(filename).c_str()));
		}

		Awesomium::JSObject JSHandlers::CreateAnswerObject(bool success, const std::wstring &message)
		{
			Awesomium::JSObject result;

			result.SetProperty(Awesomium::WSLit("success"), Awesomium::JSValue(success));
			result.SetProperty(Awesomium::WSLit("message"), Awesomium::JSValue(Awesomium::WSLit(Helpers::String::ToUtf8(message).c_str())));

			return result;
		}

		Awesomium::JSValue JSHandlers::OnSetClipboard(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (args.size() < 1)
			{
				return Awesomium::JSValue::Undefined();
			}

			Awesomium::JSValue arg(args.At(0));
			if (arg.IsString())
			{
				std::string str = Awesomium::ToString(arg.ToString());

				Helpers::Win32::SetClipboardText(str);
			}

			return Awesomium::JSValue::Undefined();
		}

		Awesomium::JSValue JSHandlers::OnGetUsersCount(Awesomium::WebView *caller, const Awesomium::JSArray &args)
		{
			if (!m_updater || m_updater->GetUsersCount() == 0)
			{
				return Awesomium::JSValue::Undefined();
			}

			return Awesomium::JSValue(m_updater->GetUsersCount());
		}
	}
}