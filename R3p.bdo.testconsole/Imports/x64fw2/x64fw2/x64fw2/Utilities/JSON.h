#pragma once
#include "../rapidjson/rapidjson.h"
#include "../rapidjson/reader.h"
#include "../rapidjson/filereadstream.h"
#include <iostream>
#include <fstream>
#include "../rapidjson/document.h"
#include "FileReader.h"
#include "../rapidjson/writer.h"
#include "../rapidjson/prettywriter.h"

using namespace rapidjson;

static Document CreateDefault(std::string filePath)
{	
	Document document;
	document.SetObject();

	Value speedhack(kObjectType);
	Value player(kObjectType);
	Value vehicle(kObjectType);

	player.AddMember("Enable", true, document.GetAllocator());
	player.AddMember("Atk", 100, document.GetAllocator());
	player.AddMember("Cst", 100, document.GetAllocator());
	player.AddMember("Mvmt", 200, document.GetAllocator());

	speedhack.AddMember("Player", player, document.GetAllocator());

	vehicle.AddMember("Enable", true, document.GetAllocator());
	vehicle.AddMember("Accel", 500, document.GetAllocator());
	vehicle.AddMember("Speed", 500, document.GetAllocator());
	vehicle.AddMember("Turn", 500, document.GetAllocator());
	vehicle.AddMember("Brake", 500, document.GetAllocator());

	speedhack.AddMember("Vehicle", vehicle, document.GetAllocator());

	document.AddMember("Speedhack", speedhack, document.GetAllocator());
		
	StringBuffer strbuf;
	PrettyWriter<StringBuffer> writer(strbuf);
	document.Accept(writer);

	std::ofstream outputFile(filePath);
	outputFile << strbuf.GetString();
	outputFile.close();

	return document;
}

static Document LoadSettings(std::string filePath)
{	
	int bytesRead = 0;

	char* fileBuffer = ReadFileBytes(filePath, bytesRead);

	if (fileBuffer == nullptr)
		return CreateDefault(filePath);

	Document d;
	d.Parse(fileBuffer, bytesRead);

	return d;
}
