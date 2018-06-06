#ifndef H_OBSERVERS
#define H_OBSERVERS

#include "../Addresses.h"
#include "../Utilities/JSON.h"
#include "../Models/Actor/ActorList.h"

static bool g_Started = false;

static Document g_Settings;

static ActorList* g_ActorList;

static void loadSettings()
{
	//g_Settings = LoadSettings("");
}

static bool init()
{
	g_ActorList = new ActorList(_actorList);

	return g_ActorList != nullptr;
}

static void oActors()
{
	if(g_ActorList != nullptr)
	{
		auto allActors = g_ActorList->getList();
	}
}

static void wObservers(HMODULE hModule)
{
	loadSettings();
	
	if(!init())
	{
		//do something if init fails
	}

	g_Started = true;

	while(g_Started)
	{
		oActors();
		
		Sleep(10);
	}

	FreeLibraryAndExitThread(hModule, 0);
}

#endif