#include "stdafx.h"
#include "Localization.h"

namespace ZenDiary
{
	namespace App
	{
		Localization::Localization()
		{
			m_current_language.reset();
		}

		Localization::~Localization()
		{

		}

		bool Localization::IsLanguageAvailable(const std::string &lang) const
		{
			return m_languages.find(lang) != m_languages.end();
		}

		ZD_STATUS Localization::AddLanguage(const std::string &key, const std::shared_ptr<Language> &lang)
		{
			if (IsLanguageAvailable(key))
			{
				return ZD_ERROR_ALREADY_EXIST;
			}

			m_languages[key] = lang;
			return ZD_NOERROR;
		}

		ZD_STATUS Localization::SetCurrentLanguage(const std::string &lang)
		{
			auto i = m_languages.find(lang);
			if (i != m_languages.end())
			{
				m_current_language = i->second;
			}
			return ZD_NOERROR;
		}

		bool Localization::IsLanguageSelected() const
		{
			return m_current_language.get() != nullptr;
		}

		const std::string &Localization::Get(const std::string &key) const
		{
			if (!IsLanguageSelected())
			{
				static const std::string stub("/?/");
				return stub;
			}

			return m_current_language->Get(key);
		}

		std::string Localization::ApplyLocalization(std::string data)
		{
			static const std::string loc_begin("{{");
			static const std::string loc_end("}}");

			static const size_t loc_begin_length = loc_begin.length();
			static const size_t loc_end_length = loc_end.length();

			size_t index = std::string::npos;
			do
			{
				index = data.find(loc_begin);
				if (index == std::string::npos)
				{
					break;
				}


				size_t end_index = data.find(loc_end, index + 1);
				if (end_index == std::string::npos)
				{
					break;
				}

				std::string left = data.substr(0, index);
				std::string right = data.substr(end_index + loc_end_length);
				std::string key = data.substr(index + loc_begin_length, end_index - index - loc_begin_length);

				const std::string &word = this->Get(key);

				data = left + word + right;

			} while (index != std::string::npos);
			return data;
		}
	}
}