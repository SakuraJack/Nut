#pragma once

#include "Memory.h"
#include "Core.h"


namespace Nut
{
	struct Buffer
	{
		void* Data = nullptr;
		uint64_t Size = 0;

		Buffer()
			: Data(nullptr), Size(0) 
		{
		}

		Buffer(const void* data, uint64_t size = 0)
			: Data((void*)data), Size(size)
		{
		}

		void Allocate(uint64_t size)
		{
			delete[](byte*)Data;
			Data = nullptr;

			if (size == 0) return;
			Data = nnew byte[size];
			Size = size;
		}

		void Release()
		{
			delete[](byte*)Data;
			Data = nullptr;
			Size = 0;
		}

		static Buffer Copy(const Buffer& other)
		{
			Buffer buffer;
			buffer.Allocate(other.Size);
			memcpy(buffer.Data, other.Data, other.Size);
			return buffer;
		}

		static Buffer Copy(const void* data, uint64_t size)
		{
			Buffer buffer;
			buffer.Allocate(size);
			memcpy(buffer.Data, data, size);
			return buffer;
		}
	};
}
