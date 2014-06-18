#pragma once

namespace ZenDiary
{
	namespace App
	{
		class EditorSettings : public Interfaces::ISerializable
		{
		public:
			EditorSettings();
			~EditorSettings();

			EditorSettings(const EditorSettings &a) = delete;
			EditorSettings &operator = (const EditorSettings &a) = delete;

			virtual ZD_STATUS Serialize(JsonBox::Value &root) const override final;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) override final;

		private:
			ZD_PROPERTY(bool, m_show_line_numbers, ShowLineNumbers);
			ZD_PROPERTY(int, m_preview_width, PreviewWidth);
			ZD_BOOL_PROPERTY(m_autoclose_brackets, AutocloseBrackets);
		};
	};
};