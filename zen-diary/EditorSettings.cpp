#include "stdafx.h"
#include "EditorSettings.h"

namespace ZenDiary
{
	namespace App
	{
		EditorSettings::EditorSettings() :
			m_show_line_numbers(false),
			m_preview_width(800),
			m_autoclose_brackets(false)
		{

		}

		EditorSettings::~EditorSettings()
		{

		}

		ZD_STATUS EditorSettings::Serialize(JsonBox::Value &root) const
		{
			root["show_line_numbers"] = m_show_line_numbers;
			root["preview_width"] = m_preview_width;
			root["autoclose_brackets"] = m_autoclose_brackets;
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
			if (root["autoclose_brackets"].isBoolean())
			{
				m_autoclose_brackets = root["autoclose_brackets"].getBoolean();
			}
			return ZD_NOERROR;
		}
	}
}