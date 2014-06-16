#pragma once

namespace ZenDiary
{
	namespace App
	{
		class Language : public Interfaces::ISerializable
		{
		public:
			Language();
			virtual ~Language();

			Language(const Language &a) = default;
			Language &operator = (const Language &a) = default;

			virtual ZD_STATUS Serialize(JsonBox::Value &root) const override final;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) override final;

			const std::string &Get(const std::string &key) const;

		private:
			ZD_STATUS LoadNode(JsonBox::Value &node, const std::string &prefix = std::string());

		private:
			const std::string m_delimiter = std::string(".");

			ZD_PROPERTY(std::string, m_language, Language);
			ZD_PROPERTY(std::string, m_title, Title);

			std::map<std::string, std::string> m_words;
		};
	}
}