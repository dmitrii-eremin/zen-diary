#include "stdafx.h"
#include "GlobalSettings.h"

namespace ZenDiary
{
	namespace App
	{
		GlobalSettings::GlobalSettings()
		{

		}

		GlobalSettings::~GlobalSettings()
		{

		}

		ZD_STATUS GlobalSettings::Serialize(JsonBox::Value &root) const
		{
			m_auth_settings.Serialize(root["auth"]);
			m_other_settings.Serialize(root["other"]);
			m_locale_settings.Serialize(root["locale"]);
			m_gui_settings.Serialize(root["gui"]);
			m_database_settings.Serialize(root["database"]);
			m_editor_settings.Serialize(root["editor"]);
			return ZD_NOERROR;
		}

		ZD_STATUS GlobalSettings::Deserialize(JsonBox::Value &root)
		{
			m_auth_settings.Deserialize(root["auth"]);
			m_other_settings.Deserialize(root["other"]);
			m_locale_settings.Deserialize(root["locale"]);
			m_gui_settings.Deserialize(root["gui"]);
			m_database_settings.Deserialize(root["database"]);
			m_editor_settings.Deserialize(root["editor"]);
			return ZD_NOERROR;
		}
	}
}