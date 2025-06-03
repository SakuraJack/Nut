#pragma once

#include "Memory.h"
#include "Base.h"
#include "Log.h"

#include "glm/glm.hpp"

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

		void Zero()
		{
			if (Data)
				memset(Data, 0, Size);
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

		template<typename T>
		T& Read(uint64_t offset = 0)
		{
			return *(T*)((byte*)Data + offset);
		}

		template<typename T>
		T& Read(uint64_t offset = 0) const
		{
			return *(T*)((byte*)Data + offset);
		}

		byte* ReadBytes(uint64_t size, uint64_t offset) const
		{
			if (size + offset > Size)
				NUT_CORE_ERROR_TAG("Memory", "Buffer::ReadBytes: out of range! Size: {0}, Offset: {1}, ReadSize: {2}", Size, offset, size);
			byte* buffer = nnew byte[size];
			memcpy(buffer, (byte*)Data + offset, size);
			return buffer;
		}

		void Write(const void* data, uint64_t size, uint64_t offset)
		{
			if (size + offset > Size)
				NUT_CORE_ERROR_TAG("Memory", "Buffer::Write: out of range! Size: {0}, Offset: {1}, WriteSize: {2}", Size, offset, size);
			memcpy((byte*)Data + offset, data, size);
		}

		operator bool() const
		{
			return Data;
		}

		byte& operator[](uint64_t index)
		{
			return ((byte*)Data)[index];
		}

		const byte& operator[](uint64_t index) const
		{
			return ((byte*)Data)[index];
		}

		template<typename T>
		T* As() const
		{
			return (T*)Data;
		}

		inline uint64_t GetSize() const { return Size; }
	};
}
