#include "stdafx.h"
#include "DatabaseSettings.h"

namespace ZenDiary
{
	namespace App
	{
		DatabaseSettings::DatabaseSettings() :
			m_path("../media/db/zen-diary.sqlite3")
		{
			m_db_items.push_back(DatabaseListItem(m_path, "zen-diary.sqlite3"));
		}

		DatabaseSettings::~DatabaseSettings()
		{

		}

		ZD_STATUS DatabaseSettings::Serialize(JsonBox::Value &root) const
		{		
			JsonBox::Array db;
			for (auto &i : m_db_items)
			{
				JsonBox::Value item;
				item["path"] = i.GetPath();
				item["name"] = i.GetName();

				db.push_back(item);
			}

			root["db"] = db;
			root["path"] = m_path;
			return ZD_NOERROR;
		}

		ZD_STATUS DatabaseSettings::Deserialize(JsonBox::Value &root)
		{			
			if (root["path"].isString())
			{
				m_path = root["path"].getString();
			}

			if (root["db"].isArray())
			{
				JsonBox::Array db = root["db"].getArray();
				for (auto &i : db)
				{
					if (i["path"].isString() && i["name"].isString())
					{
						std::string path = i["path"].getString();
						std::string name = i["name"].getString();

						if (Helpers::Files::IsFileExist(path))
						{
							m_db_items.push_back(DatabaseListItem(path, name));
						}
					}
				}
			}
			return ZD_NOERROR;
		}

		ZD_STATUS DatabaseSettings::ClearDbItems()
		{
			m_db_items.clear();
			return ZD_NOERROR;
		}

		ZD_STATUS DatabaseSettings::AddDbItem(const DatabaseListItem &item)
		{
			m_db_items.push_back(item);
			return ZD_NOERROR;
		}

		std::vector<DatabaseListItem> &DatabaseSettings::GetDatabaseListItems()
		{
			return m_db_items;
		}
	}
}