#include "stdafx.h"
#include "ZenDiary.h"

#include "Md5.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Crypto
		{
			std::string md5(std::string src)
			{
				MD5 md5 = MD5(src);

				return md5.hexdigest();
			}

			std::string EncryptString(const std::string &data, std::string key, char **new_data, size_t &new_data_size)
			{
				if (key.length() == 0 || data.length() == 0)
				{
					return std::string();
				}

				while (key.length() < 32)
				{
					key += key;
				}

				if (key.length() > 32)
				{
					key = key.substr(0, 32);
				}

				std::string result;

				aes_encrypt_ctx ctx;
				aes_encrypt_key(reinterpret_cast<uchar_t*>(const_cast<char*>(key.c_str())), key.length(), &ctx);				

				const size_t data_length = data.length();
				new_data_size = data_length - (data_length % AES_BLOCK_LEN) + AES_BLOCK_LEN;

				(*new_data) = new char[new_data_size];

				aes_encrypt(reinterpret_cast<uchar_t*>(const_cast<char*>(data.c_str())), reinterpret_cast<uchar_t*>((*new_data)), &ctx);				

				return result;
			}

			std::string DecryptString(char *data, size_t data_length, std::string key)
			{
				if (key.length() == 0 || !data)
				{
					return std::string();
				}

				while (key.length() < 32)
				{
					key += key;
				}

				if (key.length() > 32)
				{
					key = key.substr(0, 32);
				}

				std::string result;

				aes_decrypt_ctx ctx;
				aes_decrypt_key(reinterpret_cast<uchar_t*>(const_cast<char*>(key.c_str())), key.length(), &ctx);
				
				char *new_data = new char[data_length + 1];

				aes_decrypt(reinterpret_cast<uchar_t*>(data), reinterpret_cast<uchar_t*>((new_data)), &ctx);

				new_data[data_length] = '\0';
				result = new_data;

				delete []new_data;

				return result;
			}
		}
	}
}