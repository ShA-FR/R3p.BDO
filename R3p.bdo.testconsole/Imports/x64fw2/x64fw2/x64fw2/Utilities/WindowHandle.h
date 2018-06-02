#pragma once

#include <Windows.h>
#include <cstdio>

__declspec(selectany) HWND _wHWND;

static HWND GetDynamicWindowHandle()
{

	if (_wHWND != NULL)
		return _wHWND;

	// Get first window on desktop
	HWND firstwindow = FindWindowEx(NULL, NULL, NULL, NULL);
	HWND window = firstwindow;
	char windowtext[MAX_PATH];

	// We need to get the console title in case we
	// accidentally match the search word with it
	// instead of the intended target window.
	/*char consoletitle[MAX_PATH];
	GetConsoleTitleA(consoletitle, MAX_PATH);*/

	while (1)
	{
		fprintf(stderr, ".");

		// Check window title for a match
		GetWindowTextA(window, windowtext, MAX_PATH);
		if (strstr(windowtext, "BLACK") != NULL) 
			break;

		// Get next window
		window = FindWindowEx(NULL, window, NULL, NULL);
		if (window == NULL || window == firstwindow)
		{
			return 0;
		}
	}
	//printf("\n[+] Dynamic window found: %s\n", windowtext);

	_wHWND = window;

	return _wHWND;
}
