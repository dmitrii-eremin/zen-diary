#pragma once

namespace ZenDiary
{
	namespace Interfaces
	{
		struct IApplication
		{
			virtual ZD_STATUS Initialize(const std::string &argv) = 0;
			virtual ZD_STATUS Deinitialize() = 0;
			virtual ZD_STATUS Run() = 0;

			virtual ZD_STATUS Terminate() = 0;
		};
	}
}