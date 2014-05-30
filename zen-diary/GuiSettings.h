#pragma once
#include "WindowSettings.h"

namespace ZenDiary
{
	namespace App
	{
		class GuiSettings : public Interfaces::ISerializable
		{
		public:
			GuiSettings();
			~GuiSettings();

			GuiSettings(const GuiSettings &a) = delete;
			GuiSettings &operator = (const GuiSettings &a) = delete;

			virtual ZD_STATUS Serialize(JsonBox::Value &root) const override final;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) override final;

		private:
			ZD_PROPERTY_GETTER_BY_REF(WindowSettings, m_window_settings, WindowSettings);
		};
	};
};