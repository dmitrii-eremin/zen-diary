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

			std::string md5(const char *data, size_t len)
			{
				if (!data || len == 0)
				{
					return md5("");
				}

				MD5 md5(data, len);
				return md5.hexdigest();
			}

			ZD_STATUS EncryptString(std::string data, std::string key, char **new_data, size_t &new_data_size)
			{
				if (key.length() == 0 || data.length() == 0)
				{
					return ZD_ERROR_INVALID_ARGUMENT;
				}

				while (key.length() < 32)
				{
					key += key;
				}

				if (key.length() > 32)
				{
					key = key.substr(0, 32);
				}				

				aes_encrypt_ctx ctx;
				aes_encrypt_key(reinterpret_cast<uchar_t*>(const_cast<char*>(key.c_str())), key.length(), &ctx);				

				const size_t data_length = data.length();
				new_data_size = data_length - (data_length % ZD_AES_BLOCK_LEN) + ZD_AES_BLOCK_LEN;

				while (data.length() < new_data_size)
				{
					data += '\0';
				}

				(*new_data) = new char[new_data_size];

				size_t offset = 0;
				while (offset < data.length())
				{
					aes_encrypt(reinterpret_cast<uchar_t*>(const_cast<char*>(data.c_str() + offset)), reinterpret_cast<uchar_t*>((*new_data) + offset), &ctx);
					offset += ZD_AES_BLOCK_LEN;
				}

				return ZD_NOERROR;
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

				size_t offset = 0;
				while (offset < data_length)
				{					
					aes_decrypt(reinterpret_cast<uchar_t*>(data + offset), reinterpret_cast<uchar_t*>((new_data + offset)), &ctx);
					offset += ZD_AES_BLOCK_LEN;
				}				

				new_data[data_length] = '\0';
				result = new_data;

				delete []new_data;

				return result;
			}

			static const std::string base64_chars =
				"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

			static inline bool is_base64(unsigned char c) 
			{
				return (isalnum(c) || (c == '+') || (c == '/'));
			}

			std::string Base64Encode(char *bytes_to_encode, size_t in_len) 
			{
				std::stringstream result;

				int i = 0;
				int j = 0;
				unsigned char char_array_3[3];
				unsigned char char_array_4[4];

				while (in_len--)
				{
					char_array_3[i++] = *(bytes_to_encode++);

					if (i == 3) 
					{
						char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
						char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
						char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
						char_array_4[3] = char_array_3[2] & 0x3f;

						for (i = 0; (i < 4); i++)
						{
							result << base64_chars[char_array_4[i]];
						}
						i = 0;
					}
				}

				if (i)
				{
					for (j = i; j < 3; j++)
					{
						char_array_3[j] = '\0';
					}

					char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
					char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
					char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
					char_array_4[3] = char_array_3[2] & 0x3f;

					for (j = 0; (j < i + 1); j++)
					{
						result << base64_chars[char_array_4[j]];
					}

					while ((i++ < 3))
					{
						result << "=";
					}

				}

				return result.str();

			}

			ZD_STATUS Base64Decode(const std::string &encoded_string, char **data, size_t &data_size) 
			{
				int in_len = encoded_string.size();
				int i = 0;
				int j = 0;
				int in_ = 0;
				unsigned char char_array_4[4], char_array_3[3];		

				*data = new char[in_len];
				data_size = 0;

				std::string ret;

				while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_])) 
				{
					char_array_4[i++] = encoded_string[in_]; in_++;

					if (i == 4) 
					{
						for (i = 0; i < 4; i++)
						{
							char_array_4[i] = base64_chars.find(char_array_4[i]);
						}

						char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
						char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
						char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

						for (i = 0; (i < 3); i++)
						{
							(*data)[data_size++] = char_array_3[i];
							ret += char_array_3[i];
						}

						i = 0;
					}
				}

				if (i) {
					for (j = i; j < 4; j++)
					{
						char_array_4[j] = 0;
					}

					for (j = 0; j < 4; j++)
					{
						char_array_4[j] = base64_chars.find(char_array_4[j]);
					}

					char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
					char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
					char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

					for (j = 0; (j < i - 1); j++)
					{
						(*data)[data_size++] = char_array_3[j];
						ret += char_array_3[i];
					}
				}

				return ZD_NOERROR;
			}
		}
	}
}