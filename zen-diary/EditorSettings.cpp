#include "stdafx.h"
#include "EditorSettings.h"

namespace ZenDiary
{
	namespace App
	{
		EditorSettings::EditorSettings() :
			m_show_line_numbers(false),
			m_preview_width(800)
		{

		}

		EditorSettings::~EditorSettings()
		{

		}

		ZD_STATUS EditorSettings::Serialize(JsonBox::Value &root) const
		{
			root["show_line_numbers"] = m_show_line_numbers;
			root["preview_width"] = m_preview_width;
			return ZD_NOERROR;
		}

		ZD_STATUS EditorSettings::Deserialize(JsonBox::Value &root)
		{
			if (root["show_line_numbers"].isBoolean())
			{
				m_show_line_numbers = root["show_line_numbers"].getBoolean();
			}
			if (root["preview_width"].isInteger())
			{
				m_preview_width = root["preview_width"].getInt();
			}
			return ZD_NOERROR;
		}
	}
}