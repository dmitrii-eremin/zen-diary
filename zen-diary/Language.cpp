#include "stdafx.h"
#include "Language.h"

namespace ZenDiary
{
	namespace App
	{
		Language::Language()
		{

		}

		Language::~Language()
		{

		}

		ZD_STATUS Language::Serialize(JsonBox::Value &root) const
		{
			return ZD_ERROR_NOT_IMPLEMENTED;
		}

		ZD_STATUS Language::Deserialize(JsonBox::Value &root)
		{
			if (!root["info"].isObject() || 
				!root["locale"].isObject())
			{
				return ZD_ERROR_BAD_DATA;
			}

			JsonBox::Object info = root["info"].getObject();			

			if (!info["language"].isString() ||
				!info["title"].isString())
			{
				return ZD_ERROR_BAD_DATA;
			}

			m_language = info["language"].getString();
			m_title = info["title"].getString();

			return LoadNode(root["locale"]);
		}

		const std::string &Language::Get(const std::string &key) const
		{
			auto i = m_words.find(key);
			if (i == m_words.end())
			{
				static const std::string stub("//?//");
				return stub;
			}

			return i->second;
		}

		ZD_STATUS Language::LoadNode(JsonBox::Value &node, const std::string &prefix)
		{
			if (node.isObject())
			{
				JsonBox::Object obj = node.getObject();

				for (auto &i : obj)
				{
					std::stringstream new_prefix;

					if (prefix.length() > 0)
					{
						new_prefix << prefix << m_delimiter;						
					}

					new_prefix << i.first;

					LoadNode(i.second, new_prefix.str());
				}
			}
			else if (node.isString())
			{
				m_words[prefix] = node.getString();
			}
			else if (node.isInteger())
			{
				m_words[prefix] = Helpers::String::To(node.getInt());
			}
			return ZD_NOERROR;
		}
	}
}