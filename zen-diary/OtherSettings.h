#pragma once

namespace ZenDiary
{
	namespace App
	{
		class OtherSettings : public Interfaces::ISerializable
		{
		public:
			OtherSettings();
			~OtherSettings();

			OtherSettings(const OtherSettings &a) = delete;
			OtherSettings &operator = (const OtherSettings &a) = delete;

			virtual ZD_STATUS Serialize(JsonBox::Value &root) const override final;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) override final;

		private:
			ZD_PROPERTY(bool, m_use_javascript, UseJavascript);
			ZD_PROPERTY(bool, m_show_line_numbers, ShowLineNumbers);
		};
	};
};