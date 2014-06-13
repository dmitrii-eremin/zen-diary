#include "stdafx.h"
#include "SpellChecker.h"

namespace ZenDiary
{
	namespace App
	{
		SpellChecker::SpellChecker()
		{

		}

		SpellChecker::~SpellChecker()
		{

		}

		JsonBox::Array SpellChecker::Check(const std::string &text)
		{
			JsonBox::Value result;

			std::wstring wide_text = Helpers::String::strtowstr(text);
			std::string utf8_text = Helpers::String::ToUtf8(wide_text);

			std::string encoded_text = Helpers::String::UriEncode(utf8_text);

			std::stringstream url;
			url << m_spell_check_url << encoded_text;

			std::stringstream data;
			Helpers::Files::DownloadFile(url.str(), data);

			std::string answer = data.str();

			result.loadFromString(answer);

			return result.getArray();
		}
	}
}