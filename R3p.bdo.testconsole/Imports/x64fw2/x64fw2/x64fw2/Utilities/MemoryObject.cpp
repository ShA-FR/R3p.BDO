#include "MemoryObject.h"

MemoryObject::MemoryObject(__int64 address)
{
	m_Address = address;
}

MemoryObject::~MemoryObject()
{

}

template <typename T>
T MemoryObject::Read(int offset)
{
	return *(T*)(Address + offset);
}

template <typename T>
T MemoryObject::Read(__int64 address)
{
	return *(T*)(address);
}

std::string MemoryObject::ReadASCIIString(int offset)
{
	size_t length = strlen((char*)(m_Address + offset));

	return std::string((char*)(m_Address + offset), length);
}

std::string MemoryObject::ReadASCIIString(__int64 address)
{
	size_t length = strlen((char*)(address));

	return std::string((char*)(address), length);
}

std::wstring MemoryObject::ReadUTF16(int offset)
{
	auto* buffer = (wchar_t*)(m_Address + offset);

	auto strLength = wcslen(buffer);

	return std::wstring(buffer, strLength);
}

std::wstring MemoryObject::ReadUTF16(__int64 address)
{
	auto* buffer = (wchar_t*)(address);

	auto strLength = wcslen(buffer);

	return std::wstring(buffer, strLength);
}

template <typename T>
void MemoryObject::Write(int offset, T value)
{
	*(T*)(Address + offset) = value;
}

template <typename T>
void MemoryObject::Write(__int64 address, T value)
{
	*(T*)(address) = value;
}

void MemoryObject::WriteBytes(__int64 address, BYTE* buffer, size_t size)
{
	memcpy(&address, &buffer, size);
}

void MemoryObject::WriteBytes(int offset, BYTE* buffer, size_t size)
{
	memcpy(&m_Address + offset, &buffer, size);
}