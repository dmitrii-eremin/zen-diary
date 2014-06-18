#include "stdafx.h"
#include "DatabaseList.h"

namespace ZenDiary
{
	namespace App
	{
		DatabaseList::DatabaseList()
		{

		}

		DatabaseList::~DatabaseList()
		{
			FreeItems();
		}

		ZD_STATUS DatabaseList::AddItem(DatabaseListItem *item)
		{
			if (item)
			{
				for (auto &i : m_items)
				{
					if (!i)
					{
						continue;
					}

					if (i->GetPath() == item->GetPath())
					{
						delete item;
						return ZD_NOERROR;
					}
				}

				m_items.push_back(item);
				return ZD_NOERROR;
			}

			return ZD_ERROR_INVALID_ARGUMENT;
		}

		ZD_STATUS DatabaseList::RemoveItem(const std::string &path)
		{
			m_items.erase(std::remove_if(m_items.begin(), m_items.end(), [&path](DatabaseListItem *item) -> bool
			{
				if (!item)
				{
					return true;
				}

				if (item->GetPath() == path)
				{
					delete item;
					return true;
				}				

				return false;
			}), m_items.end());
			return ZD_NOERROR;
		}

		ZD_STATUS DatabaseList::FreeItems()
		{
			for (auto &i : m_items)
			{
				ZD_SAFE_DELETE(i);
			}

			m_items.clear();

			return ZD_NOERROR;
		}

		size_t DatabaseList::GetItemsCount() const
		{
			return m_items.size();
		}

		DatabaseListItem *DatabaseList::GetItemAt(size_t index)
		{
			if (index >= m_items.size())
			{
				return nullptr;
			}

			return m_items[index];
		}
	}
}