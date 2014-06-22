#include "stdafx.h"
#include "PopupDivider.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Win32
		{
			PopupDivider::PopupDivider()
			{

			}

			PopupDivider::~PopupDivider()
			{

			}

			bool PopupDivider::IsDivider() const
			{
				return true;
			}

			int PopupDivider::GetMessageId() const
			{
				return 0;
			}

			const std::string &PopupDivider::GetTitle() const
			{
				static std::string stub;
				return stub;
			}
		}
	}
}