#pragma once
#include "SQLite.h"
#include "DatabaseVersionUpdate.h"

namespace ZenDiary
{
	namespace App
	{
		class DatabaseUpdater
		{
		public:
			DatabaseUpdater();
			~DatabaseUpdater();

			DatabaseUpdater(const DatabaseUpdater &a) = delete;
			DatabaseUpdater &operator = (const DatabaseUpdater &a) = delete;

			ZD_STATUS Update(SQLiteDatabase &db, JsonBox::Value &db_update);

		private:
			int GetCurrentVersion(SQLiteDatabase &db);
			ZD_STATUS ParseUpdateFiles(JsonBox::Value &db_update, std::set<DatabaseVersionUpdate> &updates, int &last_version);
			ZD_STATUS ApplyUpdate(SQLiteDatabase &db, const DatabaseVersionUpdate &update);
		};
	};
};