#pragma once

namespace ZenDiary
{
	namespace App
	{
		class AuthSettings : public Interfaces::ISerializable
		{
		public:
			AuthSettings();
			~AuthSettings();

			AuthSettings(const AuthSettings &a) = delete;
			AuthSettings &operator = (const AuthSettings &a) = delete;

			virtual ZD_STATUS Serialize(JsonBox::Value &root) const override final;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) override final;

		private:
			ZD_BOOL_PROPERTY(m_first_run, FirstRun);

			ZD_PROPERTY(std::string, m_login, Login);
			ZD_PROPERTY(std::string, m_passhash, Passhash);
			ZD_PROPERTY(std::string, m_salt, Salt);
		};
	};
};