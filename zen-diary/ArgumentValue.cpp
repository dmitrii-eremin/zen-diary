#include "stdafx.h"
#include "ArgumentValue.h"

namespace ZenDiary
{
	namespace App
	{
		ArgumentValue::ArgumentValue() : 
			m_type(ArgumentType::Flag)
		{
			m_value.b_val = false;
			m_value.i_val = 0;			
		}

		ArgumentValue::~ArgumentValue()
		{

		}

		bool ArgumentValue::IsFlag() const
		{
			return m_value.b_val;
		}

		ZD_STATUS ArgumentValue::SetFlag(bool value)
		{
			m_value.b_val = value;
			return ZD_NOERROR;
		}


		int ArgumentValue::GetNumberValue() const
		{
			return m_value.i_val;
		}

		ZD_STATUS ArgumentValue::SetNumberValue(int value)
		{
			m_value.i_val = value;
			return ZD_NOERROR;
		}


		const std::string &ArgumentValue::GetStringValue() const
		{
			return m_value.s_val;
		}

		ZD_STATUS ArgumentValue::SetStringValue(const std::string &value)
		{
			m_value.s_val = value;
			return ZD_NOERROR;
		}

	}
}