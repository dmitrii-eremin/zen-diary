#include "stdafx.h"
#include "DatabaseUpdater.h"

namespace ZenDiary
{
	namespace App
	{
		DatabaseUpdater::DatabaseUpdater()
		{

		}

		DatabaseUpdater::~DatabaseUpdater()
		{

		}

		ZD_STATUS DatabaseUpdater::Update(SQLiteDatabase &db, JsonBox::Value &db_update)
		{
			int last_version = 0;
			std::set<DatabaseVersionUpdate> updates;
			ZD_STATUS status = ParseUpdateFiles(db_update, updates, last_version);

			if (ZD_FAILED(status))
			{
				return status;
			}

			int current_version = GetCurrentVersion(db);

			while (current_version <= last_version)
			{
				bool updated = false;

				for (auto &i : updates)
				{
					if (i.GetVersion() == current_version)
					{
						ApplyUpdate(db, i);
						updated = true;
						break;
					}
				}

				if (!updated)
				{
					break;
				}
				
				int new_current_version = GetCurrentVersion(db);
				if (new_current_version > current_version)
				{
					current_version = new_current_version;
				}
				else
				{
					break;
				}

			};

			return ZD_NOERROR;
		}

		int DatabaseUpdater::GetCurrentVersion(SQLiteDatabase &db)
		{
			int current_version = 0;
			IDatabaseResult *result = db.ExecuteSelect("SELECT `value` FROM `settings` WHERE `key` = 'version';");

			if (result)
			{
				if (result->Next())
				{
					const char *cvalue = result->ColumnData(0);
					if (cvalue)
					{
						current_version = atoi(cvalue);
					}
				}

				result->Release();
			}

			return current_version;
		}

		ZD_STATUS DatabaseUpdater::ParseUpdateFiles(JsonBox::Value &db_update, std::set<DatabaseVersionUpdate> &updates, int &last_version)
		{
			last_version = -1;

			if (!db_update["updates"].isArray())
			{
				return ZD_ERROR_INVALID_ARGUMENT;
			}

			JsonBox::Array js_updates = db_update["updates"].getArray();

			for (auto &i : js_updates)
			{
				if (!i["version"].isInteger() ||
					!i["file"].isString())
				{
					continue;
				}

				int version = i["version"].getInt();
				std::string filename = i["file"].getString();

				if (version > last_version)
				{
					last_version = version;
				}

				updates.insert(DatabaseVersionUpdate(version, filename));
			}
			return ZD_NOERROR;
		}

		ZD_STATUS DatabaseUpdater::ApplyUpdate(SQLiteDatabase &db, const DatabaseVersionUpdate &update)
		{
			std::string filedata;
			ZD_STATUS status = Helpers::Files::GetFileContent(update.GetFilename(), filedata);
			if (ZD_FAILED(status))
			{
				return status;
			}

			db.Execute(filedata);
			return ZD_NOERROR;
		}
	}
}