#include "stdafx.h"
#include "SQLite.h"

namespace ZenDiary
{
	namespace App
	{
		const uint_t SQLiteDatabase::m_version = 1;

		SQLiteDatabase::SQLiteDatabase() :
			m_db(nullptr),
			m_db_stmt(nullptr),
			m_connected(false),
			m_columns_count(0)
		{

		}

		SQLiteDatabase::~SQLiteDatabase()
		{

		}

		bool SQLiteDatabase::Open(const std::string &path)
		{
			if (m_db && IsConnected())
			{
				Close();
			}

			m_connected = true;

			auto string_to_wstring = [](const std::string& in) -> std::wstring
			{
				std::locale loc("rus");
				std::wstring out;
				std::string::const_iterator i(in.begin()), ie(in.end());

				for (; i != ie; ++i)
					out += std::use_facet<std::ctype<wchar_t> >(loc).widen(*i);

				return out;
			};

			int result = sqlite3_open16(string_to_wstring(path.c_str()).c_str(), &m_db);
			m_last_error = static_cast<std::string>(sqlite3_errmsg(m_db));

			if (!result && m_last_error.find("not an error") == std::string::npos)
			{
				m_connected = false;
			}

			if (m_connected)
			{
				m_filename = path;
			}

			return m_connected;
		}

		void SQLiteDatabase::Close()
		{
			sqlite3_close(m_db);
			m_connected = false;
			m_filename.clear();
			m_db = nullptr;
		}

		void SQLiteDatabase::BeginTransaction()
		{
			if (!IsConnected())
			{
				return;
			}

			sqlite3_exec(m_db, "BEGIN TRANSACTION", nullptr, nullptr, nullptr);
		}

		void SQLiteDatabase::CommitTransaction()
		{
			if (!IsConnected())
			{
				return;
			}

			sqlite3_exec(m_db, "COMMIT TRANSACTION", nullptr, nullptr, nullptr);
		}

		void SQLiteDatabase::RollbackTransaction()
		{
			if (!IsConnected())
			{
				return;
			}

			sqlite3_exec(m_db, "ROLLBACK TRANSACTION", nullptr, nullptr, nullptr);
		}

		IDatabaseResult *SQLiteDatabase::ExecuteSelect(const std::string &query)
		{
			if (!IsConnected())
			{
				return nullptr;
			}

			m_db_mutex.lock();

			if (sqlite3_prepare_v2(m_db, query.c_str(), -1, &m_db_stmt, nullptr) != SQLITE_OK)
			{
				m_last_error = sqlite3_errmsg(m_db);
				sqlite3_finalize(m_db_stmt);
				m_db_stmt = nullptr;

				m_db_mutex.unlock();

				return nullptr;
			}
			else
			{
				m_columns_count = sqlite3_column_count(m_db_stmt);
				IDatabaseResult *result = this;
				return result;
			}
		}

		uint_t SQLiteDatabase::Execute(const std::string &query)
		{
			if (!IsConnected())
			{
				return 0;
			}

			char *err = "";
			if (sqlite3_exec(m_db, query.c_str(), nullptr, 0, &err) != SQLITE_OK)
			{
				m_last_error = sqlite3_errmsg(m_db);
				return 0;
			}

			return sqlite3_total_changes(m_db);
		}

		const std::string &SQLiteDatabase::GetLastError()
		{
			return m_last_error;
		}

		bool SQLiteDatabase::IsConnected()
		{
			return m_connected;
		}

		int SQLiteDatabase::GetLastInsertId() const
		{
			return static_cast<int>(sqlite3_last_insert_rowid(m_db));
		}

		const std::string &SQLiteDatabase::GetFilename() const
		{
			return m_filename;
		}

		size_t SQLiteDatabase::GetColumnCount()
		{
			return m_columns_count;
		}

		const char *SQLiteDatabase::NextColumnName(size_t num)
		{
			if (num > m_columns_count)
			{
				return "";
			}

			return sqlite3_column_name(m_db_stmt, num);
		}

		bool SQLiteDatabase::Next()
		{
			return sqlite3_step(m_db_stmt) == SQLITE_ROW;
		}

		const char *SQLiteDatabase::ColumnData(size_t n)
		{
			if (n > m_columns_count)
			{
				return "";
			}
			return (const char*)(sqlite3_column_text(m_db_stmt, n));
		}

		void SQLiteDatabase::Release()
		{
			sqlite3_finalize(m_db_stmt);
			m_columns_count = 0;
			m_db_stmt = nullptr;

			m_db_mutex.unlock();
		}
	}
}