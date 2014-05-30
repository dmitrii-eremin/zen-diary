#include "stdafx.h"
#include "GuiSettings.h"

namespace ZenDiary
{
	namespace App
	{
		GuiSettings::GuiSettings()
		{

		}

		GuiSettings::~GuiSettings()
		{

		}

		ZD_STATUS GuiSettings::Serialize(JsonBox::Value &root) const
		{
			m_window_settings.Serialize(root["window"]);
			return ZD_NOERROR;
		}

		ZD_STATUS GuiSettings::Deserialize(JsonBox::Value &root)
		{
			m_window_settings.Deserialize(root["window"]);
			return ZD_NOERROR;
		}
	}
}