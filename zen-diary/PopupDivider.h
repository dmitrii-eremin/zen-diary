#pragma once
#include "PopupItemBase.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Win32
		{
			class PopupDivider : public PopupItemBase
			{
			public:
				PopupDivider();
				virtual ~PopupDivider();

				PopupDivider(const PopupDivider &a) = default;
				PopupDivider &operator = (const PopupDivider &a) = default;

				virtual bool IsDivider() const override final;

				virtual int GetMessageId() const override final;
				virtual const std::string &GetTitle() const override final;
			};
		}
	}
}