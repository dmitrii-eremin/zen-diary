#include "stdafx.h"
#include "Updater.h"

namespace ZenDiary
{
	namespace App
	{
		Updater::Updater() : 
			m_need_to_update(false),
			m_users_count(0)
		{

		}

		Updater::~Updater()
		{

		}

		ZD_STATUS Updater::DownloadUsersCount()
		{
			size_t fsize = Helpers::Files::GetInternetFileSize(m_users_count_path);
			if (fsize > 0)
			{
				char *buf = new char[fsize + 1];
				if (Helpers::Files::DownloadFile(m_users_count_path, buf, fsize))
				{
					buf[fsize] = '\0';

					JsonBox::Value root;
					root.loadFromString(buf);

					if (root["users"].isInteger())
					{
						m_users_count = root["users"].getInt();
					}
				}

				delete []buf;
			}

			return ZD_NOERROR;
		}

		ZD_STATUS Updater::CheckForUpdates()
		{
			size_t fsize = Helpers::Files::GetInternetFileSize(m_update_version_file_path);
			if (fsize > 0)
			{
				char *buf = new char[fsize + 1];
				if (Helpers::Files::DownloadFile(m_update_version_file_path, buf, fsize))
				{
					buf[fsize] = '\0';

					JsonBox::Value root;
					root.loadFromString(buf);

					if (root["version"].isObject())
					{
						JsonBox::Object version = root["version"].getObject();

						if (version["major"].isInteger() &&
							version["middle"].isInteger() &&
							version["minor"].isInteger())
						{
							int major = version["major"].getInt();
							int middle = version["middle"].getInt();
							int minor = version["minor"].getInt();

							uint_t remote_version = ZD_MAKE_VERSION(major, middle, minor);

							if (remote_version > ZD_VERSION)
							{
								m_need_to_update = true;
								
								std::stringstream new_version_stream;
								new_version_stream << major << "." << middle << "." << minor;

								m_new_verson = new_version_stream.str();
							}
						}
					}					
				}

				delete[]buf;
			}
			return ZD_NOERROR;
		}

		bool Updater::IsNeedToUpdate() const
		{
			return m_need_to_update;
		}

		ZD_STATUS Updater::OpenUpdateLink() const
		{
			ShellExecute(nullptr, "open", m_update_link.c_str(), nullptr, nullptr, SW_SHOWNORMAL);
			return ZD_NOERROR;
		}

		int Updater::GetUsersCount() const
		{
			return m_users_count;
		}
	}
}