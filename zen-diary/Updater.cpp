#include "stdafx.h"
#include "Updater.h"

namespace ZenDiary
{
	namespace App
	{
		Updater::Updater() : 
			m_need_to_update(false),
			m_users_count(0),
			m_update_link("https://github.com/NeonMercury/zen-diary/releases")
		{

		}

		Updater::~Updater()
		{

		}

		ZD_STATUS Updater::DownloadUsersCount()
		{
			m_users_count = 0;
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

					if (root["version-list"].isArray())
					{
						JsonBox::Array root_array = root["version-list"].getArray();

						std::set<VersionInfo> versions;

						for (auto &i : root_array)
						{
							if (!i.isObject())
							{
								continue;
							}

							VersionInfo version;

							if (!i["major"].isInteger() ||
								!i["middle"].isInteger() ||
								!i["minor"].isInteger() ||
								!i["link"].isString())
							{
								continue;
							}

							version.major = i["major"].getInt();
							version.middle = i["middle"].getInt();
							version.minor = i["minor"].getInt();
							version.link = i["link"].getString();

							if (i["changelog"].isString())
							{
								version.changelog = Helpers::String::ConvertUtf8ToMB(i["changelog"].getString());
							}

							versions.insert(version);
						}

						for (auto &i : versions)
						{
							uint_t remote_version = ZD_MAKE_VERSION(i.major, i.middle, i.minor);

							if (remote_version > ZD_VERSION)
							{
								m_need_to_update = true;
								if (i.changelog.length() > 0)
								{
									m_change_log += Helpers::String::ConvertUtf8ToMB(i.changelog);
									m_change_log += std::string("\n");									
								}
							}
						}

						if (m_need_to_update)
						{
							std::set<VersionInfo>::iterator last_version = --versions.end();
							
							std::stringstream new_version_stream;
							new_version_stream << static_cast<uint_t>(last_version->major) << "." << 
								static_cast<uint_t>(last_version->middle) << "." << 
								static_cast<uint_t>(last_version->minor);

							m_new_verson = new_version_stream.str();
							m_update_link = last_version->link;
						}
					}														
				}

				delete []buf;
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

		const std::string &Updater::GetNewVersion() const
		{
			return m_new_verson;
		}

		const std::string &Updater::GetChangeLog() const
		{
			return m_change_log;
		}

		bool Updater::VersionInfo::operator < (const Updater::VersionInfo &info) const
		{
			uint_t current_version = ZD_MAKE_VERSION(major, middle, minor);
			uint_t remote_version = ZD_MAKE_VERSION(info.major, info.middle, info.minor);

			return current_version < remote_version;
		}
	}
}