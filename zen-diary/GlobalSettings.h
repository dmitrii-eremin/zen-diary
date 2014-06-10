#pragma once
#include "DatabaseSettings.h"
#include "OtherSettings.h"
#include "AuthSettings.h"
#include "GuiSettings.h"

namespace ZenDiary
{
	namespace App
	{
		class GlobalSettings : public Interfaces::ISerializable
		{
		public:
			GlobalSettings();
			~GlobalSettings();

			GlobalSettings(const GlobalSettings &a) = delete;
			GlobalSettings &operator = (const GlobalSettings &a) = delete;

			virtual ZD_STATUS Serialize(JsonBox::Value &root) const override final;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) override final;

		private:
			ZD_PROPERTY_GETTER_BY_REF(AuthSettings, m_auth_settings, AuthSettings);
			ZD_PROPERTY_GETTER_BY_REF(OtherSettings, m_other_settings, OtherSettings);
			ZD_PROPERTY_GETTER_BY_REF(GuiSettings, m_gui_settings, GuiSettings);
			ZD_PROPERTY_GETTER_BY_REF(DatabaseSettings, m_database_settings, DatabaseSettings);
		};
	};
};