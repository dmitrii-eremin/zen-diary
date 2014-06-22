#include "stdafx.h"
#include "OtherSettings.h"

namespace ZenDiary
{
	namespace App
	{
		OtherSettings::OtherSettings() : 
			m_use_javascript(false),
			m_signout_when_hide_to_tray(false)
		{

		}

		OtherSettings::~OtherSettings()
		{

		}

		ZD_STATUS OtherSettings::Serialize(JsonBox::Value &root) const
		{
			root["use_javascript"] = m_use_javascript;	
			root["signout_when_hide_to_tray"] = m_signout_when_hide_to_tray;
			return ZD_NOERROR;
		}

		ZD_STATUS OtherSettings::Deserialize(JsonBox::Value &root)
		{
			if (root["use_javascript"].isBoolean())
			{
				m_use_javascript = root["use_javascript"].getBoolean();
			}			
			if (root["signout_when_hide_to_tray"].isBoolean())
			{
				m_signout_when_hide_to_tray = root["signout_when_hide_to_tray"].getBoolean();
			}
			return ZD_NOERROR;
		}
	}
}