#pragma once

namespace ZenDiary
{
	namespace Interfaces
	{
		struct ISerializable
		{
			virtual ZD_STATUS Serialize(JsonBox::Value &root) const = 0;
			virtual ZD_STATUS Deserialize(JsonBox::Value &root) = 0;

			virtual ~ISerializable() { }
		};
	}
}