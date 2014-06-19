#include "stdafx.h"
#include "DatabaseVersionUpdate.h"

namespace ZenDiary
{
	namespace App
	{
		DatabaseVersionUpdate::DatabaseVersionUpdate(int version, const std::string &filename) : 
			m_version(version),
			m_filename(filename)
		{

		}

		DatabaseVersionUpdate::~DatabaseVersionUpdate()
		{

		}

		bool operator < (const DatabaseVersionUpdate &a, const DatabaseVersionUpdate &b)
		{
			return a.GetVersion() < b.GetVersion();
		}
	}
}