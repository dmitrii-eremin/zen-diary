#pragma once

namespace ZenDiary
{
	namespace App
	{
		class Updater
		{
		public:
			Updater();
			~Updater();

			Updater(const Updater &a) = delete;
			Updater &operator = (const Updater &a) = delete;

			ZD_STATUS CheckForUpdates();
			bool IsNeedToUpdate() const;

			ZD_STATUS OpenUpdateLink() const;

		private:
#ifdef _DEBUG
			const std::string m_update_version_file_path = std::string("http://zendiary.dev/api/get-last-version.php");			
			const std::string m_update_link = std::string("http://zendiary.dev/update?app=1");
#else
			const std::string m_update_version_file_path = std::string("http://zendiary.org/api/get-last-version.php");			
			const std::string m_update_link = std::string("http://zendiary.org/update?app=1");
#endif			

			bool m_need_to_update;
			std::string m_new_verson;			
		};
	};
};