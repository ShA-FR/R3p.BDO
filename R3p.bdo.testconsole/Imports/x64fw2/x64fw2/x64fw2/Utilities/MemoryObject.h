#ifndef H_MEMORYOBJECT
#define H_MEMORYOBJECT

#include <Windows.h>
#include <vector>

class MemoryObject
{
public:
	__int64 m_Address;

	MemoryObject(__int64 address);
	~MemoryObject();

	template <typename T>
	T Read(int offset);

	template <typename T>
	T Read(__int64 address);
			
	std::string ReadASCIIString(int offset);
	std::string ReadASCIIString(__int64 address);

	std::wstring ReadUTF16(int offset);
	std::wstring ReadUTF16(__int64 address);

	template <typename T>
	void Write(int offset, T value);

	template <typename T>
	void Write(__int64 address, T value);

	void WriteBytes(__int64 address, BYTE* buffer, size_t size);
	void WriteBytes(int offset, BYTE* buffer, size_t size);
};

#endif