#pragma once

namespace ZenDiary
{
	namespace App
	{
		class SpellChecker
		{
		public:
			SpellChecker();
			~SpellChecker();

			SpellChecker(const SpellChecker &a) = default;
			SpellChecker &operator = (const SpellChecker &a) = default;

			JsonBox::Array Check(const std::string &text);

		private:
			const std::string m_spell_check_url = std::string("http://speller.yandex.net/services/spellservice.json/checkText?options=63&text=");
		};
	};
};