#include "stdafx.h"
#include "ZenDiary.h"
#include "utf8.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace String
		{
			std::string To()
			{
				return std::string();
			}

			std::string Replace(const std::string &_src, const std::string &_what, const std::string &to, bool reg_match)
			{
				std::string src;
				std::string what;

				std::string result(_src);

				if (reg_match)
				{
					src = _src;
					what = _what;
				}
				else
				{
					src = Helpers::String::ToLower(_src);
					what = Helpers::String::ToLower(_what);
				}

				size_t index = 0;
				do 
				{
					index = src.find(what, index);
					if (index == std::string::npos)
					{
						break;
					}

					std::string left_result = result.substr(0, index);
					std::string right_result = result.substr(index + what.length());

					std::string left_src = src.substr(0, index);
					std::string right_src = src.substr(index + what.length());

					src = left_src + to + right_src;
					result = left_result + to + right_result;

					index += to.length();

				} while (index != std::string::npos);

				return result;
			}

			std::string ExtractPath(const std::string &fullname)
			{
				size_t index = fullname.find_last_of("\\");
				if (index == std::string::npos)
				{
					index = fullname.find_last_of("/");
					if (index == std::string::npos)
					{
						return std::string();
					}
				}

				return fullname.substr(0, index + 1);
			}

			std::string ExtractFilename(const std::string &fullname)
			{
				size_t index = fullname.find_last_of("\\");
				if (index == std::string::npos)
				{
					index = fullname.find_last_of("/");
					if (index == std::string::npos)
					{
						return fullname;
					}
				}

				return fullname.substr(index + 1);
			}

			std::string ExtractExtension(const std::string &fullname)
			{
				size_t index = fullname.find_last_of(".");
				if (index == std::string::npos)
				{
					return std::string();					
				}

				return fullname.substr(index + 1);
			}

			std::string FilenameToUrl(const std::string &filename)
			{
				std::stringstream stream;
				stream << "file:///" << filename;
				return stream.str();
			}	

			std::string GenerateString(const std::string &alphabet, size_t count)
			{
				if (alphabet.length() == 0 || count == 0)
				{
					return std::string();
				}

				std::stringstream result;

				const size_t length = alphabet.length();

				for (size_t i = 0; i < count; i++)
				{
					size_t index = rand() % length;
					result << alphabet[index];
				}

				return result.str();
			}

			std::string ToUtf8(const std::wstring &src)
			{
				std::string result;				
				utf8::utf32to8(src.c_str(), src.c_str() + src.length(), std::back_inserter(result));

				return result;
			}

			std::string FormatTime(const SYSTEMTIME &value)
			{
				auto z = [](const int &v) -> std::string
				{
					std::string s;
					s = Helpers::String::To(v);
					if (s.length() < 2)
					{
						return "0" + s;
					}

					return s;
				};

				std::stringstream stream;
				stream << z(value.wYear) << "-" << z(value.wMonth) << "-" << z(value.wDay) << " " <<
					z(value.wHour) << ":" << z(value.wMinute) << ":" << z(value.wSecond);

				return stream.str();
			}

			std::string ToLower(const std::string &data)
			{
				return boost::to_lower_copy(data);
			}

			std::string ToUpper(const std::string &data)
			{
				return boost::to_upper_copy(data);
			}

			std::string wstrtostr(const std::wstring &wstr)
			{
				std::string strTo;
				char *szTo = new char[wstr.length() + 1];
				szTo[wstr.size()] = '\0';
				WideCharToMultiByte(CP_ACP, 0, wstr.c_str(), -1, szTo, (int)wstr.length(), NULL, NULL);
				strTo = szTo;
				delete[] szTo;
				return strTo;
			}

			std::wstring strtowstr(const std::string &str)
			{
				std::wstring wstrTo;
				wchar_t *wszTo = new wchar_t[str.length() + 1];
				wszTo[str.size()] = L'\0';
				MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, wszTo, (int)str.length());
				wstrTo = wszTo;
				delete[] wszTo;
				return wstrTo;
			}

			std::string ConvertUtf8ToMB(const std::string &src)
			{
				char *utf8 = const_cast<char*>(src.c_str());
				int length = MultiByteToWideChar(CP_UTF8, 0, utf8, -1, NULL, 0);

				std::string res = "";

				if (length > 0)
				{
					wchar_t *wide = new wchar_t[length];

					MultiByteToWideChar(CP_UTF8, 0, utf8, -1, wide, length);

					size_t converted_chars = 0;

					res = wstrtostr(wide);

					delete[]wide;
				}

				return res;
			}

			std::string UriDecode(const std::string & sSrc)
			{
				// Note from RFC1630: "Sequences which start with a percent
				// sign but are not followed by two hexadecimal characters
				// (0-9, A-F) are reserved for future extension"

				static const char HEX2DEC[256] =
				{
					/*       0  1  2  3   4  5  6  7   8  9  A  B   C  D  E  F */
					/* 0 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* 1 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* 2 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* 3 */  0, 1, 2, 3, 4, 5, 6, 7, 8, 9, -1, -1, -1, -1, -1, -1,

					/* 4 */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* 5 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* 6 */ -1, 10, 11, 12, 13, 14, 15, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* 7 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

					/* 8 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* 9 */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* A */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* B */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,

					/* C */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* D */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* E */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
					/* F */ -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
				};

				const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
				const int SRC_LEN = sSrc.length();
				const unsigned char * const SRC_END = pSrc + SRC_LEN;
				// last decodable '%' 
				const unsigned char * const SRC_LAST_DEC = SRC_END - 2;

				char * const pStart = new char[SRC_LEN];
				char * pEnd = pStart;

				while (pSrc < SRC_LAST_DEC)
				{
					if (*pSrc == '%')
					{
						char dec1, dec2;
						if (-1 != (dec1 = HEX2DEC[*(pSrc + 1)])
							&& -1 != (dec2 = HEX2DEC[*(pSrc + 2)]))
						{
							*pEnd++ = (dec1 << 4) + dec2;
							pSrc += 3;
							continue;
						}
					}

					*pEnd++ = *pSrc++;
				}

				// the last 2- chars
				while (pSrc < SRC_END)
					*pEnd++ = *pSrc++;

				std::string sResult(pStart, pEnd);
				delete[] pStart;
				return sResult;
			}

			std::string UriEncode(const std::string & sSrc)
			{
				static const char SAFE[256] =
				{
					/*      0 1 2 3  4 5 6 7  8 9 A B  C D E F */
					/* 0 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* 1 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* 2 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* 3 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0,

					/* 4 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					/* 5 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,
					/* 6 */ 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
					/* 7 */ 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0,

					/* 8 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* 9 */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* A */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* B */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,

					/* C */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* D */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* E */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
					/* F */ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
				};

				const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
				const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
				const int SRC_LEN = sSrc.length();
				unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
				unsigned char * pEnd = pStart;
				const unsigned char * const SRC_END = pSrc + SRC_LEN;

				for (; pSrc < SRC_END; ++pSrc)
				{
					if (SAFE[*pSrc])
						*pEnd++ = *pSrc;
					else
					{
						// escape this char
						*pEnd++ = '%';
						*pEnd++ = DEC2HEX[*pSrc >> 4];
						*pEnd++ = DEC2HEX[*pSrc & 0x0F];
					}
				}

				std::string sResult((char *)pStart, (char *)pEnd);
				delete[] pStart;
				return sResult;
			}
		}
	}
}