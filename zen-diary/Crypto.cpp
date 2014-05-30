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
		}
	}
}