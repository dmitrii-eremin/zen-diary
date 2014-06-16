#pragma once
#include "Language.h"

namespace ZenDiary
{
	namespace App
	{
		class Localization
		{
		public:
			Localization();
			~Localization();

			Localization(const Localization &a) = delete;
			Localization &operator = (const Localization &a) = delete;

			bool IsLanguageAvailable(const std::string &lang) const;
			ZD_STATUS AddLanguage(const std::string &key, const std::shared_ptr<Language> &lang);

			const std::string GetDefaultLanguage() const;

			ZD_STATUS SetCurrentLanguage(const std::string &lang);
			bool IsLanguageSelected() const;

			const std::string &Get(const std::string &key) const;

			std::string ApplyLocalization(std::string data);

		private:
			std::map<std::string, std::shared_ptr<Language>> m_languages;

			std::shared_ptr<Language> m_current_language;
		};
	}
}