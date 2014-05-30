#pragma once

namespace ZenDiary
{
	namespace App
	{
		class WindowSettings : public Interfaces::ISerializable
		{
		public:
			WindowSettings();
			~WindowSettings();

			WindowSettings(const WindowSettings &a) = delete;
			WindowSettings &operator = (const WindowSettings &a) = delete;

			virtual ZD_STATUS Serialize(JsonBox::Value &root) const override final;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) override final;

		private:
			ZD_PROPERTY(int, m_window_width, Width);
			ZD_PROPERTY(int, m_window_height, Height);
			ZD_PROPERTY(std::string, m_window_title, Title);
		};
	};
};