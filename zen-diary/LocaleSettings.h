#pragma once

namespace ZenDiary
{
	namespace App
	{
		class LocaleSettings : public Interfaces::ISerializable
		{
		public:
			LocaleSettings();
			~LocaleSettings();

			LocaleSettings(const LocaleSettings &a) = delete;
			LocaleSettings &operator = (const LocaleSettings &a) = delete;

			virtual ZD_STATUS Serialize(JsonBox::Value &root) const override final;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) override final;

		private:
			ZD_PROPERTY(std::string, m_language, Language);
		};
	};
};