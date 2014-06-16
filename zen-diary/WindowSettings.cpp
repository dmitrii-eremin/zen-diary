#include "stdafx.h"
#include "WindowSettings.h"

namespace ZenDiary
{
	namespace App
	{
		WindowSettings::WindowSettings() :
			m_window_width(1280),
			m_window_height(720),
			m_window_title("Zen Diary")
		{

		}

		WindowSettings::~WindowSettings()
		{

		}

		ZD_STATUS WindowSettings::Serialize(JsonBox::Value &root) const
		{
			root["width"] = m_window_width;
			root["height"] = m_window_height;
			root["title"] = m_window_title;
			return ZD_NOERROR;
		}

		ZD_STATUS WindowSettings::Deserialize(JsonBox::Value &root)
		{
			if (root["width"].isInteger())
			{
				m_window_width = root["width"].getInt();
			}
			if (root["height"].isInteger())
			{
				m_window_height = root["height"].getInt();
			}
			if (root["title"].isString())
			{
				m_window_title = root["title"].getString();
			}
			return ZD_NOERROR;
		}
	}
}