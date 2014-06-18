#pragma once
#include "DatabaseListItem.h"

namespace ZenDiary
{
	namespace App
	{
		class DatabaseList
		{
		public:
			DatabaseList();
			~DatabaseList();

			DatabaseList(const DatabaseList &a) = default;
			DatabaseList &operator = (const DatabaseList &a) = default;

			ZD_STATUS AddItem(DatabaseListItem *item);
			ZD_STATUS RemoveItem(const std::string &path);
			ZD_STATUS FreeItems();

			size_t GetItemsCount() const;
			DatabaseListItem *GetItemAt(size_t index);

		private:
			std::vector<DatabaseListItem*> m_items;
		};
	};
};