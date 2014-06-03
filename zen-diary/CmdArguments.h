#pragma once
#include "ArgumentValue.h"

namespace ZenDiary
{
	namespace App
	{
		class CmdArguments
		{
		public:
			CmdArguments();
			~CmdArguments();

			CmdArguments(const CmdArguments &a) = default;
			CmdArguments &operator = (const CmdArguments &a) = default;

			ZD_STATUS Parse(const std::string &args);

			bool Is(const std::string &key, bool def = false);
			const std::string &GetString(const std::string &key, const std::string &def = std::string());
			int GetNumber(const std::string &key, int def = 0);

		private:
			ZD_STATUS Tokenize(const std::string &args, std::vector<std::string> &tokens);
			bool IsArgumentName(const std::string &v);
			bool IsNumber(const std::string &v);

		private:
			std::map<std::string, ArgumentValue*> m_arguments;
		};
	};
};