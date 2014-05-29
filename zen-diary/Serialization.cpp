#include "stdafx.h"
#include "ZenDiary.h"

namespace ZenDiary
{
	namespace Helpers
	{
		namespace Serialization
		{
			ZD_STATUS ToFile(const Interfaces::ISerializable &object, const std::string &filename)
			{
				JsonBox::Value root;

				ZD_RETURN_IF_FAILED(object.Serialize(root));

				std::string json = String::To(root);

				return Files::SetFileContent(filename, json);
			}

			ZD_STATUS FromFile(Interfaces::ISerializable &object, const std::string &filename)
			{
				std::string json;
				ZD_RETURN_IF_FAILED(Files::GetFileContent(filename, json));

				JsonBox::Value root;
				root.loadFromString(json);

				return object.Deserialize(root);
			}
		}
	}
}