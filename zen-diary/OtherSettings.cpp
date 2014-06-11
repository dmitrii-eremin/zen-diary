#include "stdafx.h"
#include "OtherSettings.h"

namespace ZenDiary
{
	namespace App
	{
		OtherSettings::OtherSettings() : 
			m_use_javascript(false),
			m_show_line_numbers(false)
		{

		}

		OtherSettings::~OtherSettings()
		{

		}

		ZD_STATUS OtherSettings::Serialize(JsonBox::Value &root) const
		{
			root["use_javascript"] = m_use_javascript;
			root["show_line_numbers"] = m_show_line_numbers;
			return ZD_NOERROR;
		}

		ZD_STATUS OtherSettings::Deserialize(JsonBox::Value &root)
		{
			if (root["use_javascript"].isBoolean())
			{
				m_use_javascript = root["use_javascript"].getBoolean();
			}
			if (root["show_line_numbers"].isBoolean())
			{
				m_show_line_numbers = root["show_line_numbers"].getBoolean();
			}
			return ZD_NOERROR;
		}
	}
}