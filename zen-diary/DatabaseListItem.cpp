#include "stdafx.h"
#include "DatabaseListItem.h"

namespace ZenDiary
{
	namespace App
	{
		DatabaseListItem::DatabaseListItem(const std::string &path, const std::string &name) : 
			m_path(path),
			m_name(name)
		{

		}

		DatabaseListItem::~DatabaseListItem()
		{

		}
	}
}