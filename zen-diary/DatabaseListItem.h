#pragma once

namespace ZenDiary
{
	namespace App
	{
		class DatabaseListItem
		{
		public:
			DatabaseListItem(const std::string &path = std::string(), const std::string &name = std::string());
			~DatabaseListItem();

			DatabaseListItem(const DatabaseListItem &a) = default;
			DatabaseListItem &operator = (const DatabaseListItem &a) = default;

		private:
			ZD_PROPERTY(std::string, m_path, Path);
			ZD_PROPERTY(std::string, m_name, Name);			
		};
	};
};