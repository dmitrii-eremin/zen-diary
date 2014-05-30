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
			m_gui_settings.Serialize(root["gui"]);
			return ZD_NOERROR;
		}

		ZD_STATUS GlobalSettings::Deserialize(JsonBox::Value &root)
		{
			m_auth_settings.Deserialize(root["auth"]);
			m_gui_settings.Deserialize(root["gui"]);
			return ZD_NOERROR;
		}
	}
}