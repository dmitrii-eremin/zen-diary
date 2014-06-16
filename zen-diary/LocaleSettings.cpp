#include "stdafx.h"
#include "LocaleSettings.h"

namespace ZenDiary
{
	namespace App
	{
		LocaleSettings::LocaleSettings() :
			m_language("ru")
		{

		}

		LocaleSettings::~LocaleSettings()
		{

		}

		ZD_STATUS LocaleSettings::Serialize(JsonBox::Value &root) const
		{
			root["language"] = m_language;
			return ZD_NOERROR;
		}

		ZD_STATUS LocaleSettings::Deserialize(JsonBox::Value &root)
		{
			if (root["language"].isString())
			{
				m_language = root["language"].getString();
			}
			return ZD_NOERROR;
		}
	}
}