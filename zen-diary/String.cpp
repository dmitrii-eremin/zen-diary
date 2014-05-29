#include "stdafx.h"
#include "ZenDiary.h"

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

			std::string FilenameToUrl(const std::string &filename)
			{
				std::stringstream stream;
				stream << "file:///" << filename;
				return stream.str();
			}			
		}
	}
}