#pragma once

typedef unsigned char							uchar;
typedef unsigned short int						ushort;
typedef unsigned int							uint;

#define ZD_STATUS								ushort

#define ZD_NOERROR								0x0000
#define ZD_ERROR_UNKNOWN						0xFFFF

#define ZD_ERROR_NOT_EXIST						0x01
#define ZD_ERROR_INVALID_ARGUMENT				0x02
#define ZD_ERROR_ACCESS_DENIED					0x03
#define ZD_ERROR_SMALL_BUFFER					0x04

#define ZD_FAILED(X)							((X) != ZD_NOERROR)
#define ZD_SUCCEEDED(X)							((X) == ZD_NOERROR)

#define ZD_RETURN_IF_FAILED(X)					{ ZD_STATUS status = ZD_NOERROR; if (ZD_FAILED((status = (X)))) { return status; } }

#define ZD_PROPERTY(T, O, N)						private: T O; public: const T &Get##N() const { return O; } ZD_STATUS Set##N(const T &v) { O = v; return ZD_NOERROR; }
#define ZD_BOOL_PROPERTY(O, N)					private: bool O; public: bool Is##N() const { return O; } ZD_STATUS Set##N(bool v) { O = v; return ZD_NOERROR; }

#define ZD_PROPERTY_GETTER_BY_REF(T, O, N)		private: T O; public: T &Get##N() { return O; }
#define ZD_PROPERTY_CONST_GETTER_BY_REF(T, O, N)	private: T O; public: const T &Get##N() const { return O; }

#include "IApplication.h"
#include "ISerializable.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Files
		{
			bool IsFileExist(const std::string &filename);
			ZD_STATUS GetFileSize(const std::string &filename, uint &filesize);

			ZD_STATUS GetFileContent(const std::string &filename, char *data, size_t buf_size);
			ZD_STATUS GetFileContent(const std::string &filename, std::string &data);

			ZD_STATUS SetFileContent(const std::string &filename, const char *data, size_t buf_size);
			ZD_STATUS SetFileContent(const std::string &filename, const std::string &data);
		}

		namespace String
		{
			std::string To();

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
		}

		namespace Serialization
		{
			ZD_STATUS ToFile(const Interfaces::ISerializable &object, const std::string &filename);
			ZD_STATUS FromFile(Interfaces::ISerializable &object, const std::string &filename);
		}
	}
}