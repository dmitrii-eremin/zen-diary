#include "stdafx.h"
#include "DatabaseSettings.h"

namespace ZenDiary
{
	namespace App
	{
		DatabaseSettings::DatabaseSettings() :
			m_path("../media/db/zen-diary.sqlite3")
		{

		}

		DatabaseSettings::~DatabaseSettings()
		{

		}

		ZD_STATUS DatabaseSettings::Serialize(JsonBox::Value &root) const
		{
			root["path"] = m_path;
			return ZD_NOERROR;
		}

		ZD_STATUS DatabaseSettings::Deserialize(JsonBox::Value &root)
		{
			if (root["path"].isString())
			{
				m_path = root["path"].getString();
			}
			return ZD_NOERROR;
		}
	}
}