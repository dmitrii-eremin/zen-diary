#include "stdafx.h"
#include "AuthSettings.h"

namespace ZenDiary
{
	namespace App
	{
		AuthSettings::AuthSettings() : 
			m_first_run(true)
		{

		}

		AuthSettings::~AuthSettings()
		{

		}

		ZD_STATUS AuthSettings::Serialize(JsonBox::Value &root) const
		{
			root["first_run"] = m_first_run;
			root["login"] = m_login;
			root["passhash"] = m_passhash;
			root["salt"] = m_salt;
			return ZD_NOERROR;
		}

		ZD_STATUS AuthSettings::Deserialize(JsonBox::Value &root)
		{
			if (root["first_run"].isBoolean())
			{
				m_first_run = root["first_run"].getBoolean();
			}
			if (root["login"].isString())
			{
				m_login = root["login"].getString();
			}
			if (root["passhash"].isString())
			{
				m_passhash = root["passhash"].getString();
			}
			if (root["salt"].isString())
			{
				m_salt = root["salt"].getString();
			}
			return ZD_NOERROR;
		}
	}
}