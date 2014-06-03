#pragma once
#include "ArgumentType.h"

namespace ZenDiary
{
	namespace App
	{
		class ArgumentValue
		{
		public:
			ArgumentValue();
			~ArgumentValue();

			ArgumentValue(const ArgumentValue &a) = default;
			ArgumentValue &operator = (const ArgumentValue &a) = default;

			bool IsFlag() const;
			ZD_STATUS SetFlag(bool value);

			int GetNumberValue() const;
			ZD_STATUS SetNumberValue(int value);

			const std::string &GetStringValue() const;
			ZD_STATUS SetStringValue(const std::string &value);

		private:
			struct
			{
				int i_val;
				std::string s_val;
				bool b_val;
			} m_value;

			ZD_PROPERTY(ArgumentType, m_type, ArgumentType);
		};
	};
};