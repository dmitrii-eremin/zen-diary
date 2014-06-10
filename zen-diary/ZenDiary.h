#pragma once

#define __STR2__(x) #x
#define __STR1__(x) __STR2__(x)
#define __LOC2__ __FILE__ "("__STR1__(__LINE__)") : "

#ifdef NDEBUG
#define FIXME(X) __pragma(message(__LOC2__ "found FIXME in release build: " ## X))
#else
#define FIXME(X)
#endif

typedef unsigned char								uchar_t;
typedef unsigned short int							ushort_t;
typedef unsigned int								uint_t;

typedef uint_t										version_t;

#define ZD_STATUS									ushort_t

#define ZD_NOERROR									0x0000
#define ZD_ERROR_UNKNOWN							0xFFFF

#define ZD_ERROR_NOT_EXIST							0x01
#define ZD_ERROR_INVALID_ARGUMENT					0x02
#define ZD_ERROR_ACCESS_DENIED						0x03
#define ZD_ERROR_SMALL_BUFFER						0x04
#define ZD_ERROR_NOT_INITIALIZED					0x05
#define ZD_ERROR_FAILED_TO_CREATE					0x06
#define ZD_ERROR_FAILED_TO_OPEN						0x07
#define ZD_ERROR_BAD_DATA							0x08

#define ZD_MAKE_VERSION(HI, MI, LO)					(((HI) << 16) + ((MI) << 8) + (LO))
#define ZD_VERSION_HI(V)							((V) >> 16)
#define ZD_VERSION_MID(V)							(((V) & 0xFF00) >> 8)
#define ZD_VERSION_LOW(V)							((V) & 0xFF)

#define ZD_FAILED(X)								((X) != ZD_NOERROR)
#define ZD_SUCCEEDED(X)								((X) == ZD_NOERROR)

#define ZD_RETURN_IF_FAILED(X)						{ ZD_STATUS status = ZD_NOERROR; if (ZD_FAILED((status = (X)))) { return status; } }

#define ZD_PROPERTY(T, O, N)						private: T O; public: const T &Get##N() const { return O; } ZD_STATUS Set##N(const T &v) { O = v; return ZD_NOERROR; }
#define ZD_BOOL_PROPERTY(O, N)						private: bool O; public: bool Is##N() const { return O; } ZD_STATUS Set##N(bool v) { O = v; return ZD_NOERROR; }

#define ZD_PROPERTY_GETTER_BY_REF(T, O, N)			private: T O; public: T &Get##N() { return O; }
#define ZD_PROPERTY_CONST_GETTER_BY_REF(T, O, N)	private: T O; public: const T &Get##N() const { return O; }

#define ZD_SAFE_CALL(X)								if (X) X
#define ZD_SAFE_DELETE(X)							if (X) { delete X; X = nullptr; }
#define ZD_SAFE_DELETE_ARRAY(X)						if (X) { delete []X; X = nullptr; }

#define ZD_VERSION									ZD_MAKE_VERSION(2, 1, 4)
#define ZD_VERSION_STATUS							"Beta"

#define ZD_AES_BLOCK_LEN							16
#define ZD_USER_AGENT								"ZenDiaryApp/1.0"

#include "IApplication.h"
#include "ISerializable.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Files
		{
			bool IsFileExist(const std::string &filename);
			ZD_STATUS GetFileSize(const std::string &filename, uint_t &filesize);

			ZD_STATUS GetFileContent(const std::string &filename, char *data, size_t buf_size);
			ZD_STATUS GetFileContent(const std::string &filename, std::string &data);

			ZD_STATUS SetFileContent(const std::string &filename, const char *data, size_t buf_size);
			ZD_STATUS SetFileContent(const std::string &filename, const std::string &data);

			bool DownloadFile(const std::string &filename, char *bytes, size_t &max_size);
			size_t GetInternetFileSize(const std::string &filename);
		}

		namespace String
		{
			std::string To();

			std::string Replace(const std::string &src, const std::string &what, const std::string &to, bool reg_match = true);

			std::string ExtractPath(const std::string &fullname);
			std::string ExtractFilename(const std::string &fullname);
			std::string ExtractExtension(const std::string &fullname);
			std::string FilenameToUrl(const std::string &filename);	

			std::string GenerateString(const std::string &alphabet = std::string("0123456789abcdef"), size_t count = 32);
			std::string ToUtf8(const std::wstring &src);

			std::string wstrtostr(const std::wstring &wstr);
			std::wstring strtowstr(const std::string &str);
			std::string ConvertUtf8ToMB(const std::string &src);

			template <class T, class ...ARGS>
			std::string To(const T &value, ARGS ...v)
			{
				std::stringstream stream;
				stream << value << To(v...);
				return stream.str();
			}

			template <class T, class ...ARGS>
			std::string Format(const T &value, ARGS ...v)
			{
				return To(value, v...);
			}
			
			std::string FormatTime(const SYSTEMTIME &value);

			std::string ToLower(const std::string &data);
			std::string ToUpper(const std::string &data);
		}

		namespace Serialization
		{
			ZD_STATUS ToFile(const Interfaces::ISerializable &object, const std::string &filename);
			ZD_STATUS FromFile(Interfaces::ISerializable &object, const std::string &filename);
		}

		namespace Crypto
		{
			std::string md5(std::string src);
			std::string md5(const char *data, size_t len);

			ZD_STATUS EncryptString(std::string data, std::string key, char **new_data, size_t &new_data_size);
			std::string DecryptString(char *data, size_t data_length, std::string key);

			std::string Base64Encode(char *bytes_to_encode, size_t in_len);
			ZD_STATUS Base64Decode(const std::string &encoded_string, char **data, size_t &data_size);
		}

		namespace Win32
		{
			ZD_STATUS SetClipboardText(const std::string &text);
		}
	}
}