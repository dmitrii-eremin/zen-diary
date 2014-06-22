#pragma once
#include "PopupItemBase.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Win32
		{
			class PopupItem : public PopupItemBase
			{
			public:
				PopupItem(int message_id, const std::string &title);
				virtual ~PopupItem();

				PopupItem(const PopupItem &a) = default;
				PopupItem &operator = (const PopupItem &a) = default;

				virtual bool IsDivider() const override final;

				virtual int GetMessageId() const override final;
				virtual const std::string &GetTitle() const override final;

			private:
				int m_message_id;
				std::string m_title;
			};
		}
	}
}