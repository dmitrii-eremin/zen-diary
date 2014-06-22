#pragma once

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Win32
		{
			class PopupItemBase
			{
			public:
				PopupItemBase();
				virtual ~PopupItemBase();

				PopupItemBase(const PopupItemBase &a) = default;
				PopupItemBase &operator = (const PopupItemBase &a) = default;

				virtual bool IsDivider() const = 0;

				virtual int GetMessageId() const = 0;
				virtual const std::string &GetTitle() const = 0;
			};
		}
	}
}