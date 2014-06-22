#include "stdafx.h"
#include "PopupItem.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Win32
		{
			PopupItem::PopupItem(int message_id, const std::string &title) : 
				m_message_id(message_id),
				m_title(title)
			{

			}

			PopupItem::~PopupItem()
			{

			}

			bool PopupItem::IsDivider() const
			{
				return false;
			}

			int PopupItem::GetMessageId() const
			{
				return m_message_id;
			}

			const std::string &PopupItem::GetTitle() const
			{
				return m_title;
			}
		}
	}
}