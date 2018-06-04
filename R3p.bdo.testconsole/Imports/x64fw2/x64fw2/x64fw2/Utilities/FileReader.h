#ifndef H_FILEREADER
#define H_FILEREADER

#include <Windows.h>
#include <vector>
#include <fstream>
#include <string>

static std::vector<std::string> GetFilesFromDirectory(std::string dirPath)
{
	std::vector<std::string> buffer;

	std::string pattern(dirPath);
	pattern.append("\\*");
	WIN32_FIND_DATA data;
	HANDLE hFind;
	if ((hFind = FindFirstFile(pattern.c_str(), &data)) != INVALID_HANDLE_VALUE) {
		do {			
			if(data.dwFileAttributes != FILE_ATTRIBUTE_DIRECTORY)
				buffer.push_back(dirPath + data.cFileName);
		} while (FindNextFile(hFind, &data) != 0);
		FindClose(hFind);
	}

	return buffer;
}

static char * ReadFileBytes(std::string filePath, int &readBytes)
{	
	std::ifstream ifs(filePath, std::ios::binary | std::ios::ate);
	std::ifstream::pos_type pos = ifs.tellg();

	if (!ifs.is_open())
		return nullptr;

	int length = pos;

	char *pChars = new char[length];
	ifs.seekg(0, std::ios::beg);
	ifs.read(pChars, length);

	ifs.close();
	readBytes = length;
	return pChars;
}

static std::vector<std::string> ReadFileLines(std::string filePath)
{	
	std::vector<std::string> buffer;

	std::fstream in(filePath);
	std::string line;

	int index = 0;

	while (std::getline(in, line))
	{
		if (index > 0)
		{
			buffer.push_back(line);
		}

		index++;
	}

	return buffer;
}


static void replaceAll(std::string& str, const std::string& from, const std::string& to) {
	if (from.empty())
		return;
	int start_pos = 0;
	while ((start_pos = str.find(from, start_pos)) != std::string::npos) {
		str.replace(start_pos, from.length(), to);
		start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
	}
}

static std::vector<std::string> SplitText(std::string text, std::string delimiter)
{	
	std::vector<std::string> buffer;

	while (text.find(delimiter) != std::string::npos)
	{
		int pos = text.find(delimiter);

		std::string subStr = text.substr(0, pos);

		buffer.push_back(subStr);

		text.erase(0, pos + delimiter.length());
	}

	buffer.push_back(text);

	return buffer;
}

static std::vector<int> SplitIntText(std::string text, std::string delimiter)
{
	std::vector<int> buffer;

	while (text.find(delimiter) != std::string::npos)
	{
		int pos = text.find(delimiter);

		int value = std::stoi(text.substr(0, pos));

		buffer.push_back(value);

		text.erase(0, pos + delimiter.length());
	}

	buffer.push_back(std::stoi(text));

	return buffer;
}

static std::vector<float> SplitFloatText(std::string text, std::string delimiter)
{
	std::vector<float> buffer;

	while (text.find(delimiter) != std::string::npos)
	{
		int pos = text.find(delimiter);

		float value = std::stof(text.substr(0, pos));

		buffer.push_back(value);

		text.erase(0, pos + delimiter.length());
	}

	buffer.push_back(std::stof(text));

	return buffer;
}

#endif