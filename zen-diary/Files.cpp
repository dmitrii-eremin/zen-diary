#include "stdafx.h"
#include "ZenDiary.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Files
		{
			bool IsFileExist(const std::string &filename)
			{
				boost::filesystem::path filepath(filename);		
				return boost::filesystem::exists(filepath);
			}

			ZD_STATUS GetFileSize(const std::string &filename, uint_t &filesize)
			{
				if (!IsFileExist(filename))
				{
					return ZD_ERROR_NOT_EXIST;
				}

				boost::filesystem::path filepath(filename);
				filesize = static_cast<uint_t>(boost::filesystem::file_size(filepath));

				return ZD_NOERROR;
			}

			ZD_STATUS GetFileContent(const std::string &filename, char *data, size_t buf_size)
			{
				if (!IsFileExist(filename))
				{
					return ZD_ERROR_NOT_EXIST;
				}

				if (!data || buf_size == 0)
				{
					return ZD_ERROR_INVALID_ARGUMENT;
				}

				uint_t filesize = 0;
				ZD_STATUS status = GetFileSize(filename, filesize);
				if (ZD_FAILED(status))
				{
					return status;
				}

				if (filesize == 0)
				{
					return ZD_NOERROR;
				}

				if (buf_size < filesize)
				{
					return ZD_ERROR_SMALL_BUFFER;
				}

				std::ifstream file(filename, std::ios::in | std::ios::binary);
				if (!file.is_open())
				{
					return ZD_ERROR_ACCESS_DENIED;
				}

				file.read(data, filesize);
				file.close();
				return ZD_NOERROR;
			}

			ZD_STATUS GetFileContent(const std::string &filename, std::string &data)
			{
				if (!IsFileExist(filename))
				{
					return ZD_ERROR_NOT_EXIST;
				}

				
				size_t filesize = 0;
				ZD_STATUS status = GetFileSize(filename, static_cast<size_t&>(filesize));
				if (ZD_FAILED(status))
				{
					return status;
				}

				if (filesize == 0)
				{
					return ZD_NOERROR;
				}

				char *cdata = new char[filesize + 1];

				status = GetFileContent(filename, cdata, filesize + 1);
				if (ZD_FAILED(status))
				{
					return status;
				}

				cdata[filesize] = '\0';
				data = cdata;

				delete []cdata;
				return ZD_NOERROR;
			}

			ZD_STATUS SetFileContent(const std::string &filename, const char *data, size_t buf_size)
			{
				std::ofstream file(filename, std::ios::out | std::ios::binary);
				if (!file.is_open())
				{
					return ZD_ERROR_ACCESS_DENIED;
				}
				file.write(data, buf_size);
				file.close();
				return ZD_NOERROR;
			}

			ZD_STATUS SetFileContent(const std::string &filename, const std::string &data)
			{
				return SetFileContent(filename, data.c_str(), data.length());
			}
		}
	}
}