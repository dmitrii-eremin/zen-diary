#pragma once

namespace ZenDiary
{
	namespace App
	{
		struct IDatabaseResult
		{
			virtual size_t GetColumnCount() = 0;
			virtual const char *NextColumnName(size_t count) = 0;
			virtual bool Next() = 0;
			virtual const char *ColumnData(size_t n) = 0;
			virtual void Release() = 0;

			virtual ~IDatabaseResult() { }
		};

		class SQLiteDatabase : public IDatabaseResult
		{
		public:
			SQLiteDatabase();
			~SQLiteDatabase();

			SQLiteDatabase(const SQLiteDatabase &a) = delete;
			SQLiteDatabase &operator = (const SQLiteDatabase &a) = delete;

			bool Open(const std::string &path);
			void Close();

			void BeginTransaction();
			void CommitTransaction();
			void RollbackTransaction();

			IDatabaseResult *ExecuteSelect(const std::string &query);
			uint_t Execute(const std::string &query);

			const std::string &GetLastError();
			bool IsConnected();

			int GetLastInsertId() const;

			const std::string &GetFilename() const;

		private:
			virtual size_t GetColumnCount() override final;
			virtual const char *NextColumnName(size_t num) override final;
			virtual bool Next() override final;
			virtual const char *ColumnData(size_t n) override final;
			virtual void Release() override final;

		private:
			static const uint_t m_version;

			std::string m_filename;

			std::string m_last_error;

			sqlite3 *m_db;
			sqlite3_stmt *m_db_stmt;

			bool m_connected;

			std::mutex m_db_mutex;
			size_t m_columns_count;
		};
	}
}