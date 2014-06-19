#pragma once

namespace ZenDiary
{
	namespace App
	{
		class DatabaseVersionUpdate
		{
		public:
			DatabaseVersionUpdate(int version = 0, const std::string &filename = std::string());
			~DatabaseVersionUpdate();

			DatabaseVersionUpdate(const DatabaseVersionUpdate &a) = default;
			DatabaseVersionUpdate &operator = (const DatabaseVersionUpdate &a) = default;			

		private:
			ZD_PROPERTY(int, m_version, Version);
			ZD_PROPERTY(std::string, m_filename, Filename);
		};

		bool operator < (const DatabaseVersionUpdate &a, const DatabaseVersionUpdate &b);
	};
};