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
				DWORD attr = GetFileAttributes(filename.c_str());
				return attr != INVALID_FILE_ATTRIBUTES && !(attr & FILE_ATTRIBUTE_DIRECTORY);
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

			bool DownloadFile(const std::string &filename, char *bytes, size_t &max_size)
			{
				if (!bytes || max_size == 0)
				{
					return false;
				}

				HINTERNET hInternet = InternetOpen(ZD_USER_AGENT, INTERNET_OPEN_TYPE_DIRECT,
					nullptr, nullptr, 0);

				if (!hInternet)
				{
					return false;
				}

				HINTERNET hRequest = InternetOpenUrl(hInternet, filename.c_str(),
					nullptr, 0, 0, 0);

				if (!hRequest)
				{
					InternetCloseHandle(hInternet);
					return false;
				}

				DWORD filesize = 0;

				char buf[32];
				DWORD buf_len = sizeof(buf);

				HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, buf, &buf_len, nullptr);

				filesize = atol(buf);

				const size_t buffer_size = 1024 * 8;
				size_t size_to_read = buffer_size;

				bool updating = false;

				char *data_ptr = bytes;
				size_t read_size = 0;

				if (filesize)
				{
					DWORD dwRead = 0;

					size_t to_read = min(max_size, filesize);

					InternetReadFile(hRequest, bytes, to_read, &dwRead);
				}

				InternetCloseHandle(hRequest);
				InternetCloseHandle(hInternet);

				return true;
			}

			size_t GetInternetFileSize(const std::string &filename)
			{
				HINTERNET hInternet = InternetOpen(ZD_USER_AGENT, INTERNET_OPEN_TYPE_DIRECT,
					nullptr, nullptr, 0);

				if (!hInternet)
				{
					return false;
				}

				HINTERNET hRequest = InternetOpenUrl(hInternet, filename.c_str(),
					nullptr, 0, 0, 0);

				if (!hRequest)
				{
					InternetCloseHandle(hInternet);
					return false;
				}

				DWORD filesize = 0;

				char buf[32];
				DWORD buf_len = sizeof(buf);

				HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, buf, &buf_len, nullptr);

				filesize = atol(buf);

				InternetCloseHandle(hRequest);
				InternetCloseHandle(hInternet);

				return filesize;
			}
		}
	}
}