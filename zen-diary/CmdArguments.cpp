#include "stdafx.h"
#include "CmdArguments.h"

namespace ZenDiary
{
	namespace App
	{
		CmdArguments::CmdArguments()
		{

		}

		CmdArguments::~CmdArguments()
		{
			for (auto &i : m_arguments)
			{
				delete i.second;
			}
		}

		ZD_STATUS CmdArguments::Parse(const std::string &args)
		{
			std::vector<std::string> tokens;
			ZD_RETURN_IF_FAILED(Tokenize(args, tokens));

			const size_t tokens_count = tokens.size();
			for (size_t i = 0; i < tokens_count; i++)
			{
				const std::string &token = tokens[i];
				if (!IsArgumentName(token))
				{
					continue;
				}

				auto tk = m_arguments.find(token);
				if (tk != m_arguments.end())
				{
					continue;
				}
				
				if ((i == tokens_count - 1) || (IsArgumentName(tokens[i + 1])))
				{
					ArgumentValue *val = new ArgumentValue();
					val->SetArgumentType(ArgumentType::Flag);
					val->SetFlag(true);
					

					m_arguments[token] = val;
				}
				else
				{
					const std::string &next_token = tokens[i + 1];
					if (IsNumber(next_token))
					{
						ArgumentValue *val = new ArgumentValue();
						val->SetArgumentType(ArgumentType::Number);
						val->SetNumberValue(atoi(next_token.c_str()));

						m_arguments[token] = val;
					}
					else
					{
						ArgumentValue *val = new ArgumentValue();
						val->SetArgumentType(ArgumentType::String);
						val->SetStringValue(next_token);

						m_arguments[token] = val;
					}
				}
				
			}
			return ZD_NOERROR;
		}

		bool CmdArguments::Is(const std::string &key, bool def)
		{
			auto i = m_arguments.find(key);
			if (i == m_arguments.end() || i->second->GetArgumentType() != ArgumentType::Flag)
			{
				return def;
			}		

			return i->second->IsFlag();
		}

		const std::string &CmdArguments::GetString(const std::string &key, const std::string &def)
		{
			auto i = m_arguments.find(key);
			if (i == m_arguments.end() || i->second->GetArgumentType() != ArgumentType::String)
			{
				return def;
			}

			return i->second->GetStringValue();
		}

		int CmdArguments::GetNumber(const std::string &key, int def)
		{
			auto i = m_arguments.find(key);
			if (i == m_arguments.end() || i->second->GetArgumentType() != ArgumentType::Number)
			{
				return def;
			}

			return i->second->GetNumberValue();
		}

		ZD_STATUS CmdArguments::Tokenize(const std::string &args, std::vector<std::string> &tokens)
		{
			bool quotes = false;

			const size_t args_len = args.length();

			std::stringstream stream;
			for (size_t i = 0; i < args_len; i++)
			{
				if (args[i] == ' ' && !quotes)
				{
					std::string arg = stream.str();
					if (arg.length() > 0)
					{
						if (arg[0] == '"' && arg[arg.length() - 1] == '"')
						{
							arg = arg.substr(1, arg.length() - 2);
						}
						tokens.push_back(arg);
					}
					stream.str("");
				}
				else
				{
					stream << args[i];
				}

				if (args[i] == '"')
				{
					quotes = !quotes;
				}
			}
			
			std::string arg = stream.str();
			if (arg.length() > 0)
			{
				if (arg[0] == '"' && arg[arg.length() - 1] == '"')
				{
					arg = arg.substr(1, arg.length() - 2);
				}
				tokens.push_back(arg);
			}
			return ZD_NOERROR;
		}

		bool CmdArguments::IsArgumentName(const std::string &v)
		{
			if (v.length() == 0)
			{
				return false;
			}

			return v[0] == '-';
		}

		bool CmdArguments::IsNumber(const std::string &v)
		{
			for (auto &i : v)
			{
				if (i < '0' || i > '9')
				{
					return false;
				}
			}
			
			return true;
		}
	}
}