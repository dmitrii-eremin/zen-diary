#include "stdafx.h"
#include "ZenDataSource.h"

#ifdef _DEBUG
#define ZD_SUCCESSIVELY_REQUESTS
#endif

namespace ZenDiary
{
	namespace App
	{
		ZenDataSource::ZenDataSource()
		{
			char cd[MAX_PATH];
			::GetCurrentDirectory(MAX_PATH, cd);
			m_current_directory = cd;
		}

		ZenDataSource::~ZenDataSource()
		{

		}

		void ZenDataSource::OnRequest(int request_id, const Awesomium::ResourceRequest& request, const Awesomium::WebString& path)
		{
			std::string uri(Awesomium::ToString(request.url().path()));

			uri = Helpers::String::UriDecode(uri);
			uri = Helpers::String::ConvertUtf8ToMB(uri);
			
			std::string fullname;

			if (uri.length() > 0 && Helpers::Files::IsFileExist(uri.substr(1)))
			{
				fullname = uri.substr(1);
			}
			else
			{
				fullname = m_home_path + uri.substr(1);
			}

			ZenThreadParam *p = new ZenThreadParam();
			p->object = this;
			p->fullname = fullname;
			p->request_id = request_id;

#ifdef ZD_SUCCESSIVELY_REQUESTS
			ThreadProc(p);
#else
			_beginthread(&ZenDataSource::ThreadProc, 0, p);
#endif
		}

		ZD_STATUS ZenDataSource::AddMimeType(const std::string &key, const std::string &value)
		{
			m_mime_types[key] = value;
			return ZD_NOERROR;
		}

		const std::string &ZenDataSource::GetMimeType(const std::string &key)
		{
			auto i = m_mime_types.find(Helpers::String::ToLower(key));
			if (i == m_mime_types.end())
			{
				static std::string stub("text/html");
				return stub;
			}

			return i->second;
		}

		void ZenDataSource::ThreadProc(void *pParam)
		{
			if (!pParam)
			{
				return;
			}				

			ZenThreadParam *p = reinterpret_cast<ZenThreadParam*>(pParam);

			std::string ext = std::string(".") + Helpers::String::ExtractExtension(p->fullname);
			const std::string &mime_type = p->object->GetMimeType(ext);	

			::SetCurrentDirectory(p->object->GetCurrentDirectory().c_str());

			if (Helpers::Files::IsFileExist(p->fullname))
			{
				size_t fsize = 0;
				if (ZD_SUCCEEDED(Helpers::Files::GetFileSize(p->fullname, fsize)))
				{
					if (fsize > 0)
					{
						char *data = new char[fsize];

						ZD_STATUS status = Helpers::Files::GetFileContent(p->fullname, data, fsize);
						if (ZD_SUCCEEDED(status))
						{					
							p->object->SendResponse(p->request_id, fsize,
								reinterpret_cast<unsigned char*>(data),
								Awesomium::WSLit(mime_type.c_str()));
						}

						delete[]data; 
					}
					else
					{
						p->object->SendResponse(p->request_id, 0,
							reinterpret_cast<unsigned char*>(""),
							Awesomium::WSLit(mime_type.c_str()));
					}
				}
			}
			else
			{
				p->object->SendResponse(p->request_id, 0,
					reinterpret_cast<unsigned char*>(""),
					Awesomium::WSLit(mime_type.c_str()));
			}

			delete p;
		}		
	}
}